"""
    File defining individual parts of dual path architecture.
"""
from abc import ABC, abstractmethod

import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F

# Tensor format batch x channels x H x W
from typing import List

get_channels = lambda ten: ten[0]
get_resolution = lambda ten: (ten[1], ten[2])

class BackboneBase(nn.Module):
    """
        Class works as backbone class template, each backbone has to inherited it
    """

    def __init__(self):
        super().__init__()

        # Variable holds outputs for sides
        self.side_outputs = []

    @abstractmethod
    def get_side_outputs(self):
        pass

    @abstractmethod
    def forward(self, x):
        pass


class SpacialPath(nn.Module):
    """
        Spacial path class implementation, build in by the layer output description.
        Remember the sum of channels in side_outputs should equal to that in backbone_out
    """

    def __init__(self, side_outputs, backbone_out, target_x, target_y, batch_size=2):
        super().__init__()
        self.layer_outputs = side_outputs
        self.cams = []

        self.batch = batch_size

        # Get target resolution values of 1st block
        self.x, self.y = get_resolution(side_outputs[0])

        # Create list of CAM blocks
        for ten in side_outputs:
            self.cams.append(CAM(get_channels(ten), get_resolution(ten)))
        # GAP operator
        self.GAP = nn.AvgPool2d(get_resolution(backbone_out))
        # Upsampling
        self.up = nn.Upsample(size=(self.x, self.y), mode='nearest')
        # Final upsampling with convolution
        spacial_path_channels = sum([get_channels(cam) for cam in side_outputs])
        self.semantic = nn.Sequential(
            nn.Conv2d(get_channels(backbone_out) + spacial_path_channels,
                      get_channels(backbone_out) // 4, kernel_size=3, padding=1),
            nn.Conv2d(get_channels(backbone_out) // 4, 3, kernel_size=3, padding=1),
            nn.Upsample(scale_factor=2, mode='bilinear'),
            nn.Upsample(scale_factor=2, mode='bilinear'),
            nn.Upsample(scale_factor=2, mode='bilinear'),
            nn.Upsample(scale_factor=2, mode='bilinear'),
            nn.Upsample(scale_factor=2, mode='bilinear'),
            nn.Upsample(scale_factor=2, mode='bilinear'),
            nn.Upsample(scale_factor=2, mode='bilinear'),
            nn.Upsample(size=(target_x, target_y), mode='bilinear')
        )

    def _add_padding(self, input):
        """
            Add padding to last 2 dimensions to have same resolution as 1 CAM block
            It should secure same resolution of CAM blocks outputs
        """
        x, y = input.size()[2], input.size()[3]
        diffX = self.x - x
        diffY = self.y - y
        input = F.pad(input, [diffY // 2, diffY - diffY // 2, diffX // 2, diffX - diffX // 2])
        return input

    def forward(self, ten_x: List[torch.tensor], gap_tensor):
        """
            List of tensors as input into spatial path, side output from backbone
            Gap tensor stands for the final output from backbone pipeline
        """
        cams_outputs = []
        final_outputs = []
        # Each CAM block is applied on its own side input
        for i, x_in in enumerate(ten_x):
            cam_out = self.cams[i](x_in)
            repaded = self._add_padding(cam_out)
            cams_outputs.append(repaded)
        # The first one is without addition
        final_outputs.append(cams_outputs[0])
        # Add individual outputs to next output
        for i in range(1, len(cams_outputs)):
            # Fill previous output with zero channels to have same channel count
            zero_channel_cnt = get_channels(self.layer_outputs[i]) - get_channels(self.layer_outputs[i-1])
            zero_tensor = torch.zeros((self.batch, zero_channel_cnt, 64, 64))
            filled_channels = torch.cat((cams_outputs[i-1], zero_tensor), 1)
            # Add to current output previous CAM block output
            final_outputs.append(filled_channels + cams_outputs[i])

        # On the last output of backbone the GAP operation is applied
        gap_out = self.GAP(gap_tensor)
        upsampled = self.up(gap_out)
        final_outputs.append(upsampled)
        # Concatenate CAM outputs over channel dimension
        concated = torch.cat(tuple(final_outputs), 1)
        return self.semantic(concated)

# Methods for edge Holistic edge detection class
def crop(variable, th, tw):
    h, w = variable.shape[2], variable.shape[3]
    x1 = int(round((w - tw) / 2.))
    y1 = int(round((h - th) / 2.))
    return variable[:, :, y1: y1 + th, x1: x1 + tw]


def make_bilinear_weights(size, num_channels):
    factor = (size + 1) // 2
    if size % 2 == 1:
        center = factor - 1
    else:
        center = factor - 0.5
    og = np.ogrid[:size, :size]
    filt = (1 - abs(og[0] - center) / factor) * (1 - abs(og[1] - center) / factor)
    filt = torch.from_numpy(filt)
    w = torch.zeros(num_channels, num_channels, size, size)
    w.requires_grad = False
    for i in range(num_channels):
        for j in range(num_channels):
            if i == j:
                w[i, j] = filt
    return w


def upsample(input, stride, num_channels=1):
    kernel_size = stride * 2
    kernel = make_bilinear_weights(kernel_size, num_channels).cuda()
    return torch.nn.functional.conv_transpose2d(input, kernel, stride=stride)


class EdgePath(nn.Module):
    """
        Edge path class, implementation of Holistically-Nested Edge Detection
        https://github.com/meteorshowers/hed
    """
    def __init__(self, side_outputs, target_h, target_w):
        super().__init__()
        self.heds = []
        # Get target resolution values of 1st block
        self.x, self.y = get_resolution(side_outputs[0])
        # Prepare convolutions by the inputs
        for input in side_outputs:
            conv = nn.Conv2d(get_channels(input), 1, 1)
            self.heds.append(conv)
        self.score_final = nn.Conv2d(len(side_outputs), 1, 1)
        # Upsample to final resolution
        self.upsampler = nn.Sequential(
                    nn.Upsample(scale_factor=2, mode='bilinear'),
                    nn.Upsample(scale_factor=2, mode='bilinear'),
                    nn.Upsample(scale_factor=2, mode='bilinear'),
                    nn.Upsample(scale_factor=2, mode='bilinear'),
                    nn.Upsample(scale_factor=2, mode='bilinear'),
                    nn.Upsample(scale_factor=2, mode='bilinear'),
                    nn.Upsample(scale_factor=2, mode='bilinear'),
                    nn.Upsample(size=(target_h, target_w), mode='bilinear')
                )

    def _add_padding(self, side_input):
        """
            Add padding to last 2 dimensions to have same resolution as 1 CAM block
            It should secure same resolution of CAM blocks outputs
        """
        x, y = side_input.size()[2], side_input.size()[3]
        diffX = self.x - x
        diffY = self.y - y
        side_input = F.pad(side_input, [diffY // 2, diffY - diffY // 2, diffX // 2, diffX - diffX // 2])
        return side_input

    def forward(self, xs: List[torch.tensor]):
        heds_out = []
        for conv, x in zip(self.heds, xs):
            heds_out.append(conv(x))
        upsamples = [heds_out[0]]
        size = 4
        for so in heds_out[1:]:
            weight_deconv = make_bilinear_weights(size, 1)
            transposed = F.conv_transpose2d(so, weight_deconv, stride=size//2)
            upsamples.append(self._add_padding(transposed))
            size *= 2
        # Apply cropping
        fusecat = torch.cat(tuple(upsamples), dim=1)
        fuse = self.score_final(fusecat)
        result = torch.sigmoid(fuse)
        up_res = self.upsampler(result)
        return up_res

class CAM(nn.Module):
    """
        Channel Attention Module (CAM) implementation.
        3x3Conv -> BatchNorm -> ReLU -+-> GAP -> 1x1Conv -> BatchNorm -> Sigmoid -> * ->
                                      L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ ^
    """

    def __init__(self, channels, resolution):
        super().__init__()

        dim_x, dim_y = resolution

        self.conv = nn.Sequential(
            nn.Conv2d(channels, channels, kernel_size=3, padding=1),
            nn.BatchNorm2d(channels),
            nn.ReLU(inplace=True),
        )
        self.inner = nn.Sequential(
            nn.AvgPool2d((dim_x, dim_y)),
            # Make global pooling https://discuss.pytorch.org/t/global-average-pooling-in-pytorch/6721
            nn.Conv2d(channels, channels, kernel_size=1),  # Learn weight to get appropriate attention to each feature
            nn.BatchNorm2d(channels),
            nn.Sigmoid()
        )

    def _forward_impl(self, x):
        x = self.conv(x)
        y = self.inner(x)
        out = x * y
        return out

    def forward(self, x):
        return self._forward_impl(x)

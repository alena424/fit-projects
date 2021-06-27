""""
    Original implementation: https://github.com/tonylins/pytorch-mobilenet-v2
    modified for dual path experiments
"""

import torch.nn as nn
import math

from modelParts import BackboneBase


def conv_bn(inp, oup, stride):
    return nn.Sequential(
        nn.Conv2d(inp, oup, 3, stride, 1, bias=False),
        nn.BatchNorm2d(oup),
        nn.ReLU6(inplace=True)
    )


def conv_1x1_bn(inp, oup):
    return nn.Sequential(
        nn.Conv2d(inp, oup, 1, 1, 0, bias=False),
        nn.BatchNorm2d(oup),
        nn.ReLU6(inplace=True)
    )


def make_divisible(x, divisible_by=8):
    import numpy as np
    return int(np.ceil(x * 1. / divisible_by) * divisible_by)


class BRB(nn.Module):
    """
    Implementation of BRB block as describe in dual path article
    """
    def __init__(self, inp, oup, stride, expand_ratio, atrous=1):
        super(BRB, self).__init__()
        self.stride = stride
        assert stride in [1, 2]

        hidden_dim = int(inp * expand_ratio)
        self.use_res_connect = self.stride == 1 and inp == oup

        if expand_ratio == 1:
            self.conv = nn.Sequential(
                # dw
                nn.Conv2d(hidden_dim, hidden_dim, 3, stride, 1, groups=hidden_dim, bias=False),
                nn.BatchNorm2d(hidden_dim),
                nn.ReLU6(inplace=True),
                # pw-linear
                nn.Conv2d(hidden_dim, oup, 1, 1, 0, bias=False),
                nn.BatchNorm2d(oup),
            )
        else:
            self.conv = nn.Sequential(
                # pw
                nn.Conv2d(inp, hidden_dim, 1, 1, 0, dilation=atrous, bias=False),
                nn.BatchNorm2d(hidden_dim),
                nn.ReLU6(inplace=True),
                # dw
                nn.Conv2d(hidden_dim, hidden_dim, 3, stride, 1, groups=hidden_dim, bias=False),
                nn.BatchNorm2d(hidden_dim),
                nn.ReLU6(inplace=True),
                # pw-linear
                nn.Conv2d(hidden_dim, oup, 1, 1, 0, bias=False),
                nn.BatchNorm2d(oup),
            )

    def forward(self, x):
        if self.use_res_connect:
            return x + self.conv(x)
        else:
            return self.conv(x)


class MobileNetV2(BackboneBase):
    def __init__(self, BRB_setting, input_size=224, width_mult=1.):
        super(MobileNetV2, self).__init__()
        block = BRB
        input_channel = 32
        last_channel = 1280

        # Setup output block to spacial and edge path
        side_output_blocks = [3, 4, 5, 6, 7]

        # building first layer
        assert input_size % 32 == 0
        # input_channel = make_divisible(input_channel * width_mult)  # first channel is always 32!
        self.last_channel = make_divisible(last_channel * width_mult) if width_mult > 1.0 else last_channel
        self.features = [conv_bn(3, input_channel, 2)]
        block_i = 1
        # building inverted residual blocks
        for t, c, n, s, at in BRB_setting:
            output_channel = make_divisible(c * width_mult) if t > 1 else c
            for i in range(n):
                if i == 0:
                    self.features.append(block(input_channel, output_channel, s, expand_ratio=t, atrous=at))
                else:
                    self.features.append(block(input_channel, output_channel, 1, expand_ratio=t))
                input_channel = output_channel
            # Check if it should be used as side output for forward method
            if block_i in side_output_blocks:
                self.side_outputs.append(len(self.features)-1)
            block_i += 1

        # # For backbone learning make it sequetial
        # # make it nn.Sequential
        # self.sequential = nn.Sequential(*self.features)
        #
        # # building classifier
        # self.classifier = nn.Linear(self.last_channel, n_class)

        self._initialize_weights()

    def get_side_outputs(self):
        return self.side_outputs

    def forward(self, x):
        # make it nn.Sequential
        results = []
        x = self.features[0](x)
        for i, block in enumerate(self.features[1:], 1):
            x = block(x)
            if i in self.side_outputs:
                results.append(x)
        return results

    def _initialize_weights(self):
        for m in self.modules():
            if isinstance(m, nn.Conv2d):
                n = m.kernel_size[0] * m.kernel_size[1] * m.out_channels
                m.weight.data.normal_(0, math.sqrt(2. / n))
                if m.bias is not None:
                    m.bias.data.zero_()
            elif isinstance(m, nn.BatchNorm2d):
                m.weight.data.fill_(1)
                m.bias.data.zero_()
            elif isinstance(m, nn.Linear):
                n = m.weight.size(1)
                m.weight.data.normal_(0, 0.01)
                m.bias.data.zero_()
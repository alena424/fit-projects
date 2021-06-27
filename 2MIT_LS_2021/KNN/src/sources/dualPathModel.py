"""
    This class is template for dual-path architecture for semantic segmentation of high resolution aerial space images.
    It is composed of three main parts typical for dual path architecture given in article:
        1) Backbone model: responsible for encoding image to inner representation -> class inherited by Backbone class
    Source of article: https://www.mdpi.com/2220-9964/8/12/582/htm
"""

from torch import nn

from modelParts import SpacialPath, EdgePath, BackboneBase
from mobileNet import MobileNetV2

class DualPathModel(nn.Module):
    """
        Connect all parts together to one model Dualpath
    """
    def __init__(self, res_x, res_y):
        """
        Setting done for 512x512
        :param res_x: Square resolution of input images
        :param res_y: Square resolution of input images res_x == res_y
        """
        super(DualPathModel, self).__init__()
        assert (res_x == res_y)
        # Setting of mobileNetV2 model
        BRB_setting = [
            # t, c, n, s, at
            [1, 16, 1, 1, 1],
            [6, 24, 2, 2, 1],
            [6, 32, 3, 2, 1],
            [6, 64, 4, 1, 2],
            [6, 96, 3, 1, 4],
            [6, 160, 3, 1, 8],
            [6, 320, 1, 1, 16],
        ]
        # Setting for spacial and path model
        side_output_settings = [
            # channels, H, W, channels_out
            (32, 64, 64),
            (64, 61, 61),  # Dilatation make resolution
            (96, 53, 53),
            (160, 38, 38),
            (320, 33, 33)
        ]
        backbone_output = side_output_settings[-1]
        self.backbone = MobileNetV2(BRB_setting=BRB_setting, input_size=512)
        self.spacial_path = SpacialPath(side_outputs=side_output_settings, backbone_out=backbone_output,
                                        target_x=res_x, target_y=res_y)
        self.edge_path = EdgePath(side_outputs=side_output_settings, target_h=res_x, target_w=res_y)

    def forward(self, x):
        xs = self.backbone(x)
        gap_tensor = xs[-1]
        spatial = self.spacial_path(xs, gap_tensor)
        edge = self.edge_path(xs)
        return spatial, edge

if __name__ == '__main__':
    print('ahojda', torch.ones(2))
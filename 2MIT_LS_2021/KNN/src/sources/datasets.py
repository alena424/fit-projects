import torch
import numpy as np
from skimage.io import imread
from torch.utils import data


class SegmentationDataSet(data.Dataset):
    def __init__(self,
                 inputs: list,
                 targets: list,
                 transform=None
                 ):
        self.inputs = inputs
        self.targets = targets
        self.inputs_dtype = torch.float32
        self.targets_dtype = torch.long
        self.transform = transform

    def __len__(self):
        return len(self.inputs)

    def __getitem__(self,
                    index: int):
        # Select the sample
        input_ID = self.inputs[index]
        target_ID = self.targets[index]

        # Load input and target
        x, y = imread(input_ID), imread(target_ID)

        # Preprocessing
        if self.transform is not None:
            x, y = self.transform(x, y)

        # Typecasting
        x, y = torch.from_numpy(x).type(self.inputs_dtype), torch.from_numpy(y).type(self.targets_dtype)

        return x, y

class MoveAxis:
    """From [H, W, C] to [C, H, W]"""

    def __init__(self, transform_input: bool = True, transform_target: bool = False):
        self.transform_input = transform_input
        self.transform_target = transform_target

    def __call__(self, inp: np.ndarray, tar: np.ndarray):
        if self.transform_input: inp = np.moveaxis(inp, -1, 0)
        if self.transform_target: tar = np.moveaxis(tar, -1, 0)

        return inp, tar

    def __repr__(self):
        return str({self.__class__.__name__: self.__dict__})
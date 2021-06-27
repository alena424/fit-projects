import pathlib

from cv2.cv2 import imread, resize
from sklearn.model_selection import train_test_split
from tqdm import tqdm
import numpy as np
from predict import predict
from sources.couch import Couch

import torch
import torch.nn as nn
import torch.utils.data as data
import torchvision.utils as utils
import torchvision.datasets as dataset
import torchvision.transforms as transforms

from pathlib import Path
from customdatasets import SegmentationDataSet

from unet import UNet
from transformations import Resize, Compose, MoveAxis

batch_size = 1
img_size = 256
lr = 0.0002
epoch = 1

# select device
if torch.cuda.is_available():
    device = torch.device('cuda')
else:
    device = torch.device('cpu')

# aerial directory
directory = "../../aerial_dataset/train/"
# reference directory
directory2 = "../data"

img_dir = Path(directory)
root = img_dir

# Only for comparing results
img_dir2 = Path(directory)
img_data = dataset.ImageFolder(root=img_dir2, transform=transforms.Compose([
    transforms.Resize(size=img_size),
    transforms.CenterCrop(size=(img_size, img_size)),
    transforms.ToTensor(),
]))

# random seed
random_seed = 42

# split dataset into training set and validation set
train_size = 0.8  # 80:20 split


def get_filenames_of_path(path: pathlib.Path, ext: str = '*'):
    """Returns a list of files in a directory/path. Uses pathlib."""
    filenames = [file for file in path.glob(ext) if file.is_file()]
    return filenames


inputs = get_filenames_of_path(root / 'Input')
targets = get_filenames_of_path(root / 'Target')

targets_train, targets_valid = train_test_split(
    targets,
    random_state=random_seed,
    train_size=train_size,
    shuffle=True)

inputs_train, inputs_valid = train_test_split(
    inputs,
    random_state=random_seed,
    train_size=train_size,
    shuffle=True)

transforms_training = Compose([
    Resize(input_size=(256, 256, 3), target_size=(256, 256)),
    MoveAxis(transform_target=True)
    # transforms.CenterCrop(size=(img_size, img_size)),
])

dataset_train = SegmentationDataSet(inputs=inputs_train,
                                    targets=targets_train,
                                    transform=transforms_training)

img_batch = data.DataLoader(dataset=dataset_train,
                            batch_size=batch_size,
                            shuffle=True)

img_batch_reference = data.DataLoader(dataset=img_data,
                            batch_size=batch_size,
                            shuffle=True)

model = UNet(3, 3, 64).to(device) #dim in, dim out, number of filters
model_path = '../model/unet2.pkl'
try:
    model = torch.load(Path(model_path))
except:
    pass

mse = nn.MSELoss()
optimizer = torch.optim.Adam(model.parameters(), lr=lr)

for i in range(epoch):
    #couch = Couch(directory)
    #s_example, s_reference, s_to_compare = couch.get_train_triple(couch.get_iterable_trains()[0])
    batch_iter = tqdm(enumerate(img_batch), 'Training', total=len(img_batch),
                      leave=False)
    for _, (x,y) in batch_iter:
        # print("ted")
        # print(x.size())
        # print(y.size())
        # print(f'x = shape: {x.shape}; type: {x.dtype}')
        # print(f'x = min: {x.min()}; max: {x.max()}')
        # print(f'y = shape: {y.shape}; class: {y.unique()}; type: {y.dtype}')

        optimizer.zero_grad()

        x, y_ = x.to(device), y.to(device)
        y = model.forward(x)
        loss = mse(y.float(), y_.float())
        loss.backward()
        optimizer.step()

        if _ % 10 == 0:
            print(i)
            print(loss)
            utils.save_image(x.cpu().data, Path("../result/original_image_{}_{}.png".format(i, _)))
            y_data = y_.cpu().data.float()
            y2data = y.cpu().data.float()
            utils.save_image(y_data, Path("../result/label_image_{}_{}.png".format(i, _)))
            utils.save_image(y2data, Path("../result/gen_image_{}_{}.png".format(i, _)))
            torch.save(model, Path(model_path))


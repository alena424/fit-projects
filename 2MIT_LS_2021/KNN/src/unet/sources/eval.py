from pathlib import Path

import numpy as np
# from predict import predict
import torch
# from catalyst.contrib.nn.criterion import iou
from catalyst.metrics import iou
from skimage.io import imread
from skimage.transform import resize
import torchvision.utils as utils
from PIL import Image
import catalyst.utils
# preprocess function
from sklearn.model_selection import train_test_split
from torch.utils.data import DataLoader
# from trainer import get_filenames_of_path, device, model_path
import os
import napari

from tqdm import tqdm, gui
from demo.inference import predict
from sources.datasets import SegmentationDataSet, MoveAxis

model_path = '../model/unet2.pkl'
root = "../../aerial_dataset/train/"

#img_dir = Path(directory)

#root = 'aerial_dataset/train/'
root = '../../sources/tmp-data-512x512-from-1-pictures'

preprocessor = MoveAxis(True, True)

# input and target files
inputs = list(map(lambda x: os.path.join(os.path.join(root, 'origins'), x), os.listdir(os.path.join(root, 'origins'))))
targets = list(map(lambda x: os.path.join(os.path.join(root, 'classes'), x), os.listdir(os.path.join(root, 'classes'))))


# random seed
random_seed = 42

# split dataset into training set and validation set
train_size = 0.8  # 80:20 split

inputs_train, inputs_valid = train_test_split(
    inputs,
    random_state=random_seed,
    train_size=train_size,
    shuffle=True)

targets_train, targets_valid = train_test_split(
    targets,
    random_state=random_seed,
    train_size=train_size,
    shuffle=True)

# inputs_train, inputs_valid = inputs[:80], inputs[80:]
# targets_train, targets_valid = targets[:80], targets[:80]

# dataset training
dataset_train = SegmentationDataSet(inputs=inputs_train,
                                    targets=targets_train,
                                    transform=preprocessor)

# dataset validation
dataset_valid = SegmentationDataSet(inputs=inputs_valid,
                                    targets=targets_valid,
                                    transform=preprocessor)

# dataloader training
dataloader_training = DataLoader(dataset=dataset_train,
                                 batch_size=2,
                                 shuffle=True)

# dataloader validation
dataloader_validation = DataLoader(dataset=dataset_valid,
                                   batch_size=2,
                                   shuffle=True)

# device
if torch.cuda.is_available():
    device = torch.device('cuda')
else:
    device = torch.device('cpu')



def preprocess(img: np.ndarray):
    #img = np.moveaxis(img, -1, 0)  # from [H, W, C] to [C, H, W]
    #img = normalize_01(img)  # linear scaling to range [0-1]
    #img = np.expand_dims(img, axis=0)  # add batch dimension [B, C, H, W]
    #img = img.astype(np.float32)  # typecasting to float32
    return img


# postprocess function
def postprocess(img: torch.tensor):
    img = torch.argmax(img, dim=1)  # perform argmax to generate 1 channel
    img = img.cpu().numpy()  # send to cpu and transform to numpy.ndarray
    img = np.squeeze(img)  # remove batch dim and channel dim -> [H, W]
    #img = re_normalize(img)  # scale it to the range [0-255]
    return img

# Running eval
print("evaluation starts...")
batch_iter = tqdm(enumerate(dataloader_training), 'Evaluation', total=len(dataloader_training),leave=False)
model = torch.load(Path(model_path))

for _, (input,targets) in batch_iter:
    #print(inputs, targets)
    # IoU metric
    output = predict(input, model, preprocess, postprocess, device)
    score = iou(outputs=output, targets=targets, class_dim=1,mode="micro")
    print("score is: ", score)
    # save image from tensor
    # viewer = napari.Viewer()
    #
    # # idx = 1
    # img_nap = viewer.add_image(input, name='Input')
    # tar_nap = viewer.add_labels(output, name='Target')
    # out_nap = viewer.add_labels(targets, name='Prediction')


# predict the segmentation maps
# # images_names = get_filenames_of_path(img_dir / 'Input')
# print(images_names)
#images = [imread(img_name) for img_name in images_names]

# images_res = [resize(img, (256, 256, 3)) for img in images]
i = 0

model = torch.load(Path(model_path))

# for img in images_res:
#     output = predict(img, model, preprocess, postprocess, device)
#     iou (output, targets)


    #score = metriioU(output, reference)
    # im = Image.fromarray(output)
    # im.savePath("../result/eval_{}.png".format(i))
    #utils.save_image(output, Path("../result/eval_{}.png".format(i)))
    # i = i +1

# output = [predict(img, model, preprocess, postprocess, device) for img in images_res]
# utils.save_image(y2data, Path("../result/gen_image_{}_{}.png".format(i, _)))
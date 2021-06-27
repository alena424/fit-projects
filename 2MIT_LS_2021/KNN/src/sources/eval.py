from pathlib import Path

import cv2
from catalyst.metrics import iou, accuracy
from sklearn.model_selection import train_test_split
from torch.utils.data import DataLoader
import os
import torch
from torchvision.utils import save_image
from tqdm import tqdm, gui

from sources.couch import get_imgs_difference_iou, get_imgs_difference_structural_similarity
from sources.datasets import MoveAxis, SegmentationDataSet

#model_path = '../unet/model/unet2.pkl'
root = 'tmp-data-512x512-from-1-pictures'

preprocessor = MoveAxis(True, True)

# input and target files
inputs = list(map(lambda x: os.path.join(os.path.join(root, 'origins'), x), os.listdir(os.path.join(root, 'origins'))))
targets = list(map(lambda x: os.path.join(os.path.join(root, 'classes'), x), os.listdir(os.path.join(root, 'classes'))))


def predict(img, model):
    model.eval()
    x = img
    with torch.no_grad():
        out = model(x)  # send through model/network
        print(f'out = shape: {out.shape}; outclass: {out.unique()}; outtype: {out.dtype}')

    out_softmax = torch.softmax(out, dim=1)  # perform softmax on outputs
    result = out_softmax
    return result


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

print("evaluation starts...")
batch_iter = tqdm(enumerate(dataloader_training), 'Evaluation', total=len(dataloader_training), leave=False)
model = torch.load(Path(model_path), map_location='cpu')

scoresIoU = []
i = 0
for _, (inputs, targets) in batch_iter:
    output = predict(inputs, model)
    #print(inputs, targets)
    print("ted")
    #print(inputs.size())
    #print(targets.size())
    print(f'xinputs = shape: {inputs.shape}; inputstype: {inputs.dtype}')
    print(f'inputsx = min: {inputs.min()}; max: {inputs.max()}')
    print(f'targetsy = shape: {targets.shape}; class: {targets.unique()}; type: {targets.dtype}')
    print(f'outputy = shape: {output.shape}; class: {output.unique()}; type: {output.dtype}')
    scoreIoU = iou(outputs=output, targets=targets, mode="micro")
    save_image(output[0], 'results/output1' + str(i) + ".png")
    save_image(output[1], 'results/output2' + str(i) + ".png")

    save_image(targets[0].float(), 'results/output1' + str(i) + "_target.png")
    save_image(targets[1].float(), 'results/output2' + str(i) + "_target.png")
    i = i+1
    scoresIoU.append(scoreIoU)
    # IoU metric
    print("Score IuO is: ", scoreIoU)
    # save image from tensor


for i, img in enumerate(os.listdir("results")):
    if "_target.png" in img:
        generated = img.split("_target.png")[0]
        cv_gen = cv2.imread(os.path.join("results", generated + ".png"))
        cv_tar = cv2.imread(os.path.join("results", img))
        print("Score get_imgs_difference_iou: {}".format(get_imgs_difference_iou(cv_gen, cv_tar)))
        score, ref, res, diff, thresh = get_imgs_difference_structural_similarity(cv_gen, cv_tar)
        print("Score get_imgs_difference_structural_similarity: {}".format(score))

print("Statistics")
#print("Average score IuO is: ", sum(scoresIoU) / len(scoresIoU))

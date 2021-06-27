import torch

from datasets import SegmentationDataSet, MoveAxis
from dualPathModel import DualPathModel
from sklearn.model_selection import train_test_split
from torch.utils.data import DataLoader
from trainer import Trainer

import os

# root directory, setup by you
root = 'tmp-data-512x512-from-20-pictures'

preprocessor = MoveAxis(True, True)

# input and target files
inputs = list(map(lambda x: os.path.join(os.path.join(root, 'origins'), x), os.listdir(os.path.join(root, 'origins'))))
targets = list(map(lambda x: os.path.join(os.path.join(root, 'classes'), x), os.listdir(os.path.join(root, 'classes'))))

# random seed
random_seed = 42


class Runner:
    def __init__(self):
        self.dataloader_training = None
        self.dataloader_validation = None
        self.trainer = None
        self.tr_losses, self.vl_losses, self.lr_rates = (None, None, None)

    def init_dataloaders(self, train_size=0.8):
        inputs_train, inputs_valid, targets_train, targets_valid = train_test_split(
            inputs, targets,
            random_state=random_seed,
            train_size=train_size,
            shuffle=True)
        # dataset training
        dataset_train = SegmentationDataSet(inputs=inputs_train, targets=targets_train, transform=preprocessor)
        # dataset validation
        dataset_valid = SegmentationDataSet(inputs=inputs_valid, targets=targets_valid, transform=preprocessor)
        # dataloader training
        self.dataloader_training = DataLoader(dataset=dataset_train, batch_size=2, shuffle=True)
        # dataloader validation
        self.dataloader_validation = DataLoader(dataset=dataset_valid, batch_size=2, shuffle=True)

    def set_trainer(self, model, device, optimizer, path_criterion, edge_criterion, epochs=10):
        self.trainer = Trainer(model=model,
                               device=device,
                               criterion_spacial=path_criterion,
                               criterion_edge=edge_criterion,
                               optimizer=optimizer,
                               training_DataLoader=self.dataloader_training,
                               validation_DataLoader=self.dataloader_validation,
                               epochs=epochs,
                               epoch=0)

    def train(self):
        self.tr_losses, self.vl_losses, self.lr_rates = self.trainer.run_trainer()


if __name__ == "__main__":
    # device
    if torch.cuda.is_available():
        device = torch.device('cuda')
    else:
        device = torch.device('cpu')
    # Model setup with MSELoss and 512x512 resolution
    model = DualPathModel(res_x=512, res_y=512).to(device)
    criterion = torch.nn.MSELoss()
    optimizer = torch.optim.SGD(model.parameters(), lr=0.01)
    runner = Runner()
    runner.init_dataloaders()
    runner.set_trainer(model, device, optimizer, criterion, criterion)
    runner.train()

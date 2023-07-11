import torch
import os
import matplotlib.pyplot as plt
import numpy as np
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import torchvision.transforms as transforms
from src import hw3utils
torch.multiprocessing.set_start_method('spawn', force=True)



DEVICE_ID = 'cpu' # set to 'cpu' for cpu, 'cuda' / 'cuda:0' or similar for gpu.
LOG_DIR = 'checkpoints'
VISUALIZE = False# set True to visualize input, prediction and the output from the last batch
LOAD_CHKPT = False

def get_test_loader(device):
    # src_dir = os.path.relpath('src')
    # run ls command
    test_set = hw3utils.HW3ImageFolder(root="test_dataset",device=device)
    test_loader = torch.utils.data.DataLoader(test_set, batch_size=1, shuffle=False, num_workers=0)
    return test_loader



class Net(nn.Module):
    def __init__(self, layer_count, number_of_kernels):
        super(Net, self).__init__()
        self.layers = nn.ModuleList()
        # add layers here
        layer = nn.Conv2d(1, 16, 3, padding=1)
        layer.add_module('relu', nn.ReLU())
        self.layers.append(layer)
        for i in range(layer_count - 2):
            layer = nn.Conv2d(16, 16, 3, padding=1)
            layer.add_module('relu', nn.ReLU())
            self.layers.append(layer)
        layer = nn.Conv2d(16, 3, 3, padding=1)
        self.layers.append(layer)

    def forward(self, grayscale_image):
        # apply the layers in order
        x = self.layers[0](grayscale_image)
        for i in range(1, len(self.layers)):

            x = self.layers[i](x)

        x = torch.tanh(x)
        return x

device = torch.device(DEVICE_ID)
test_loader = get_test_loader(device)

model = Net(layer_count=4, number_of_kernels=4)
model.load_state_dict(torch.load('model.pt'))

random_indexes = np.random.choice(np.arange(2000), 100, replace=False)

estimations = []

# create a file to write the results
myfile = open('test_images.txt', 'w')
myfile.close()

for i in random_indexes:
    inputs, targets = test_loader.dataset[i]
    preds = model(inputs)

    # preds = preds.detach().cpu().numpy()
    # shift range from [-1,1] to [0,255]
    preds = (preds + 1) * 127.5

    # convert (3,80,80) to (80,80,3)
    transposed = preds.permute(1, 2, 0)
    # cast to uint8
    transposed = transposed.type(torch.uint8)

    # append to estimations
    estimations.append(transposed.numpy())

estimations = np.array(estimations)

np.save('estimations_test.npy', estimations)








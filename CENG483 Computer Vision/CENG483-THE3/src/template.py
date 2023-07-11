# Feel free to change / extend / adapt this source code as needed to complete the homework, based on its requirements.
# This code is given as a starting point.
#
# REFEFERENCES
# The code is partly adapted from pytorch tutorials, including https://pytorch.org/tutorials/beginner/blitz/cifar10_tutorial.html

# ---- hyper-parameters ----
# You should tune these hyper-parameters using:
# (i) your reasoning and observations, 
# (ii) by tuning it on the validation set, using the techniques discussed in class.
# You definitely can add more hyper-parameters here.

# TODO: validate
# TODO: automatically adjust epoch number
# TODO: plot loss
# TODO: hp stats

batch_size = 16
max_num_epoch = 100
hps = [
    {
        'number_of_kernels': 4,
        'layer_count': 2,
        'lr': 0.0001,
    },
    {
        'number_of_kernels': 4,
        'layer_count': 2,
        'lr': 0.001,
    },
    {
        'number_of_kernels': 4,
        'layer_count': 2,
        'lr': 0.1,
    },
    {
        'number_of_kernels': 4,
        'layer_count': 1,
        'lr': 0.001,
    },
    {
        'number_of_kernels': 4,
        'layer_count': 4,
        'lr': 0.001,
    },
    {
        'number_of_kernels': 2,
        'layer_count': 2,
        'lr': 0.001,
    },
    {
        'number_of_kernels': 8,
        'layer_count': 2,
        'lr': 0.001,
    },
]

# ---- options ----
DEVICE_ID = 'cuda:0' # set to 'cpu' for cpu, 'cuda' / 'cuda:0' or similar for gpu.
LOG_DIR = 'checkpoints'
VISUALIZE = False # set True to visualize input, prediction and the output from the last batch
LOAD_CHKPT = False

# --- imports ---
import torch
import os
import matplotlib.pyplot as plt
import numpy as np
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import torchvision.transforms as transforms
import hw3utils
import threading
torch.multiprocessing.set_start_method('spawn', force=True)

batch_size = 16
max_num_epoch = 100

# ---- utility functions -----
def get_loaders(batch_size,device):
    data_root = 'ceng483-f22-hw3-dataset' 
    train_set = hw3utils.HW3ImageFolder(root=os.path.join(data_root,'train'),device=device)
    train_loader = torch.utils.data.DataLoader(train_set, batch_size=batch_size, shuffle=True, num_workers=0)
    val_set = hw3utils.HW3ImageFolder(root=os.path.join(data_root,'val'),device=device)
    val_loader = torch.utils.data.DataLoader(val_set, batch_size=batch_size, shuffle=False, num_workers=0)
    # Note: you may later add test_loader to here.
    return train_loader, val_loader

# ---- ConvNet -----
class Net(nn.Module):
    def __init__(self, layer_count, number_of_kernels, group_count):
        super(Net, self).__init__()
        self.layers = nn.ModuleList()
        # add layers here
        if layer_count == 1:
            # self.layers.append(nn.Conv2d(1, 3, 3, padding=1))
            layer = nn.Conv2d(1, 3, 3, padding=1, groups=1)
            # layer.add_module('batch_norm', nn.BatchNorm2d(3))
            layer.add_module('relu', nn.ReLU())
            self.layers.append(layer)
        else:
            layer = nn.Conv2d(1, 16, 3, padding=1, groups=1)
            layer.add_module('relu', nn.ReLU())
            # layer.add_module('batch_norm', nn.BatchNorm2d(number_of_kernels))
            self.layers.append(layer)
            for i in range(layer_count - 2):
                layer = nn.Conv2d(16, 16, 3, padding=1, groups=group_count)
                layer.add_module('relu', nn.ReLU())
                # layer.add_module('batch_norm', nn.BatchNorm2d(number_of_kernels))
                self.layers.append(layer)
            layer = nn.Conv2d(16, 3, 3, padding=1, groups=1)
            # layer.add_module('batch_norm', nn.BatchNorm2d(number_of_kernels))
            self.layers.append(layer)

    def forward(self, grayscale_image):
        # apply the layers in order
        # apply max pooling after each layer

        x = self.layers[0](grayscale_image)
        for i in range(1, len(self.layers)):
            # max pooling
            # x = torch.nn.MaxPool2d(2)
            # x = F.relu(x)
            x = self.layers[i](x)

        x = torch.tanh(x)

        return x

def thread_main(hp, hp_index):
    # ---- setup ----

    prev_validation_loss = 0
    patience = 0
    last_epoch = 0
    print(f'hp {hp_index} started')

    loss_list = []

    device = torch.device(DEVICE_ID)
    train_loader, val_loader = get_loaders(batch_size,device)
    net = Net(hp['layer_count'], hp['number_of_kernels'], group_count=2).to(device)
    criterion = nn.MSELoss()
    optimizer = optim.Adam(net.parameters(), lr=hp['lr'])
    # ---- training loop ----
    with open(os.path.join(LOG_DIR, f'group_{hp_index}.txt'), 'w') as f:

        for epoch in range(max_num_epoch):  # loop over the dataset multiple times
            running_loss = 0.0
            for iteri, data in enumerate(train_loader, 0):
                inputs, targets = data # inputs: low-resolution images, targets: high-resolution images.

                optimizer.zero_grad() # zero the parameter gradients

                # do forward, backward, SGD step
                preds = net(inputs.to(device))
                loss = criterion(preds, targets)
                loss.backward()
                optimizer.step()

                # print loss
                running_loss += loss.item()
                print_n = 100 # feel free to change this constant
                if iteri % print_n == (print_n-1):    # print every print_n mini-batches
                    # print('[%d, %5d] network-loss: %.3f' %
                    #       (epoch + 1, iteri + 1, running_loss / 100))
                    f.write(f'[{epoch + 1}, {iteri + 1}] network-loss: {running_loss / 100}\n')
                    running_loss = 0.0
                    # note: you most probably want to track the progress on the validation set as well (needs to be implemented)

                if (iteri==0) and VISUALIZE:
                    hw3utils.visualize_batch(inputs,preds,targets)


            # torch.save(net.state_dict(), os.path.join(LOG_DIR,'checkpoint.pt'))
            # hw3utils.visualize_batch(inputs,preds,targets,os.path.join(LOG_DIR,'example.png'))

            if epoch % 5 == 0:
                with torch.no_grad():
                    # validate
                    print(f'validating {epoch}')
                    validation_loss = 0.0
                    for iteri, data in enumerate(val_loader, 0):
                        inputs, targets = data
                        preds = net(inputs)
                        loss = criterion(preds, targets)
                        validation_loss += loss.item()

                    validation_loss /= len(val_loader)

                    f.write(f'validation-loss: {validation_loss}\n')

                    loss_list.append(validation_loss)

                    if abs(validation_loss - prev_validation_loss) < 0.0001 or validation_loss > prev_validation_loss:
                        patience += 1
                        if validation_loss > prev_validation_loss:
                            patience += 1
                        if patience >= 5:
                            break
                    else:
                        patience = 0

                    prev_validation_loss = validation_loss

        hp_mean = np.mean(loss_list)
        hp_std = np.std(loss_list)
        # calculate confidence interval
        hp_ci = 1.96 * hp_std / np.sqrt(len(loss_list))
        # print mean and confidence interval
        f.write('loss: %.3f +- %.3f' % (hp_mean, hp_ci))

    print(f'hp {hp_index} finished')


if not os.path.exists(LOG_DIR):
    os.makedirs(LOG_DIR)
# if LOAD_CHKPT:
#     print('loading the model from the checkpoint')
#     model.load_state_dict(os.path.join(LOG_DIR,'checkpoint.pt'))

threads = []
#
hp_num = 1
for hp in hps:
    hp_thread = threading.Thread(target=thread_main, args=(hp, hp_num))
    threads.append(hp_thread)
    hp_thread.start()
    hp_num += 1

for hp_thread in threads:
    hp_thread.join()






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

DEVICE_ID = 'cuda:0' # set to 'cpu' for cpu, 'cuda' / 'cuda:0' or similar for gpu.
LOG_DIR = 'checkpoints'
VISUALIZE = False# set True to visualize input, prediction and the output from the last batch
LOAD_CHKPT = False

batch_size = 16
max_num_epoch = 100


def get_loaders(batch_size,device):
    data_root = '../ceng483-f22-hw3-dataset'
    train_set = hw3utils.HW3ImageFolder(root=os.path.join(data_root,'train'),device=device)
    train_loader = torch.utils.data.DataLoader(train_set, batch_size=batch_size, shuffle=True, num_workers=0)
    val_set = hw3utils.HW3ImageFolder(root=os.path.join(data_root,'val'),device=device)
    val_loader = torch.utils.data.DataLoader(val_set, batch_size=batch_size, shuffle=False, num_workers=0)
    # Note: you may later add test_loader to here.
    return train_loader, val_loader

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

prev_validation_loss = 0
patience = 0
last_epoch = 0

loss_list = []

class TwelveMarginLoss(nn.Module):
    def __init__(self):
        super(TwelveMarginLoss, self).__init__()
        self.margin = 0.094

    def forward(self, input, target):
        return torch.mean(torch.clamp(torch.abs(input - target) - self.margin, min=0))

device = torch.device(DEVICE_ID)
train_loader, val_loader = get_loaders(batch_size,device)
net = Net(layer_count=4, number_of_kernels=4).to(device)
mse_criterion = nn.MSELoss()
margin_criterion = TwelveMarginLoss()
optimizer = optim.Adam(net.parameters(), lr=0.001)

train_losses = []
val_losses = []

best_val_loss = float('inf')
patience = 5
patience_counter = 0

if not os.path.exists(LOG_DIR):
    os.makedirs(LOG_DIR)

# ---- training loop ----
with open(os.path.join(LOG_DIR, f'results.txt'), 'w') as f:

    for epoch in range(max_num_epoch):  # loop over the dataset multiple times
        epoch_train_losses = []
        running_loss = 0.0
        for iteri, data in enumerate(train_loader, 0):
            inputs, targets = data # inputs: low-resolution images, targets: high-resolution images.

            optimizer.zero_grad() # zero the parameter gradients

            # do forward, backward, SGD step
            preds = net(inputs.to(device))
            loss = mse_criterion(preds, targets)
            epoch_train_losses.append(loss.item())

            # loss = margin_criterion(preds, target=targets)
            # loss.backward()
            loss = margin_criterion(preds, targets)
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



        train_losses.append(np.mean(epoch_train_losses))

        # hw3utils.visualize_batch(inputs,preds,targets,os.path.join(LOG_DIR,'example.png'))

        with torch.no_grad():
            # validate

            validation_loss = 0.0
            prev_validation_loss = 0.0
            for iteri, data in enumerate(val_loader, 0):
                inputs, targets = data
                preds = net(inputs)
                loss = margin_criterion(preds, targets).item()
                validation_loss += loss
                # validation_loss += margin_criterion(preds,  target=targets)

            validation_loss /= len(val_loader)

            f.write(f'{epoch}: validation-loss: {validation_loss}\n')
            print(f'validating {epoch} : {validation_loss} {patience_counter}')

            val_losses.append(validation_loss)

            if validation_loss < best_val_loss:
                best_val_loss = validation_loss
                patience_counter = 0
            else:
                patience_counter += 1

            if patience_counter >= patience:
                for iteri, data in enumerate(val_loader, 0):
                    inputs, targets = data
                    preds = net(inputs)
                    hw3utils.visualize_batch(inputs, preds, targets, os.path.join(LOG_DIR, f'{iteri}.png'))
                    loss = margin_criterion(preds, targets).item()
                    print(f'{iteri} - loss: {loss}')
                    if iteri == 5:
                        break
                break

    f.write('finished at epoch: ' + str(epoch))

    src_dir = os.path.join(os.path.dirname(os.getcwd()))

    # save the model
    torch.save(net.state_dict(), os.path.join(src_dir, 'model.pt'))

    fig, ax = plt.subplots()
    ax.plot(np.arange(1, len(train_losses)+1), train_losses, color='blue')
    ax.set(xlabel='Epoch', ylabel='Loss', title='Mean Squared Error Loss Over Epochs')
    fig.savefig(os.path.join(LOG_DIR, 'training_losses.png'))
    fig, ax = plt.subplots()
    ax.plot(np.arange(1, len(val_losses) + 1), val_losses, color='red', label='Validation 12 Margin Error')
    ax.set(xlabel='Epoch', ylabel='Loss', title='12 Margin Error Over Epochs')
    fig.savefig(os.path.join(LOG_DIR, 'validation_losses.png'))



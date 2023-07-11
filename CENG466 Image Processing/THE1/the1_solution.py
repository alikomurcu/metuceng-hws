# CENG466-THE1
# Ali Komurcu 2380699
# Muhammed Haseeb Motan 2398071

import os
import cv2 as cv
import numpy as np
import math
import matplotlib.pyplot as plt
from skimage.transform import rescale, resize, rotate
from skimage.util import img_as_float, img_as_ubyte
from skimage import exposure

INPUT_PATH = "./THE1_Images/"
OUTPUT_PATH = "./Outputs/"


def read_image(img_path, rgb=True):
    img = cv.imread(img_path, int(rgb))
    return img

def write_image(img, output_path, rgb=True):
    cv.imwrite(output_path, img)


def extract_save_histogram(img, path):
    plt.hist(img.flatten(), bins=256, range=[0, 256], color='c')
    plt.title("histogram")
    plt.savefig(path)
    plt.close()


def rotate_image(img, degree=0, interpolation_type="linear"):
    image = img_as_float(img)

    if interpolation_type == "linear":
        return img_as_ubyte(rotate(image, degree, resize=True, order=1))
    elif interpolation_type == "cubic":
        return img_as_ubyte(rotate(image, degree, resize=True, order=3))

def rotate_image_our_version(img, degree=0, interpolation_type="linear"):   # Just handles the linear interpolation
    # interpolation type: "linear" or "cubic"
    # degree: 45 or 90
    degreeRad = math.radians(degree)
    affine_matrix = np.array([[math.cos(degreeRad), -math.sin(degreeRad), 0],
                             [math.sin(degreeRad), math.cos(degreeRad), 0],
                             [0, 0, 1]])
    rows = img.shape[0]         # of columns
    columns = img.shape[1]      # of rows

    newshape = (int(rows * 2**0.5) + 1, int(columns * 2**0.5) + 1, img.shape[2])    # Enlarge the image bounding square in a circular way
    #newshape = (rows * 3, columns * 3, img.shape[2])    # Enlarge the image bounding square in a circular way
    # + 1 is for rounding up
    #TODO: newshape
    #newshape = (2048, 2048, 3)
    newArr = np.zeros(newshape[0] * newshape[1] * newshape[2]).astype(int)
    newArr.fill(-1)
    rotatedImage = newArr.reshape(newshape)

    mask = np.zeros(newshape[0] * newshape[1]).astype(bool)
    mask_RotatedImage = mask.reshape((newshape[0], newshape[1]))
    mask_RotatedImage.fill(False)

    for i in range(rows):
        for j in range(columns):
            point = np.array([[i-rows//2],
                             [j-columns//2],
                             [1]])   # This is the current point to be transformed as a 3x1 vector
            affineCoordinate = np.matmul(affine_matrix, point).astype(int)

            afX = (affineCoordinate[0][0] + newshape[0] // 2)
            afY = (affineCoordinate[1][0] + newshape[1] // 2)

            rotatedImage[afX][afY] = img[i][j]
            mask_RotatedImage[afX][afY] = True      # mask the cell to indicate that the pixel is captured after rotation

    if interpolation_type == "linear":
        r = mask_RotatedImage.shape[0]
        c = mask_RotatedImage.shape[1]
        for i in range(r):      #rows
            for j in range(c):      #columns
                x = i - r//2
                y = j - c//2
                if mask_RotatedImage[x][y] == False:
                    # Start to interpolate
                    # Boundary check
                    upBorder = downBorder = leftBorder = rightBorder = False
                    if x % newshape[0] == 0:
                        upBorder = True
                    elif x % newshape[0] == newshape[0] - 1:
                        downBorder = True
                    if y % newshape[1] == 0:
                        leftBorder = True
                    elif y % newshape[1] == newshape[1] - 1:
                        rightBorder = True
                    # if( or ):
                    count = 0
                    up = down = 0
                    left = right = 0
                    if not upBorder:
                        up = rotatedImage[x-1][y]
                        count += 1
                    elif not downBorder:
                        down = rotatedImage[x+1][y]
                        count += 1
                    if not leftBorder:
                        left = rotatedImage[x][y - 1]
                        count += 1
                    elif not rightBorder:
                        right = rotatedImage[x][y + 1]
                        count += 1
                    # rotatedImage[x][y] = (up + down + left + right) // count
                    rotatedImage[x][y] = (rotatedImage[x-1][y] + rotatedImage[x+1][y] + rotatedImage[x][y-1] + rotatedImage[x][y+1])//4

    return rotatedImage
def histogram_equalization(img):
    row, column = img.shape
    hist, bins = np.histogram(img.flatten(), 256, [0, 256])

    # convert the histogram into a pdf
    nxm = hist.cumsum().max()
    hist = hist / float(nxm)

    # generate the cumulative pdf that sums to 1
    cdf = hist.cumsum()

    # multiply cdf with 255 to get gray-level
    cdf = (cdf * 255).astype(int)

    img = img.flatten()
    # replace original gray-levels with new ones
    # for i in range(nxm):
    #     img[i] = cdf[img[i]]

    # same as the commented code above
    img = cdf[img]

    return img.reshape(row, column)


def adaptive_histogram_equalization(img):
    adaptive = cv.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))
    cl1 = adaptive.apply(img)
    return cl1


if __name__ == '__main__':
    if not os.path.exists(OUTPUT_PATH):
        os.makedirs(OUTPUT_PATH)
    # PART1
    img = read_image(INPUT_PATH + "a1.png")
    output = rotate_image(img, 45, "linear")
    write_image(output, OUTPUT_PATH + "a1_45_linear.png")

    img = read_image(INPUT_PATH + "a1.png")
    output = rotate_image(img, 45, "cubic")
    write_image(output, OUTPUT_PATH + "a1_45_cubic.png")

    img = read_image(INPUT_PATH + "a1.png")
    output = rotate_image(img, 90, "linear")
    write_image(output, OUTPUT_PATH + "a1_90_linear.png")

    img = read_image(INPUT_PATH + "a1.png")
    output = rotate_image(img, 90, "cubic")
    write_image(output, OUTPUT_PATH + "a1_90_cubic.png")

    img = read_image(INPUT_PATH + "a2.png")
    output = rotate_image(img, 45, "linear")
    write_image(output, OUTPUT_PATH + "a2_45_linear.png")

    img = read_image(INPUT_PATH + "a2.png")
    output = rotate_image(img, 45, "cubic")
    write_image(output, OUTPUT_PATH + "a2_45_cubic.png")

    # # PART2
    img = read_image(INPUT_PATH + "b1.png", rgb=False)
    extract_save_histogram(img, OUTPUT_PATH + "original_histogram.png")
    equalized = histogram_equalization(img)
    extract_save_histogram(equalized, OUTPUT_PATH + "equalized_histogram.png")
    write_image(equalized, OUTPUT_PATH + "enhanced_image.png")

    # BONUS
    # Define the following function
    equalized = adaptive_histogram_equalization(img)
    extract_save_histogram(equalized, OUTPUT_PATH + "adaptive_equalized_histogram.png")
    write_image(equalized, OUTPUT_PATH + "adaptive_enhanced_image.png")
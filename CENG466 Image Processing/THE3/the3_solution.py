# Ali Komurcu 2380699
# Muhammad Haseeb Motan 2398071
import os
import cv2 as cv
from sklearn.cluster import KMeans
import numpy as np
import math
from skimage import img_as_ubyte

INPUT_PATH = "./THE3_Images/"
OUTPUT_PATH = "./Outputs/"
count = 0
imgstr = "1"
def read_image(img_path, rgb=True):
    img = cv.imread(img_path, int(rgb))
    return img

def write_image(img, output_path, rgb=True):
    cv.imwrite(output_path, img)

def question1():

    img1 = read_image(INPUT_PATH + "1_source.png")
    img2 = read_image(INPUT_PATH + "2_source.png")
    img3 = read_image(INPUT_PATH + "3_source.png")

    write_image(detect_faces(img1, 1), OUTPUT_PATH + "1_faces.png")
    write_image(detect_faces(img2, 2), OUTPUT_PATH + "2_faces.png")
    write_image(detect_faces(img3, 3), OUTPUT_PATH + "3_faces.png")
    return
def detect_faces(image, num):
    # Convert the image to HSV color space
    hsv = cv.cvtColor(image, cv.COLOR_BGR2HSV)

    # Split the HSV channels
    h, s, v = cv.split(hsv)

    # For each image:
    if num == 1:
        lower_h, upper_h = 0, 20
        lower_s, upper_s  = 50, 255
        lower_v, upper_v = 242, 255
        numberOfSquares = 3
    if num == 2:
        lower_h, upper_h = 0, 15
        lower_s, upper_s  = 75, 120
        lower_v, upper_v = 210, 255
        numberOfSquares = 5
    if num == 3:
        lower_h, upper_h = 0, 255
        lower_s, upper_s  = 75, 255
        lower_v, upper_v = 160, 255
        numberOfSquares = 1

    # Create the mask image using comparison operators
    mask = np.logical_and(lower_h <= h, h <= upper_h)
    mask = np.logical_and(mask, lower_s <= s)
    mask = np.logical_and(mask, s <= upper_s)
    mask = np.logical_and(mask, lower_v <= v)
    mask = np.logical_and(mask, v <= upper_v)

    # Convert the mask to an unsigned 8-bit integer type
    mask = mask.astype(np.uint8)

    # Apply the mask using the bitwise_and operator
    masked_img = cv.bitwise_and(image, image, mask=mask)
    if num == 1:
        kernel1 = np.ones((4, 4), np.uint8)
        kernel2 = np.ones((4, 4), np.uint8)
        masked_img = cv.erode(masked_img, kernel1, iterations=2)
        masked_img = cv.dilate(masked_img, kernel2, iterations=10)
        border = 2
    elif num == 2:
        masked_img = cv.dilate(masked_img, None, iterations=2)
        border = 5
    elif num == 3:
        kernel = np.ones((1,47), np.uint8)
        masked_img = cv.erode(masked_img, kernel, iterations=1)
        masked_img = cv.dilate(masked_img, None, iterations=35)
        border = 2
    # return masked_img

    # Find the contours in the mask image
    masked_img = cv.cvtColor(masked_img, cv.COLOR_HSV2BGR)
    masked_img = cv.cvtColor(masked_img, cv.COLOR_BGR2GRAY)
    contours, _ = cv.findContours(masked_img, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
    contours = sorted(contours, key=cv.contourArea, reverse=True)[:numberOfSquares]
    # Draw a square around each contour
    for c in contours:
        x, y, w, h = cv.boundingRect(c)
        cv.rectangle(image, (x, y), (x + w, y + h), (0, 0, 255), border)
    return image

def detect_edges():

    img_source1 = read_image(INPUT_PATH + "1_source.png")
    img_source2 = read_image(INPUT_PATH + "2_source.png")
    img_source3 = read_image(INPUT_PATH + "3_source.png")
    img_source4 = read_image(INPUT_PATH + "4_source.png")
    
    i = 1
    for image in [img_source1, img_source2, img_source3, img_source4]:
        img_grayscale = cv.cvtColor(image, cv.COLOR_BGR2GRAY)
        img_edges = cv.Canny(img_grayscale, 30, 100)
        write_image(img_edges, OUTPUT_PATH + f"{i}_colored_edges.png")
        i += 1

    return

def question2():
    img1 = read_image(INPUT_PATH + "1.png", False)
    img_source1 = read_image(INPUT_PATH + "1_source.png")
    img2 = read_image(INPUT_PATH + "2.png", False)
    img_source2 = read_image(INPUT_PATH + "2_source.png")
    img3 = read_image(INPUT_PATH + "3.png", False)
    img_source3 = read_image(INPUT_PATH + "3_source.png")
    img4 = read_image(INPUT_PATH + "4.png", False)
    img_source4 = read_image(INPUT_PATH + "4_source.png")

    img1_colored = color_images(img1, img_source1, 256)
    img2_colored = color_images(img2, img_source2, 256)
    img3_colored = color_images(img3, img_source3, 256)
    img4_colored = color_images(img4, img_source4, 5)

    write_image(img1_colored, OUTPUT_PATH + "1_colored.png")
    write_image(img2_colored, OUTPUT_PATH + "2_colored.png")
    write_image(img3_colored, OUTPUT_PATH + "3_colored.png")
    write_image(img4_colored, OUTPUT_PATH + "4_colored.png")

    i = 1
    for image in [img1_colored, img2_colored, img3_colored, img4_colored]:
        b, g, r = cv.split(image)

        h, s, v = cv.split(cv.cvtColor(image, cv.COLOR_BGR2HSV))

        write_image(b, OUTPUT_PATH + f"{i}_b.png")
        write_image(g, OUTPUT_PATH + f"{i}_g.png")
        write_image(r, OUTPUT_PATH + f"{i}_r.png")
        write_image(h, OUTPUT_PATH + f"{i}_h.png")
        write_image(s, OUTPUT_PATH + f"{i}_s.png")
        write_image(h, OUTPUT_PATH + f"{i}_v.png")

        i = i + 1

    detect_edges()

    return

def color_images(img_grayscale, img_source, num_of_colors):

    img_color = np.zeros((img_grayscale.shape[0], img_grayscale.shape[1], 3), dtype=np.uint8)

    img_source = cv.cvtColor(img_source, cv.COLOR_BGR2HSV)

    img_source = np.reshape(img_source, (img_source.shape[0] * img_source.shape[1], 3))
    
    clusterer = KMeans(n_clusters=num_of_colors, n_init=1, max_iter=1)
    clusterer.fit(img_source)
    colors = clusterer.cluster_centers_

    colors = np.ndarray.tolist(colors)

    list.sort(colors, key=lambda x: x[2])
    
    for i in range(img_color.shape[0]):
        for j in range(img_color.shape[1]):
            img_color[i][j] = colors[img_grayscale[i][j] % num_of_colors]

    # Haseeb try below method, i think it will be same and more efficient than above
    # img_color = colors[img_grayscale % num_of_colors]

    return cv.cvtColor(img_color, cv.COLOR_HSV2BGR)

if __name__ == '__main__':    

    if not os.path.exists(OUTPUT_PATH):
        os.makedirs(OUTPUT_PATH)

    # Face Detection
    # question1()

    # Pseduo coloring
    question2()



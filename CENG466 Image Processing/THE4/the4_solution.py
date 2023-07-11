# Ali Komurcu 2380699
# Muhammad Haseeb Motan 2398071
import os
import cv2 as cv
from sklearn.cluster import KMeans
import numpy as np
from skimage import img_as_ubyte
from sklearn.cluster import MeanShift, estimate_bandwidth
from skimage import data, segmentation, color
from skimage.future import graph
from sklearn.tree import DecisionTreeClassifier, plot_tree
from skimage.transform import rescale
import matplotlib.pyplot as plt

INPUT_PATH = "./THE4_Images/"
OUTPUT_PATH = "./Outputs/"


def read_image(img_path, rgb=True):
    img = cv.imread(img_path, int(rgb))
    return img


def write_image(img, output_path, rgb=True):
    cv.imwrite(output_path, img)


def question1():
    img1 = read_image(INPUT_PATH + "A1.png")
    img2 = read_image(INPUT_PATH + "A2.png")
    img3 = read_image(INPUT_PATH + "A3.png")
    write_image(count_obejcts(img1, 1), OUTPUT_PATH + "A1_output.png")
    write_image(count_obejcts(img2, 2), OUTPUT_PATH + "A2_output.png")
    write_image(count_obejcts(img3, 3), OUTPUT_PATH + "A3_output.png")
    return


def count_obejcts(img, num):
    # Convert the image to grayscale
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    # Blur the image slightly
    gray = cv.GaussianBlur(gray, (5, 5), 0)
    if num == 1:
        hsv = cv.cvtColor(img, cv.COLOR_BGR2HSV)
        h, s, v = cv.split(hsv)

        lower_h, upper_h = 128, 255
        lower_s, upper_s  = 75, 255
        lower_v, upper_v = 160, 255

        # Create the mask image using comparison operators
        mask = np.logical_and(lower_h <= h, h <= upper_h)
        mask = np.logical_and(mask, lower_s <= s)
        mask = np.logical_and(mask, s <= upper_s)
        mask = np.logical_and(mask, lower_v <= v)
        mask = np.logical_and(mask, v <= upper_v)
        mask = mask.astype(np.uint8)
        masked_img = cv.bitwise_and(img, img, mask=mask)
        gray1 = cv.cvtColor(masked_img, cv.COLOR_BGR2GRAY)

        # morphological close operation
        kernel = np.ones((5, 5), np.uint8)
        # opening
        closing = cv.morphologyEx(gray1, cv.MORPH_CLOSE, kernel, iterations=8)
        opening = cv.morphologyEx(closing, cv.MORPH_OPEN, kernel, iterations=8)
        # binarize the image
        ret, thresh = cv.threshold(opening, 128, 255, cv.THRESH_BINARY_INV | cv.THRESH_OTSU)
        # invert the image
        thresh = cv.bitwise_not(thresh)


    elif num == 2:
        hsv = cv.cvtColor(img, cv.COLOR_BGR2HSV)
        h, s, v = cv.split(hsv)

        lower_h, upper_h = 128, 255
        lower_s, upper_s  = 75, 255
        lower_v, upper_v = 160, 255

        # Create the mask image using comparison operators
        mask = np.logical_and(lower_h <= h, h <= upper_h)
        mask = np.logical_and(mask, lower_s <= s)
        mask = np.logical_and(mask, s <= upper_s)
        mask = np.logical_and(mask, lower_v <= v)
        mask = np.logical_and(mask, v <= upper_v)
        mask = mask.astype(np.uint8)
        masked_img = cv.bitwise_and(img, img, mask=mask)
        gray2 = cv.cvtColor(masked_img, cv.COLOR_BGR2GRAY)

        # morphological close operation
        kernel = np.ones((5, 5), np.uint8)
        # opening
        closing = cv.morphologyEx(gray2, cv.MORPH_CLOSE, kernel, iterations=24)
        opening = cv.morphologyEx(closing, cv.MORPH_OPEN, kernel, iterations=18)
        # binarize the image
        # return closing
        ret, thresh = cv.threshold(opening, 128, 255, cv.THRESH_BINARY_INV | cv.THRESH_OTSU)
        # invert the image
        thresh = cv.bitwise_not(thresh)


    elif num == 3:
        hsv = cv.cvtColor(img, cv.COLOR_BGR2HSV)
        h, s, v = cv.split(hsv)

        lower_h, upper_h = 128, 168
        lower_s, upper_s  = 0, 255
        lower_v, upper_v = 160, 255
        mask = np.logical_and(lower_h <= h, h <= upper_h)
        mask = np.logical_and(mask, lower_s <= s)
        mask = np.logical_and(mask, s <= upper_s)
        mask = np.logical_and(mask, lower_v <= v)
        mask = np.logical_and(mask, v <= upper_v)
        mask = mask.astype(np.uint8)
        masked_img = cv.bitwise_and(img, img, mask=mask)
        gray3 = cv.cvtColor(masked_img, cv.COLOR_BGR2GRAY)
        kernel = np.ones((5, 5), np.uint8)
        # opening
        opening = cv.morphologyEx(gray3, cv.MORPH_OPEN, kernel, iterations=60)
        closing = cv.morphologyEx(opening, cv.MORPH_CLOSE, kernel, iterations=50)
        # ret, thresh = cv.threshold(gray, 128, 255, cv.THRESH_BINARY_INV | cv.THRESH_OTSU)
        ret, thresh = cv.threshold(closing, 128, 255, cv.THRESH_BINARY_INV | cv.THRESH_OTSU)
        thresh = cv.bitwise_not(thresh)


    # find contours
    cnts = cv.findContours(thresh.copy(), cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
    cnts = cnts[0] if len(cnts) == 2 else cnts[1]
    print("The number of flowers in image A" + str(num) + " is " + str(len(cnts)))
    return thresh
    # draw contours
    for c in cnts:
        x, y, w, h = cv.boundingRect(c)
        cv.rectangle(img, (x, y), (x + w, y + h), (36, 255, 12), 5)

    return img


def question2():
    img1 = read_image(INPUT_PATH + "B1.jpg")
    
    # resizing to make the algorithm run faster
    # r = 50.0 / img1.shape[0]
    # dim = (int(img1.shape[1] * r), 50)
    # img1 = cv.resize(img1, dim, interpolation = cv.INTER_AREA)

    img2 = read_image(INPUT_PATH + "B2.jpg")

    # resizing to make the algorithm run faster
    # r = 50.0 / img2.shape[0]
    # dim = (int(img2.shape[1] * r), 50)
    # img2 = cv.resize(img2, dim, interpolation = cv.INTER_AREA)

    img3 = read_image(INPUT_PATH + "B3.jpg")
    img4 = read_image(INPUT_PATH + "B4.jpg")

    # resizing to make the algorithm run faster
    # r = 50.0 / img4.shape[0]
    # dim = (int(img4.shape[1] * r), 50)
    # img4 = cv.resize(img4, dim, interpolation = cv.INTER_AREA)

    image_segmentation(img1, output=OUTPUT_PATH + "B1_algorithm_meanshift_parameterset_1.png", mode=0, bandwidth=30)
    image_segmentation(img1, output=OUTPUT_PATH + "B1_algorithm_meanshift_parameterset_2.png", mode=0, bandwidth=40)
    image_segmentation(img1, output=OUTPUT_PATH + "B1_algorithm_meanshift_parameterset_3.png", mode=0, bandwidth=50)

    image_segmentation(img2, output=OUTPUT_PATH + "B2_algorithm_meanshift_parameterset_1.png", mode=0, bandwidth=30)
    image_segmentation(img2, output=OUTPUT_PATH + "B2_algorithm_meanshift_parameterset_2.png", mode=0, bandwidth=40)
    image_segmentation(img2, output=OUTPUT_PATH + "B2_algorithm_meanshift_parameterset_3.png", mode=0, bandwidth=50)

    image_segmentation(img3, output=OUTPUT_PATH + "B3_algorithm_meanshift_parameterset_1.png", mode=0, bandwidth=30)
    image_segmentation(img3, output=OUTPUT_PATH + "B3_algorithm_meanshift_parameterset_2.png", mode=0, bandwidth=40)
    image_segmentation(img3, output=OUTPUT_PATH + "B3_algorithm_meanshift_parameterset_3.png", mode=0, bandwidth=50)

    image_segmentation(img4, output=OUTPUT_PATH + "B4_algorithm_meanshift_parameterset_1.png", mode=0, bandwidth=30)
    image_segmentation(img4, output=OUTPUT_PATH + "B4_algorithm_meanshift_parameterset_2.png", mode=0, bandwidth=40)
    image_segmentation(img4, output=OUTPUT_PATH + "B4_algorithm_meanshift_parameterset_3.png", mode=0, bandwidth=50)

    image_segmentation(img1, output=OUTPUT_PATH + "B1_algorithm_ncut_parameterset_1.png", mode=1, thresh=0.01)
    image_segmentation(img1, output=OUTPUT_PATH + "B1_algorithm_ncut_parameterset_2.png", mode=1, thresh=0.05)
    image_segmentation(img1, output=OUTPUT_PATH + "B1_algorithm_ncut_parameterset_3.png", mode=1, thresh=0.1)

    image_segmentation(img2, output=OUTPUT_PATH + "B2_algorithm_ncut_parameterset_1.png", mode=1, thresh=0.01)
    image_segmentation(img2, output=OUTPUT_PATH + "B2_algorithm_ncut_parameterset_2.png", mode=1, thresh=0.05)
    image_segmentation(img2, output=OUTPUT_PATH + "B2_algorithm_ncut_parameterset_3.png", mode=1, thresh=0.1)

    image_segmentation(img3, output=OUTPUT_PATH + "B3_algorithm_ncut_parameterset_1.png", mode=1, thresh=0.01)
    image_segmentation(img3, output=OUTPUT_PATH + "B3_algorithm_ncut_parameterset_2.png", mode=1, thresh=0.05)
    image_segmentation(img3, output=OUTPUT_PATH + "B3_algorithm_ncut_parameterset_3.png", mode=1, thresh=0.1)

    image_segmentation(img4, output=OUTPUT_PATH + "B4_algorithm_ncut_parameterset_1.png", mode=1, thresh=0.01)
    image_segmentation(img4, output=OUTPUT_PATH + "B4_algorithm_ncut_parameterset_2.png", mode=1, thresh=0.05)
    image_segmentation(img4, output=OUTPUT_PATH + "B4_algorithm_ncut_parameterset_3.png", mode=1, thresh=0.1)


# mode is 0 for meanshift, 1 for n-cut
def image_segmentation(img, output, mode, bandwidth=50, thresh=0.01):

    img = img_as_ubyte(img)
    img_shape_x, img_shape_y = img.shape[0:2]
    segmentation_map = img.copy()
    boundary_overlay = img.copy()

    if (mode == 0):
        segmentation_map = np.reshape(segmentation_map, (img.shape[0] * img.shape[1], 3))  # represent as row*column,channel number

        ms = MeanShift(bandwidth=bandwidth, bin_seeding=True)
        seg_img = ms.fit_predict(segmentation_map)
        labels = ms.labels_
        cluster_centers = ms.cluster_centers_

        labels_unique = np.unique(labels)
        n_clusters_ = len(labels_unique)

        print("number of estimated clusters : %d" % n_clusters_)

        if (n_clusters_ > 1):
            g = graph.rag_mean_color(img, labels.reshape(img_shape_x, img_shape_y), mode='similarity')
                

        colors = [(255,255,255), (255,0,0), (0,255,0), (0,0,255), (255,255,0), (0,255,255), (255,0,255), (0,0,0)]   

        for k, col in zip(range(n_clusters_), colors):
            my_members = labels == k
            segmentation_map[my_members] = col

        # Tree Relationship Structure
        tree = DecisionTreeClassifier()
        tree.fit(segmentation_map, labels)

        segmentation_map = segmentation_map.reshape(img_shape_x, img_shape_y, 3)

        edges = cv.Canny(segmentation_map, 100, 200)

        for i in range(edges.shape[0]):
            for j in range(edges.shape[1]):
                if edges[i][j] == 255:
                    boundary_overlay[i][j] = [255, 255, 255]

        boundary_overlay = cv.cvtColor(boundary_overlay, cv.COLOR_BGR2RGB)

        fig, ax = plt.subplots(nrows=5, sharex=True, sharey=True, figsize=(24, 40))

        ax[0].set_title('Original image')
        ax[0].imshow(cv.cvtColor(img, cv.COLOR_BGR2RGB))
        ax[1].set_title('Segmentation map')
        ax[1].imshow(segmentation_map)
        ax[2].set_title('Boundary overlay')
        ax[2].imshow(boundary_overlay)
        ax[3].set_title('Tree Relationship Structure')
        plot_tree(tree, ax=ax[3])
        
        if (n_clusters_ > 1):
            ax[4].set_title('Region adjacency graph')
            graph.show_rag(labels.reshape(img_shape_x, img_shape_y), g, cv.cvtColor(img, cv.COLOR_BGR2RGB), ax=ax[4])

        for a in ax:
            a.axis('off')

        plt.savefig(output)
        plt.close()

    elif mode == 1:
        # compute segmentation with n-cut algorithm
        segments = segmentation.slic(img, compactness=30, n_segments=400)
        output1 = color.label2rgb(segments, img, kind='avg')

        g = graph.rag_mean_color(img, segments, mode='similarity')

        segments2 = graph.cut_normalized(segments, g, thresh=0.05)

        # choose 1 of below 2 outputs
        output2 = color.label2rgb(segments2, img, kind='avg')

        # Tree Relationship Structure
        tree = DecisionTreeClassifier()
        tree.fit(output2.reshape(output2.shape[0] * output2.shape[1], 3), segments2.reshape(output2.shape[0] * output2.shape[1], 1))

        # graph_cut_output = segmentation.mark_boundaries(out1, labels2, (0, 0, 0), mode='thick')

        edges = cv.Canny(output2, 100, 200)

        for i in range(edges.shape[0]):
            for j in range(edges.shape[1]):
                if edges[i][j] == 255:
                    boundary_overlay[i][j] = [255, 255, 255]

        boundary_overlay = cv.cvtColor(boundary_overlay, cv.COLOR_BGR2RGB)

        fig, ax = plt.subplots(nrows=5, sharex=True, sharey=True, figsize=(24, 40))

        ax[0].set_title('Original image')
        ax[0].imshow(cv.cvtColor(img, cv.COLOR_BGR2RGB))
        ax[1].set_title('Segmentation map')
        ax[1].imshow(output2)
        ax[2].set_title('Boundary overlay')
        ax[2].imshow(boundary_overlay)
        ax[3].set_title('Tree Relationship Structure')
        plot_tree(tree, ax=ax[3])
        ax[4].set_title('Region adjacency graph')
        graph.show_rag(segments, g, cv.cvtColor(img, cv.COLOR_BGR2RGB), ax=ax[4])

        for a in ax:
            a.axis('off')

        plt.savefig(output)
        plt.close()


if __name__ == '__main__':

    if not os.path.exists(OUTPUT_PATH):
        os.makedirs(OUTPUT_PATH)

    # Image Counting
    question1()

    # image Segmentation
    question2()

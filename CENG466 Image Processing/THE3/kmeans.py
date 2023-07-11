import numpy as np
from sklearn.cluster import KMeans
from sklearn.utils import shuffle
import cv2 as cv
import matplotlib.pyplot as plt

# Load the image and convert it to a NumPy array
image = cv.imread("THE3_images/1_source.png")
image = np.array(image)

# Flatten the image into a single-dimensional array
image = image.reshape((image.shape[0] * image.shape[1], 3))

# Shuffle the image to improve the clustering results
image = shuffle(image, random_state=0)

# Create a KMeans model and fit it to the image
model = KMeans(n_clusters=5)
model.fit(image)

# Get the cluster labels for each pixel
clusters = model.predict(image)

# Reshape the clusters back into the original image shape
clusters = clusters.reshape((image.shape[0] // image.shape[1], image.shape[1]))

# Display the resulting image
plt.imshow(clusters)
plt.show()

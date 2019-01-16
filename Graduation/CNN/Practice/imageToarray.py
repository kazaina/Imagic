import tensorflow as tf
import numpy as np
import PIL.Image as pilimg

img = pilimg.open('test.PNG') # bring image file using PIL
#img.show()
img = np.array(img) #convert to array


x = tf.placeholder(tf.float32, [None, len(img[0]) * len(img)])

print(img[0])

import tensorflow as tf
import numpy as np

## Parameters ##
LearningRate = 0.1
ROW = 8

data = np.loadtxt("Preferance_Data.csv", delimiter = ',', dtype = np.float32)

x_data = data[:,0:-1]
y_data = data[:,[-1]]

x = tf.placeholder(tf.float, shape = [None, ROW])
y = tf.placeholder(tf.float, shape = [None, 1])

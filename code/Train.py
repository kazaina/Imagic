import tensorflow as tf
import numpy as np

## Parameters ##
LearningRate = 0.1
condition = 8
nb_classes = 14

data = np.loadtxt('Preferance_Data.csv', delimiter = ',', dtype = np.float32)

x_data = data[:,0:-1]
y_data = data[:,[-1]]

x = tf.placeholder(dtype = tf.float32, shape = [None, condition)
y = tf.placeholder(dtype = tf.int32, shape = [None, 1])

y_one_hot = tf.one_hot(y, n)
y_one_hot = tf.reshape(y_one_hot, [-1, nb_classes])

w = tf.Variable(tf.random_normal([]))





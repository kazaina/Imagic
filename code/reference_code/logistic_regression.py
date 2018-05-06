import tensorflow as tf
import numpy as np


xy = np.loadtxt('diavate_data.csv', delimiter=',' , dtype=np.float32)

x_data = xy[:,0:-1]
y_data = xy[:,[-1]]

x = tf.placeholder(tf.float32, shape=[None, 8])
y = tf.placeholder(tf.float32, shape=[None,1])

w = tf.Variable(tf.random_normal([8,1]),name='weight')
b = tf.Variable(tf.random_normal([1]),name = 'bias')

hypo = tf.sigmoid(tf.matmul(x,w) + b)
cost = -tf.reduce_mean(y*tf.log(hypo) + (1-y)*tf.log(1-hypo))
train = tf.train.GradientDescentOptimizer(learning_rate=0.08).minimize(cost)

prediction= tf.cast(hypo>0.5,dtype=tf.float32)
accuracy = tf.reduce_mean(tf.cast(tf.equal(prediction, y), dtype=tf.float32))

with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    feed = {x:x_data, y:y_data}
    for step in range(30001):
        sess.run(train, feed_dict=feed)
        if step % 1000 == 0:
            print(step, sess.run(cost, feed_dict=feed))
    h, c, a = sess.run([hypo, prediction, accuracy],feed_dict=feed)
    print("Hypothesis: {}\nCorrect: {}\nAccuracy: {:.2%}\n".format(h,c,a))

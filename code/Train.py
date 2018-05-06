import tensorflow as tf
import numpy as np

## Parameters ##
LearningRate = 0.02
condition = 12
nb_classes = 14

data = np.loadtxt('a.csv', delimiter = ',', dtype = np.float32)

x_data = data[:,0:-1]
y_data = data[:,[-1]]

x = tf.placeholder(dtype = tf.float32, shape = [None, condition])
y = tf.placeholder(dtype = tf.int32, shape = [None, 1])

y_one_hot = tf.one_hot(y, nb_classes)
y_one_hot = tf.reshape(y_one_hot, [-1, nb_classes])

w = tf.Variable(tf.random_normal([condition, nb_classes]))
b = tf.Variable(tf.random_normal([nb_classes]))
                                                
logits = tf.matmul(x,w) + b
hypo = tf.nn.softmax(logits)
cost_i = tf.nn.softmax_cross_entropy_with_logits_v2(logits = logits, labels = y_one_hot)
cost = tf.reduce_mean(cost_i)

train = tf.train.AdamOptimizer(learning_rate=LearningRate).minimize(cost)
prediction = tf.argmax(hypo,1)
cor_predict = tf.equal(prediction, tf.argmax(y_one_hot,1))

accuracy = tf.reduce_mean(tf.cast(cor_predict, dtype=tf.float32))

with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    feed = {x:x_data, y:y_data}
    for step in range(30000):
        sess.run(train, feed_dict= feed)
        if step % 1000 ==0:
            print("Step: {}  Loss: {}".format(step, sess.run(cost, feed_dict=feed)))
    acc = sess.run(accuracy, feed_dict=feed)
    print("Accuracy {:.2%}".format(acc))

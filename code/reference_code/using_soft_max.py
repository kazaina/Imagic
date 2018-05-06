import tensorflow as tf
import numpy as np

xy = np.loadtxt('diavate_data.csv', delimiter=',', dtype=np.float32)
nb_classes = 2

x_data = xy[:,0:-1]
y_data = xy[:,[-1]]

x = tf.placeholder(dtype= tf.float32, shape=[None, 8])
y = tf.placeholder(dtype= tf.int32, shape=[None,1])

y_one_hot = tf.one_hot(y, nb_classes)
y_one_hot = tf.reshape(y_one_hot,[-1,nb_classes])

w = tf.Variable(tf.random_normal([8,nb_classes]), name='weight')
b = tf.Variable(tf.random_normal([nb_classes]),name = 'bias')

logits = tf.matmul(x,w)+b

hypo = tf.nn.softmax(logits)
cost_i = tf.nn.softmax_cross_entropy_with_logits_v2(logits=logits, labels=y_one_hot)
cost = tf.reduce_mean(cost_i)
#cost = tf.reduce_mean(-tf.reduce_sum(y*tf.log(hypo),axis=1))

optimizer = tf.train.GradientDescentOptimizer(learning_rate=0.009).minimize(cost)

#prediction = tf.cast(hypo>0.5, tf.float32)
#accuracy = tf.reduce_mean(tf.cast(tf.equal(prediction,y),dtype=tf.float32))

prediction = tf.argmax(hypo,1)
correct_prediction = tf.equal(prediction, tf.argmax(y_one_hot,1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, dtype=tf.float32))

with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    feed = {x:x_data, y:y_data}
    for step in range(50001):
        sess.run(optimizer,feed_dict=feed)
        if step % 1000 == 0:
            print(step, sess.run(cost, feed_dict=feed))
    a = sess.run(accuracy, feed_dict=feed)
    print("Accuracy : {:.2%}".format(a))

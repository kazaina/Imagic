import tensorflow as tf
import numpy as np

def MinMaxScaler(data):
    ''' Min Max Normalization
    Parameters
    ----------
    data : numpy.ndarray
        input data to be normalized
        shape: [Batch size, dimension]
    Returns
    ----------
    data : numpy.ndarry
        normalized data
        shape: [Batch size, dimension]
    References
    ----------
    .. [1] http://sebastianraschka.com/Articles/2014_about_feature_scaling.html
    '''
    numerator = data - np.min(data, 0)
    denominator = np.max(data, 0) - np.min(data, 0)
    # noise term prevents the zero division
    return numerator / (denominator + 1e-7)

xy = np.loadtxt('Grade_data.csv', delimiter=',',dtype=np.float32) #load csv file to train tensorflow
#xy = MinMaxScaler(xy)
#
# print(xy)
x_data = xy[:,0:-1]
y_data = xy[:,[-1]]

## Variable of dynamical
nb_classess = 10
lr = 0.01

x = tf.placeholder(tf.float32, [None, 8])
y = tf.placeholder(tf.int32, [None, 1])

y_one_hot = tf.one_hot(y,nb_classess)
y_one_hot = tf.reshape(y_one_hot,[-1, nb_classess])

w = tf.Variable(tf.random_normal([8,nb_classess]),name='weight')
b = tf.Variable(tf.random_normal([nb_classess]), name='bias')

logits = tf.matmul(x,w) + b
hypo = tf.nn.softmax(logits)

cost_i = tf.nn.softmax_cross_entropy_with_logits(logits=logits, labels=y_one_hot)
cost = tf.reduce_mean(cost_i)

optimizer = tf.train.GradientDescentOptimizer(learning_rate=lr).minimize(cost)

prediction = tf.argmax(hypo,1)

correct_prediction = tf.equal(prediction,tf.argmax(y_one_hot,1))

accuracy = tf.reduce_mean(tf.cast(correct_prediction,tf.float32))

with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    for step in range(5000):
        sess.run(optimizer,feed_dict={x:x_data, y:y_data})
        if step%100 ==0:
             #print("W: {}".format(sess.run(w)))
            #loss, acc = sess.run([cost, accuracy], feed_dict={x:x_data, y:y_data})
            #print("Step : {}\tLoss : {:3f}\tAcc : {:.2%}".format(step,loss,acc))

    # print("Train Complete.")
    #
    # score = [int(x) for x in input("Input Scores: ").split()]
    #
    # pred = sess.run(prediction, feed_dict={x:[score,]})
    # print("Your Grade is: ", end ='')
    #
    #
    # print(pred[0])
    # # if grade is 1: print("A+")
    # # elif grade i50s 2: print("A")
    # # elif grade is 3: print("B+")
    # # elif grade is 4: print("B")
    # # elif grade is 5: print("C+")
    # # elif grade is 6: print("C")
    # # elif grade is 7: print("D+")
    # # elif grade is 8: print("D")
    # # else: print("F")


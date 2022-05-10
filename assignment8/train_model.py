import tensorflow as tf
import numpy as np
import random

print(tf.__version__)
from tensorflow import keras
from tensorflow.keras import layers

inputs = keras.Input(shape=(2), name='num')
x = layers.Dense(units=2, activation=tf.nn.relu)(inputs)
outputs = layers.Dense(units=1)(x)

model = keras.Model(inputs=inputs, outputs=outputs, name='MLP_XOR')
model.summary()

train_T = 500
X_train = np.zeros((train_T, 2))
Y_train = np.zeros(train_T)
for i in range(train_T):
    a = random.randint(0, 1)
    b = random.randint(0, 1)
    X_train[i][0] = a
    X_train[i][1] = b
    Y_train[i] = a ^ b

learning_rate = 0.01
model.compile(optimizer=tf.keras.optimizers.Adam(learning_rate), loss='mse')
model.fit(X_train, Y_train, batch_size=16, epochs=5, validation_split=0.1)

test_T = 20
for i in range(test_T):
    a = random.randint(0, 1)
    b = random.randint(0, 1)
    inputs = np.array([a, b]).reshape(1, 2)
    print(str(a) + "^" + str(b) + "=" + str(a ^ b))
    result = model.predict(inputs) > 0.5
    print("predict: {0:2f}".format(result[0][0] > 0.5))

model.save('MLP_XOR.h5')

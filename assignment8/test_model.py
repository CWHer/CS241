import tensorflow as tf
from tensorflow import keras
import random
import numpy as np

model = keras.models.load_model("MLP_XOR.h5")

model.summary()

test_T = 20
for i in range(test_T):
    a = random.randint(0, 1)
    b = random.randint(0, 1)
    inputs = np.array([a, b]).reshape(1, 2)
    print(str(a) + "^" + str(b) + "=" + str(a ^ b))
    result = model.predict(inputs) > 0.5
    print("predict: {0:2f}".format(result[0][0] > 0.5))

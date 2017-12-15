# 2b

import numpy as np
import matplotlib.pyplot as pl

data = [[40, 713],
        [42, 1812],
        [45, 7705],
        [47, 19966],
        [50, 84716],
        [52, 221387],
        [55, 938937]]

# approach 1: fit an exponential function. doesn't quite work
#fun = lambda x: (1, x, x**2, 1.8**x, 1.9**x, 2**x)
#A = np.array([fun(data[i][0]) for i in range(len(data))], dtype=np.float64)
#
#y = np.array([data[i][1] for i in range(len(data))], dtype=np.float64)
#
#out = np.linalg.lstsq(A, y)
#x, res, rank, s = out
#print(x)
#print(res)
#print(rank)
#print(s)
#
#
#test = np.array(fun(50), dtype=np.float64).dot(x)
#print(test)
#
#print('original    new')
#for i in range(len(A)):
#    A_ = np.copy(A)
#    y_ = np.copy(y)
#    A_ = np.delete(A_, i, 0)
#    y_ = np.delete(y_, i)
#    out = np.linalg.lstsq(A_, y_)
#    x, res, rank, s = out
#    test = np.array(fun(data[i][0]), dtype=np.float64).dot(x)
#    print('%9.2f   %9.2f' % (y[i], test))
#    print(x)
#print('#################################')

# approach 2: fit an affine function to transformed data
#                     vvv log2 #of clock ticks
data2 = [[data[i][0], np.log2(data[i][1]*(3060000000/1000))]\
         for i in range(len(data))]
data2 = np.array(data2, dtype=np.float64)
pl.plot(data2[:,0], data2[:,1], '.')

fun = lambda x: (1, x)

A = np.array([fun(data2[i][0]) for i in range(len(data2))], dtype=np.float64)
y = np.array([data2[i][1] for i in range(len(data2))], dtype=np.float64)
out = np.linalg.lstsq(A, y)
x, res, rank, s = out
r2 = 1 - res / (len(y) * y.var())

ytarget = np.log2(np.float64(18446744073709551615-3060000000*60))
ypredict = np.array([1, 88], dtype=np.float64).dot(x)# = (1, n).dot(x)

print(ytarget, ypredict)



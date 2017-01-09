import matplotlib.pyplot as plt
import sys
name = sys.argv[1]
X, Y = [], []
with open(name) as f:
    for line in f:
        x, y = line.split()
        X.append(int(x))
        Y.append(float(y))
plt.plot(X, Y)
plt.show()

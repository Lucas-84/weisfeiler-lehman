import matplotlib.pyplot as plt
import sys
name = sys.argv[1]
C = []
with open(name) as f:
    for line in f:
        x, y = line.split()
        C.append((int(x), float(y)))
C.sort()
X, Y = [], []
for x, y in C:
    X.append(x)
    Y.append(y)
plt.plot(X, Y)
plt.show()

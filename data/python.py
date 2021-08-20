import os
from matplotlib import pyplot as plt

with open("data/polygon_irregular.txt") as file:
    xs, ys = list(zip(*list(map(lambda i: list(map(float, i.split())), file.readlines()))))
    for (i, (x, y)) in enumerate(zip(xs, ys)):
        plt.text(x, y, str(i))
    plt.plot(xs, ys)
    plt.show()
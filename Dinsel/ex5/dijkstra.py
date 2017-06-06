#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
from collections import defaultdict
from random import shuffle, choice


class Graph():
    """."""
    def __init__(self):
        """."""
        self.nodes = set()
        self.edges = defaultdict(list)
        self.distances = {}

    def addNode(self, value):
        """Add a ndoe to the graph."""
        self.nodes.add(value)


def randomPoints():
    """Generate a 2D set of random points to exercise with."""
    size = 10
    x = np.random.randint(0, 9, size=size, dtype='l')
    y = np.random.randint(0, 9, size=size, dtype='l')
    w = np.random.randint(1000, high=5000, size=size, dtype='l')

    return x, y, w


def plotPoints(x, y):
    """Plot random genrated points to see whats happening."""
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)

    # plot dots
    ax.scatter(x, y, s=20, c='#00a0fa', edgecolor='none', zorder=2)

    # # plot every connection
    coords = list(zip(x, y))
    print(choice(coords))
    for dot1 in coords:
        # coords2 = shuffle(coords)
        # print(shuffle(coords))
        new = []
        for i, dot2 in enumerate(coords):
            print(dot1, dot2)
            if dot1 != dot2:
                ax.plot(*zip(*(dot1, dot2)), 'lightgray', zorder=1)

    # ax.set_xlim(-1, 10)
    # ax.set_ylim(-1, 10)
    ax.set_aspect('equal')
    plt.show()

def readGPH():
    """."""
    path = 'world666.gph'
    data = np.genfromtxt(path, dtype=float, comments='#', delimiter=' ', skip_header=1)
    # print(data.shape)
    return data[:, 0], data[:, 1], data[:, 2]

def dijkstra():
    """."""


# x, y, w = readGPH()
x, y, w = randomPoints()
plotPoints(x, y)

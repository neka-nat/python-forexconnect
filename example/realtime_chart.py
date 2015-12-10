import sys
import collections
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import FormatStrFormatter

fig, ax = plt.subplots()
ax.yaxis.set_major_formatter(FormatStrFormatter('%.4f'))
plt.ion()
plt.show()
x = np.arange(0, 100)
datum = {}
lines = {}

def add_data(name, data):
    global datum
    datum[name] = collections.deque(maxlen = len(x))
    datum[name].extend(data)

def init():
    global datum, lines
    for key in datum.keys():
        lines[key], = ax.plot(x, np.array(datum[key]), "-")
    ax.relim()
    ax.autoscale_view()

def update_data(name, d):
    global datum, lines
    datum[name].append(d)
    for key in datum.keys():
        lines[key].set_ydata(datum[key])
    ax.relim()
    ax.autoscale_view()
    plt.draw()

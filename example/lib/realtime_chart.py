import collections
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import FormatStrFormatter

fig, ax = plt.subplots()
ax.yaxis.set_major_formatter(FormatStrFormatter('%.4f'))
x = np.arange(0, 100)
datum = {}
lines = {}

def add_data(name, data):
    global datum
    datum[name] = collections.deque(maxlen = len(x))
    datum[name].extend(data)

def init(title=''):
    global datum, lines
    plt.title(title)
    for key in datum.keys():
        lines[key], = ax.plot(x, np.array(datum[key]), "-")
    plt.legend(datum.keys(), loc='upper left')
    ax.relim()
    ax.autoscale_view()
    plt.pause(0.01)

def update_data(dic, reset=False):
    global datum, lines
    for k, v in dic.items():
        if isinstance(v, collections.Iterable):
            if reset:
                datum[k].clear()
            datum[k].extend(v)
        else:
            datum[k].append(v)
    for key in datum.keys():
        lines[key].set_ydata(datum[key])
    ax.relim()
    ax.autoscale_view()
    plt.pause(0.01)

"""
An example plotting time series currency.
"""
import sys
import forexconnect
import login_manager
import collections
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

if len(sys.argv) < 2:
    print "Usage: python tick_animation.py instrument(etc. 'EUR/USD')"
    sys.exit()

instrument = sys.argv[1]
username, password, connection = login_manager.get_login_params()
client = forexconnect.ForexConnectClient(username,
                                         password,
                                         connection)
fig, ax = plt.subplots()
x = np.arange(0, 100)
data = collections.deque(maxlen = len(x))
data.extend([client.get_ask(instrument)] * len(x))
line, = ax.plot(x, np.array(data), "r-")

def animate(i):
    global data
    data.append(client.get_ask(instrument))
    line.set_ydata(np.array(data))
    ax.relim()
    ax.autoscale_view()
    return line,

def init():
    global data
    line.set_ydata(np.ma.array(data, mask=True))
    return line,

ani = animation.FuncAnimation(fig, animate, np.arange(0, 100), init_func=init,
                              interval=200)
plt.show()

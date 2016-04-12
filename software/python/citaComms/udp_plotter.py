"""
Emulate an oscilloscope.  Requires the animation API introduced in
matplotlib 1.0 SVN.
"""
import numpy as np
from matplotlib.lines import Line2D
import matplotlib.pyplot as plt
import matplotlib.animation as animation

import os
from socket import *

class Scope(object):
    def __init__(self, ax, maxt=1, dt=0.02):
        # UDP Variables
        self.host = '127.0.0.1'
        self.port = 6001
        self.buf = 1024
        self.addr = (self.host, self.port)
        self.socket = socket(AF_INET, SOCK_DGRAM)
        self.socket.bind(self.addr)
        self.variables = [
            'running_avg_light',
            'decaying_light_min',
            'decaying_light_max',
            #'value',
            #'bucket',
        ]

        # Graph Variables
        self.ax = ax
        self.dt = dt
        self.mint = 0
        self.maxt = maxt
        self.miny = -0.1
        self.maxy = 200
        self.t = 0
        self.tdata = [[0] for var in self.variables]
        self.ydata = [[0] for var in self.variables]
        self.lines = [Line2D(self.tdata[i], self.ydata[i]) for i,var in enumerate(self.variables)]
        for line in self.lines:
            self.ax.add_line(line)
        self.ax.set_ylim(self.miny, self.maxy)
        self.ax.set_xlim(0, self.maxt)

    def update(self, y):
        self.t = self.t + self.dt

        (data, addr) = self.socket.recvfrom(self.buf)

        key, value = data.decode('UTF-8').split(':')
        value = float(value)

        for i, var in enumerate(self.variables):
            if key == var:
                self.ydata[i].append(value)
                self.tdata[i].append(self.t)

                if self.t > self.maxt:
                    self.mint = self.t - (self.maxt - self.mint)
                    self.maxt = self.t + (self.maxt - self.mint)*0.1
                if value > self.maxy:
                    self.maxy = value + 0.1*value

                self.ax.set_xlim(self.mint, self.maxt)
                self.ax.set_ylim(self.miny, self.maxy)
                self.ax.figure.canvas.draw()

                self.lines[i].set_data(self.tdata[i], self.ydata[i])

        return self.lines

if __name__ == '__main__':
    fig, ax = plt.subplots()
    scope = Scope(ax)

    # pass a generator in "emitter" to produce data for the update func
    ani = animation.FuncAnimation(fig, scope.update, interval=10, blit=True)

    plt.show()

    scope.socket.close()
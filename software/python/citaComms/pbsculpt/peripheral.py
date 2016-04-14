import random
from collections import deque

import simpleTeensyComs

import numpy as np

class AdaptiveValuationMixin(object):
    N_SMOOTHING = 10
    N_RUNNING = 100
    N_STDEV = 1

    def __init__(self):
        self._values = deque([], maxlen=self.N_RUNNING)

    @property
    def smoothed_value(self):
        return np.average(self._values[:-self.N_SMOOTHING], weights=list(range(self.N_SMOOTHING)))

    @property
    def running_avg(self):
        return np.average(self._values)

    @property
    def stdev(self):
        return np.std(self._values)

    @property
    def value(self):
        return (self.smoothed_value - self.running_avg) / (self.N_STDEV * self.stdev)

class States:
    ACTIVE = 0
    INACTIVE = 1

class StatefulMixin(object):
    OVERLOAD_DECAY = 0.75
    OVERLOAD_MAX = 175
    RESTING_DECAY = 0.75
    RESTING_MIN = 2

    def __init__(self):
        self.state = States.ACTIVE
        self.overload = 0

    def loop(self):
        if self.state == States.ACTIVE:
            # Decay the overload variable
            self.overload = self.overload * self.OVERLOAD_DECAY

            # Add to overload
            self.overload += self.value

            if self.overload > self.OVERLOAD_MAX:
                self.state = States.INACTIVE
        else:
            self.overload = self.overload * self.RESTING_DECAY

            if self.overload < self.RESTING_MIN:
                self.state = States.ACTIVE

class Peripheral(object):
    '''
    '''

    def __init__(self, parent, address, type):
        '''
        Parameters
        ----------
            parent : Node/DeviceModule
                The parent node/device that the peripheral belongs to
            port : int
                The physical port on the Control Node that the peripheral's Device Module is connected on
            address : int
                The address (1-8) on the 8P cable that the peripheral lives on
            type : int
                The TypeID of the peripheral
        '''
        self.parent = parent
        self.address = address
        self.type = type

        self._value = 0

    def loop(self):
        pass

    @property
    def port(self):
        return self.parent.port

    @property
    def byte_array(self):
        out_b_array = self.address.to_bytes(1, byteorder='big')
        out_b_array += self.type.to_bytes(1, byteorder='big')
        out_b_array += self.port.to_bytes(1, byteorder='big')
        return out_b_array

    @property
    def connection(self):
        return self.parent.connection

    @property
    def serial_number(self):
        return self.parent.parent.serial_number

    @property
    def origin(self):
        return self.parent.parent.origin

    @property
    def value(self):
        return self._value

    @property
    def is_sensor(self):
        return False

    @property
    def is_actuator(self):
        return False

    def __str__(self):
        return "Address = %d, Type = %d, Port = %d" % (self.address, self.type, self.port)

class Sensor(Peripheral, AdaptiveValuationMixin):
    def loop(self):
        simpleTeensyComs.Read(self.connection, self.serial_number, self.origin, self.byte_array(), 0)

    @property
    def is_sensor(self):
        return True


class Actuator(Peripheral, StatefulMixin):
    def loop(self):
        super(Actuator, self).loop()

        if self.state == States.ACTIVE:
            simpleTeensyComs.Fade(self.connection, self.serial_number, self.origin, self.byte_array(),
                                  self.value, 0)
        else:
            simpleTeensyComs.Fade(self.connection, self.serial_number, self.origin, self.byte_array(),
                                  0, 0)

    @property
    def is_actuator(self):
        return True

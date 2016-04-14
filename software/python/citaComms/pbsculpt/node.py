import datetime
import random

import serial
from serial.tools import list_ports

import simpleTeensyComs

from .device import DeviceModule
from .peripheral import Sensor, Actuator


class Node(object):
    '''
    '''
    origin = 222222 # The computer's virtual serial number

    def __init__(self, serial_number, timeout=1):
        '''
        Parameters
        ----------
            serial_number : int
                The serial number of the Teensy

            timeout : int
                The timeout in seconds for serial operations

        '''
        self.serial_number = serial_number

        # Initialize the connection
        port = Node.port_for_serial(self.serial_number)
        self.connection = serial.Serial(port)
        self.connection.timeout = timeout

        self.init_devices()

    def init_devices(self):
        peripherals = simpleTeensyComs.QueryIDs(self.connection, self.serial_number, self.origin)

        self.devices = {}
        for peripheral in peripherals:
            if peripheral.port not in self.devices:
                self.devices[peripheral.port] = DeviceModule(self, peripheral.port)

            if peripheral.type % 2 == 0:
                self.devices[peripheral.port].add(
                    Sensor(self.devices[peripheral.port], peripheral.address, peripheral.type))
            else:
                self.devices[peripheral.port].add(
                    Actuator(self.devices[peripheral.port], peripheral.address, peripheral.type))


    def loop(self):
        for port in self.devices:
            self.devices[port].loop()

    def exit(self):
        pass

    @staticmethod
    def port_for_serial(serial_number):
        port_mapping = {}
        for port, pname, desc in list_ports.comports():
            if desc.split()[0] == 'USB':
                snr = int(desc.split()[2].split('=')[1][:-1])

                if snr == serial_number:
                    return port

        raise ConnectionError('No port containing Teensy with serial number %d' %serial_number)
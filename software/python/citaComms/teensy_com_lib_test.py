''' Simple Logger Main

Starts a python process that turns the various actuators on randomly and logs the sensor readings to an sqlite database.
'''
from datetime import datetime
import random
import time
from serial.tools import list_ports

import simpleTeensyComs
from pymongo import MongoClient

import pbsculpt
from pbsculpt.node import Node

SLEEP_LENGTH = 1

serials = []

loop_count = 0

def map_ports(serials):
    '''Map ports to serial number listings

    :type serials: List of serial numbers
    :return: Dictionary of port mappings {serialNumber:port}

    :todo: Is this cross-platform friendly?
    '''
    port_mapping = {}
    for port, pname, desc in list_ports.comports():
        if desc.split()[0] == 'USB':
            snr = desc.split()[2].split('=')[1][:-1]

            port_mapping[int(snr)] = port

    return port_mapping

def simple_logger_setup(args):
    ''' Run the simple logger setup

    :type args: object
    :return: None
    '''
    global serials, origin, nodes, db

    print(args)

    # Unpack the input arguments
    serials = args.teensy
    origin = args.comp_serial

    # A Dict of the communications in the form of SERIAL_NUMBER: CONNECTION
    print( map_ports(serials) )
    portmap = map_ports(serials)

    nodes = [Node(sn) for sn in portmap]

    for node in nodes:
        # Set up the Teensy communications
        numDevices = simpleTeensyComs.QueryNumDevices(node, node.serial_number, origin)
        print('The teensy has', numDevices, 'devices')
        devices = simpleTeensyComs.QueryIDs(node, node.serial_number, origin)

    # Set up the database
    client = MongoClient()
    db = client.USBStressTest

def shutdown():
    for node in nodes:
        for actuator in node.actuators:
            simpleTeensyComs.Fade(node.connection, node.serial_number, node.origin, actuator.byte_array(), 0, 0)

def simple_logger_loop():
    ''' Run the simple logger loop

    :return:
    '''

    for node in nodes:
        node.loop()

def port_serial_type(port_serial_string):
    serial, port = port_serial_string.split(',')
    serial = int(serial)

    return (serial, port,)

if __name__ == '__main__':

        import argparse

        parser = argparse.ArgumentParser(description='Start a simple logging process using random outputs and logging the inputs.')
        parser.add_argument('--teensy', dest='teensy', type=str, help='The Teensy serial numbers.', nargs='+')
        parser.add_argument('comp_serial', type=int, help='The computers serial number for the purposes of simulation [22222]',
                           default=simpleTeensyComs.cbla_pc_id, nargs='?' )
        parser.add_argument('grasshopper_serial', type=int, help='The Grasshopper nodes serial number for the purposes of simulation [33333]',
                           default=simpleTeensyComs.udp_node_id, nargs='?' )

        args = parser.parse_args()

        simple_logger_setup(args)

        # Run until CTRL+C is pressed
        try:
            while True:
                start = datetime.now()
                simple_logger_loop()
                end = datetime.now()
                loop_count += 1
                print('Looped %d times...Loop took %s' %(loop_count, str(end-start)))
                time.sleep(SLEEP_LENGTH)
        except KeyboardInterrupt:
            shutdown()

        except Exception as e:
            shutdown()
            print(e)

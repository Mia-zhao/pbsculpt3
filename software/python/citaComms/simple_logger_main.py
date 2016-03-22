''' Simple Logger Main

Starts a python process that turns the various actuators on randomly and logs the sensor readings to an sqlite database.
'''
from datetime import datetime
import random
import time

import simpleTeensyComs
from pymongo import MongoClient

SLEEP_LENGTH = 1

destination = -1
origin = -1
teensyComms = None
actuators = {} # Dict containting the device and its most recent reading
sensors = {}
db = None

def simple_logger_setup(args):
    ''' Run the simple logger setup

    :type args: object
    :return: None
    '''
    global destination, origin, teensyComms, actuators, sensors, db

    print(args)

    # Unpack the input arguments
    destination = args.teensy_serial
    origin = args.comp_serial
    Grasshopper = args.grasshopper_serial
    teensyComms = simpleTeensyComs.initializeComms(args.teensy_comport)

    # Set up the Teensy communications
    numDevices = simpleTeensyComs.QueryNumDevices(teensyComms, destination, origin)
    print('The teensy has', numDevices, 'devices')
    devices = simpleTeensyComs.QueryIDs(teensyComms, destination, origin)

    for i in range(len(devices)):
        print(devices[i].pr())
        if devices[i].type%2 == 0:
            sensors[devices[i]] = 0
        else:
            actuators[devices[i]] = 0

    # Set up the database
    client = MongoClient()
    db = client.USBStressTest

def simple_logger_loop():
    ''' Run the simple logger loop

    :return:
    '''

    for actuator in actuators:
        actuators[actuator] = random.randint(0, 15)
        simpleTeensyComs.Fade(teensyComms, destination, origin, actuator.genByteStr(), int(actuators[actuator]),0)

        db.readings.insert_one({
            'datetime': datetime.now(),
            'address': actuator.address,
            'type': actuator.type,
            'port': actuator.port,
            'value': actuators[actuator],
        })

    for sensor in sensors:
        sensors[sensor] = simpleTeensyComs.Read(teensyComms, destination, origin, sensor.genByteStr(), 0)

        db.readings.insert_one({
            'datetime': datetime.now(),
            'address': sensor.address,
            'type': sensor.type,
            'port': sensor.port,
            'value': sensors[sensor],
        })

if __name__ == '__main__':

    import argparse

    parser = argparse.ArgumentParser(description='Start a simple logging process using random outputs and logging the inputs.')
    parser.add_argument('teensy_comport', type=str, help='The Teensy com port.')
    parser.add_argument('teensy_serial', type=int, help='The Teensy serial number - usually 6-7 digits.')
    parser.add_argument('comp_serial', type=int, help='The computers serial number for the purposes of simulation [22222]',
                       default=simpleTeensyComs.cbla_pc_id, nargs='?' )
    parser.add_argument('grasshopper_serial', type=int, help='The Grasshopper nodes serial number for the purposes of simulation [33333]',
                       default=simpleTeensyComs.udp_node_id, nargs='?' )

    args = parser.parse_args()

    simple_logger_setup(args)

    # Run until CTRL+C is pressed
    try:
        while True:
            simple_logger_loop()
            time.sleep(SLEEP_LENGTH)
    except KeyboardInterrupt:
        pass
''' Simple Logger Main

Starts a python process that turns the various actuators on randomly and logs the sensor readings to an sqlite database.
'''
from datetime import datetime
import random
import time

import simpleTeensyComs
from pymongo import MongoClient

SLEEP_LENGTH = 1

serials = []
origin = -1
teensyComms = []
actuators = {} # Dict containting the device and its most recent reading
sensors = {}
db = None

loop_count = 0

def simple_logger_setup(args):
    ''' Run the simple logger setup

    :type args: object
    :return: None
    '''
    global serials, origin, teensyComms, actuators, sensors, db

    print(args)

    # Check to see that the number of Teensys makes sense
    if len(args.teensy_serial) != len(args.teensy_comport):
        raise AttributeError('The number of Serial Numbers does not match the number of Comm Ports given.')

    # Unpack the input arguments
    serials = args.teensy_serial
    origin = args.comp_serial
    Grasshopper = args.grasshopper_serial
    # A Dict of the communications in the form of SERIAL_NUMBER: CONNECTION
    teensyComms = dict(zip(serials, [simpleTeensyComs.initializeComms(p) for p in args.teensy_comport]))

    for sn in teensyComms:
        # Set up the Teensy communications
        numDevices = simpleTeensyComs.QueryNumDevices(teensyComms[sn], sn, origin)
        print('The teensy has', numDevices, 'devices')
        devices = simpleTeensyComs.QueryIDs(teensyComms[sn], sn, origin)

        sensors[sn] = {}
        actuators[sn] = {}

        for i in range(len(devices)):
            print(devices[i].pr())
            if devices[i].type%2 == 0:
                sensors[sn][devices[i]] = 0
            else:
                actuators[sn][devices[i]] = 0

    # Set up the database
    client = MongoClient()
    db = client.USBStressTest

def simple_logger_loop():
    ''' Run the simple logger loop

    :return:
    '''
    values = []
    readings = []

    for sn in teensyComms:
        for actuator in actuators[sn]:
            actuators[sn][actuator] = random.randint(0, 15)
            simpleTeensyComs.Fade(teensyComms[sn], sn, origin, actuator.genByteStr(), int(actuators[sn][actuator]),0)

            db.readings.insert_one({
                'datetime': datetime.now(),
                'teensy_serial': sn,
                'address': actuator.address,
                'type': actuator.type,
                'port': actuator.port,
                'value': actuators[sn][actuator],
            })

            values.append(actuators[sn][actuator])

        for sensor in sensors[sn]:
            sensors[sn][sensor] = simpleTeensyComs.Read(teensyComms[sn], sn, origin, sensor.genByteStr(), 0)

            db.readings.insert_one({
                'datetime': datetime.now(),
                'teensy_serial': sn,
                'address': sensor.address,
                'type': sensor.type,
                'port': sensor.port,
                'value': sensors[sn][sensor],
            })

            readings.append(sensors[sn][sensor])

    print('Acts: ' + str(values))
    print('Sens: ' + str(readings))

if __name__ == '__main__':

    import argparse

    parser = argparse.ArgumentParser(description='Start a simple logging process using random outputs and logging the inputs.')
    parser.add_argument('--com', dest='teensy_comport', type=str, help='The Teensy com port.', nargs='+')
    parser.add_argument('--sn', dest='teensy_serial', type=int, help='The Teensy serial number - usually 6-7 digits.', nargs='+')
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
        for sn in teensyComms:
            for actuator in actuators[sn]:
                simpleTeensyComs.Fade(teensyComms[sn], sn, origin, actuator.genByteStr(), 0, 0)
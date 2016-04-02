''' Simple Logger Main

Starts a python process that turns the various actuators on randomly and logs the sensor readings to an sqlite database.
'''
from datetime import datetime
import random
import time
from serial.tools import list_ports

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
    global serials, origin, teensyComms, actuators, sensors, db

    print(args)

    # Unpack the input arguments
    serials = args.teensy
    origin = args.comp_serial
    Grasshopper = args.grasshopper_serial
    # A Dict of the communications in the form of SERIAL_NUMBER: CONNECTION
    print( map_ports(serials) )
    portmap = map_ports(serials)
    teensyComms = dict([(sn, simpleTeensyComs.initializeComms(portmap[sn])) for sn in portmap])

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

def shutdown():
    for sn in teensyComms:
        for actuator in actuators[sn]:
            simpleTeensyComs.Fade(teensyComms[sn], sn, origin, actuator.genByteStr(), 0, 0)

def simple_logger_loop():
    ''' Run the simple logger loop

    :return:
    '''
    values = []
    readings = []

    for sn in teensyComms:
        for actuator in actuators[sn]:
            actuators[sn][actuator] = random.randint(0, 15)

            print(teensyComms[sn], actuator.genByteStr(), sn, origin)

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

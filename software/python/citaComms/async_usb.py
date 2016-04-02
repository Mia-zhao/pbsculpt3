import timeit

from threading import Thread
from datetime import datetime, timedelta

from simpleTeensyComs import *

def fade(connection, destination, computer_id, teensy_id, targetValue, speed, results, timeout=1):
    cmd = bytearray()
    cmd += teensy_id
    cmd += targetValue.to_bytes(1,'big')
    cmd += speed.to_bytes(2,'big')

    WriteToSerial(connection, destination, computer_id, FADE, cmd)

    start = datetime.now()
    while datetime.now()-start < timedelta(seconds=timeout):
        try:
            (msgsize,msg) = ReadFromSerial(connection)
            print('Message: %d, %s' %(msgsize, msg) )
            results[connection.port] = msg
            return msg
        except ConnectionError:
            pass

    raise TimeoutError('No reponse in the allotted timeout.')

def __test_async(connections, destinations, computer_ids, teensy_ids):
    results = {}

    t = []
    for i in range(2):
        t.append(Thread(target=fade, args=(connections[i], destinations[i],
                                computer_ids[i], teensy_ids[i], 15, 0, results)))

    for th in t:
        th.start()
        th.join()

    print(results)

def __test_sync(connections, destinations, computer_ids, teensy_ids):

    msg = []
    for i in range(2):
        msg.append(Fade(connections[i], destinations[i],
                        computer_ids[i], teensy_ids[i], 15, 0))

    print(msg)

if __name__=='__main__':

    computer_ids = [222222, 222222]
    connections = [initializeComms('/dev/ttyACM0'), initializeComms('/dev/ttyACM1')]
    destinations = [136324, 129272]
    teensy_ids = [b'\x04\x05\x01', b'\x06\x05\x01']

    print ("Async")
    print(timeit.timeit('__test_async(connections, destinations, computer_ids, teensy_ids)',
          'from __main__ import __test_async, computer_ids, connections, destinations, teensy_ids',
          number=10))

    print ("Sync")
    print(timeit.timeit('__test_sync(connections, destinations, computer_ids, teensy_ids)',
          'from __main__ import __test_sync, computer_ids, connections, destinations, teensy_ids',
          number=10))
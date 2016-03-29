# Very simple Serial Port Comms between Python and Teensy

import serial
import time
import socket

#setup the Teensy
#teensy_sernum = 69029 #107935 #69029
#teensy_comport = "COM4"

#CBLA setup
cbla_pc_id = 22222

#setup for UDP transmitting
UDPSend = True
#UDP_IP = "127.0.0.1"
UDP_IP = "10.1.45.171"
UDP_PORT = 6000
udp_node_id = 33333
#sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)


#Command definitions
QUERYNUMDEV = 1
QUERYIDS = 2
FADE = 3
READ = 4

def sendUDP(msg):
    pass
    #if UDPSend == True:
    #    sock.sendto(msg, (UDP_IP, UDP_PORT))


#Function to initialize the serial port
'''
@param timeout The timeout for USB Read in seconds
'''
def initializeComms(CoMId, timeout = 0.1):
    ser = serial.Serial(CoMId)
    ser.timeout = timeout
    return ser

def ReadFromSerial(ser):
    #print("Sleeping...")
    time.sleep(ser.timeout)
    #print("Waking...")
    if ser.is_open:
        numBytes = ser.in_waiting
        if numBytes > 0:
            r1 = ser.read_all()
            sendUDP(r1)
            #print(r1)
            resp = bytearray(r1)
        else:
            raise ConnectionError("No data at serial port. Did you forget to close TyQT or the Arduino Serial Monitor?")
    else:
            raise ConnectionError("Serial port not open. Did you forget to close TyQT or the Arduino Serial Monitor?")

    destId = int.from_bytes(resp[0:3], byteorder='big')
    origId = int.from_bytes(resp[3:6], byteorder='big')
    cmdNum = resp[6]
    msgSize = int.from_bytes(resp[7:11], byteorder='big')
    if msgSize > 0:
        if msgSize == 1:
            msg = resp[11]
        else:
            msg = resp[11:11+msgSize]
        #print(msg)

    return (msgSize,msg)


def WriteToSerial(tSerConnection, destID, origID, cmdNum, cmdData):
    out_b_array = destID.to_bytes(3,byteorder='big')
    out_b_array += origID.to_bytes(3,byteorder='big')
    out_b_array += cmdNum.to_bytes(1,byteorder='big')
    messageSize = len(cmdData)
    out_b_array += messageSize.to_bytes(4,byteorder='big')
    if cmdData:
        out_b_array += cmdData
    tSerConnection.write(out_b_array)
    #print(out_b_array)

def Echo(tSerConnection):
    tSerConnection.write(str.encode('Test Message'))
    time.sleep(1)
    if tSerConnection.is_open:
        numBytes = tSerConnection.in_waiting
        if numBytes > 0:
            r1 = tSerConnection.readline()
            #print(r1)

def QueryNumDevices(tSerConnection, destTeensy, myId):
    WriteToSerial(tSerConnection, destTeensy, myId, QUERYNUMDEV, [])
    (msgsize,msg) = ReadFromSerial(tSerConnection)
    if msgsize == 1:
        return msg
    else:
        print('Unexpected message length received for QueryNumDevices request')

class DevInfo(object):
    def __init__(self, addr, devtype, portnum):
        self.address = addr
        self.type = devtype
        self.port = portnum

    def genByteStr(self):
        out_b_array =self.address.to_bytes(1,byteorder='big')
        out_b_array +=self.type.to_bytes(1,byteorder='big')
        out_b_array +=self.port.to_bytes(1,byteorder='big')
        return out_b_array

    def pr(self):
        outstr = "Address = %d, Type = %d, Port = %d" % (self.address, self.type, self.port)
        return outstr



def parseQueryIDResponse(respStr):

    devList = []
    for i in range(0,len(respStr),3):
        devList.append(DevInfo(respStr[i], respStr[i+1], respStr[i+2]))
    return devList


def QueryIDs(tSerConnection, destTeensy, myId):
    WriteToSerial(tSerConnection, destTeensy, myId,QUERYIDS, [] )
    (msgsize,msg) =  ReadFromSerial(tSerConnection)
    if msgsize == 0:
        print("No devices connected, message length is zero to QueryIDs prompt")
    else:
        return parseQueryIDResponse(msg)

def Fade(tSerConnection, destTeensy, myId, deviceId, targetValue, speed):
    cmd = bytearray()
    cmd += deviceId
    cmd += targetValue.to_bytes(1,'big')
    cmd += speed.to_bytes(2,'big')
    #print(cmd)
    WriteToSerial(tSerConnection, destTeensy, myId,FADE, cmd)
    (msgsize,msg) = ReadFromSerial(tSerConnection)
    return msg

def ParseReadResponse(respStr):
    readDevId = int.from_bytes(respStr[0:3],byteorder='big') #this is wrong
    procType = respStr[3]  #should do some error checking to make sure this is what was requested
    readval = int.from_bytes(respStr[4:6], byteorder='big')
    #print('The sensor reading is',readval)
    return readval



def Read(tSerConnection, destTeensy, myId, deviceId, prepType):
    cmd = bytearray()
    cmd += deviceId
    cmd += prepType.to_bytes(1,'big')
    WriteToSerial(tSerConnection, destTeensy, myId, READ, cmd)
    (msgsize,msg) = ReadFromSerial(tSerConnection)
    if msgsize == 6:
        return ParseReadResponse(msg)
    else:
        print('Incorrect message size received for Read command')

def CBLAStatusReport(myId, destListener, CBLANodeId, numExperts, maxActionValue):
    cmd = bytearray()
    cmd += destListener.to_bytes(3,'big')
    cmd += myId.to_bytes(3,'big')
    cmd += (251).to_bytes(1,'big')
    cmd += (4).to_bytes(4,'big')
    cmd += CBLANodeId.to_bytes(1,'big')
    cmd += numExperts.to_bytes(1,'big')
    m = int(abs(10000*maxActionValue))
    cmd += m.to_bytes(2,'big')
    #print(cmd)
    sendUDP(cmd)

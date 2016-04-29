#! /usr/bin/python

# Import and init an XBee device
import struct

import binascii
from xbee import ZigBee
import serial

def tx(xbee, addr64, addr16, data):
    '''

    Parameters
    ----------
    xbee : xbee.ZigBee
        The xbee object to use for communication
    addr16 : 16-bit byte string
        The short address of the destination (e.g. b'\xDC\xD5')
    data : byte string
        The data packet to send

    '''
    xbee.tx(
        dest_addr_long=addr64,
        dest_addr=addr16,
        data=data
    )

if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument('serial', help='Serial port to connect to.')
    parser.add_argument('--baud', default=9600, type=int, help='Baud rate')

    subparsers = parser.add_subparsers(dest='messageType')

    # Transmit
    tx_parser = subparsers.add_parser('tx', help='Send a ZB Transmit Request (0x10)')
    tx_parser.add_argument('addr64', type=str, help='Long (64-bit) address of the destination.')
    tx_parser.add_argument('addr16', type=str, help='Short (16-bit) address of the destination, eg. DCD5.')
    tx_parser.add_argument('data', type=int, help='Quoted data to send')

    # Transmit Status
    subparsers.add_parser('txStatus', help='Send a ZB Transmit Status (0x8B)')

    # Receive Packet
    subparsers.add_parser('rx', help='Send a ZB Receive Packet (0x90)')

    args = parser.parse_args()

    ser = serial.Serial(args.serial, args.baud)
    # Use an XBee 802.15.4 device
    # To use with an XBee ZigBee device, replace with:
    #xbee = ZigBee(ser)
    xbee = ZigBee(ser)

    if args.messageType == 'tx':
        addr64 = binascii.unhexlify(args.addr64)
        addr16 = binascii.unhexlify(args.addr16) #bytes(args.addr16, 'utf-8')
        tx(xbee, addr64, addr16, struct.pack('>B',args.data))
'''

'''
import random
from binascii import hexlify

import pygame
from enum import Enum

NODE_ADDRS = [
    b'0013a20040e99c5c',
    b'0013a20040e99cb3',
    b'0013a20040f2a727',
]

NODE_POS = [
    (100, 100),
    (250, 100),
    (400, 100),
]

class PeripheralMode(Enum):

    OFF = 0
    TEST = 1
    SILENT = 2
    ACTIVE = 3
    BACKGROUND = 4
    SLAVE = 5

class EventType(Enum):
    BackgroundMode = 0
    BackgroundActivation = 1

class Event(object):

    def __init__(self, message_type, event_type, peripheral_type, port, address):

        self.message_type = message_type
        self.event_type = event_type
        self.peripheral_type = peripheral_type
        self.port = port
        self.address = address

class Ecosystem(object):

    def __init__(self):
        # List of nodes present
        self.node_list = pygame.sprite.Group()

        # Peripherals list
        self.peripherals_list = pygame.sprite.Group()

        # This is a list of every sprite.
        self.all_sprites_list = pygame.sprite.Group()

    def handle_zb_data(self, data):
        print('DATA: %s' %str(data))

        addr64 = hexlify(data['source_addr_long'])

        node = self.get_node(addr64)

        if node == False:
            node = Node(address64=addr64)

            self.node_list.add(node)
            self.all_sprites_list.add(node)

            if node.address64 in NODE_ADDRS:
                node.rect.x = NODE_POS[NODE_ADDRS.index(node.address64)][0]
                node.rect.y = NODE_POS[NODE_ADDRS.index(node.address64)][1]

        payload = [b for b in data['rf_data']]

        evt = Event(*payload)

        if evt.event_type == EventType.BackgroundActivation.value:
            node.event_queue.append(evt)

    def handle_zb_error(self, error):
        print('ERR: %s' %str(error))

    def get_node(self, address64):
        for node in self.node_list:
            if node.address64 == address64:
                return node

        return False

    '''def update(self):
        pass'''

class Node(pygame.sprite.Sprite):
    '''
    A node in a pbsculpt sculpture.
    '''

    DEFAULT_COLOUR = [255, 0, 0]

    def __init__(self, address64):
        super().__init__()

        #self.serial_number = 0
        #self.address16 = address16
        self.address64 = address64

        self.mode = PeripheralMode.ACTIVE

        width, height = 100, 100
        self.colour = self.DEFAULT_COLOUR

        # Pass in the color of the car, and its x and y position, width and height.
        # Set the background color and set it to be transparent
        self.image = pygame.Surface([width, height])
        self.image.fill(self.colour)
        #self.image.set_colorkey(colour)

        # Draw the car (a rectangle!)
        pygame.draw.rect(self.image, self.colour, [0, 0, width, height])

        # Instead we could load a proper pciture of a car...
        # self.image = pygame.image.load("car.png").convert_alpha()

        # Fetch the rectangle object that has the dimensions of the image.
        self.rect = self.image.get_rect()

        #self.rect.x = random.randint(0, 700)
        #self.rect.y = random.randint(0, 400)

        self.event_queue = []

        self.devices = []

    def update(self):
        super().update()

        if self.colour != self.DEFAULT_COLOUR:
            self.colour = [fade_from(c, dc) for c,dc in zip(self.colour, self.DEFAULT_COLOUR)]

        while len(self.event_queue) > 0:
            evt = self.event_queue.pop(0)

            if evt.event_type == EventType.BackgroundActivation.value:
                self.colour = [0, 255, 255]

        self.image.fill(self.colour)
        pygame.draw.rect(self.image, self.colour, self.rect)

def fade_from(x, y, rate = 0.99):
    return y - (y-x)*rate
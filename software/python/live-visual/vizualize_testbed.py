'''

'''
import pygame, serial, time
from xbee import ZigBee

from pbsculpt3.hardware import Node, Ecosystem

# Define some colors
BLACK = (  0,   0,   0)
WHITE = (255, 255, 255)
RED   = (255,   0,   0)

def print_data(data):
    """
    This method is called whenever data is received
    from the associated XBee device. Its first and
    only argument is the data contained within the
    frame.
    """
    print(data)

def print_error(data):
    """
    This method is called whenever data is received
    from the associated XBee device. Its first and
    only argument is the data contained within the
    frame.
    """
    print('ERROR: %s' %str(data))

def _init_xbee(port, callback, errcallback):
    print(port)
    PORT = '/dev/ttyUSB0'
    BAUD_RATE = 9600
    serial_port = serial.Serial(PORT, BAUD_RATE, timeout=0.1)
    zb = ZigBee(serial_port, callback=callback, error_callback=errcallback, escaped=True)

    return zb, serial_port

def main(args):
    pygame.init()

    ecosystem = Ecosystem()

    zb, ser = _init_xbee(args.port, callback=ecosystem.handle_zb_data, errcallback=ecosystem.handle_zb_error)

    screen_width = 700
    screen_height = 400
    screen = pygame.display.set_mode([screen_width, screen_height])

    # Loop until the user clicks the close button.
    done = False

    # Used to manage how fast the screen updates
    clock = pygame.time.Clock()

    while not done:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                done = True

        # Clear the screen
        screen.fill(WHITE)

        '''n.rect.x = 100
        n.rect.y = 100
        n.colour = ((n.colour[0]+1)%255, n.colour[1], n.colour[2],)
        n.image.fill(n.colour)'''

        # Update all the sprites
        ecosystem.all_sprites_list.update()

        # Draw all the spites
        ecosystem.all_sprites_list.draw(screen)

        # Go ahead and update the screen with what we've drawn.
        pygame.display.flip()

        # Limit to 60 frames per second
        clock.tick(60)

    # halt() must be called before closing the serial
    # port in order to ensure proper thread shutdown
    zb.halt()
    ser.close()

    pygame.quit()

if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(description='Visualize the testbed.')
    parser.add_argument('port', metavar='P', type=str, help='The port that the XBee is connected on.')

    args = parser.parse_args()

    main(args)
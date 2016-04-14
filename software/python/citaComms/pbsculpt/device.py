

class DeviceModule(object):
    '''
    '''
    def __init__(self, parent, port):
        self.parent = parent
        self.port = port
        self.peripherals = {}

    def loop(self):
        for address in self.peripherals:
            self.peripherals[address].loop()

        self.reflex()

    def reflex(self):
        sensor = 0
        for peripheral in filter(lambda p: p.is_sensor, self.peripherals):
            sensor = peripheral.value

        for peripheral in filter(lambda p: p.is_actuator, self.peripherals):
            peripheral.value = sensor

    def add(self, peripheral):
        self.peripherals[peripheral.address] = peripheral

    @property
    def connection(self):
        return self.parent.connection

# class ProprioceptiveLEDModule(DeviceModule):
#
#     def reflex(self):

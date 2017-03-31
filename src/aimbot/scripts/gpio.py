#----------------------------------------------------------------------
# Gaugette.GPIO from from https://github.com/guyc/py-gaugette
# Guy Carpenter, Clearwater Software
#
# This is an abstraction layer for GPIO calls to isolate the
# differences between the RasperryPi and BeagleBone Black implementations.
#
# On the RPi, we use wiringpi.GPIO
# On the BBB, we use Adafruit_BBIO.GPIO
#
#----------------------------------------------------------------------
import wiringpi

class GPIO:
    def __init__(self):
        self.gpio = wiringpi.GPIO(wiringpi.GPIO.WPI_MODE_PINS)
        self.setup = self.wiringpi_setup
        self.output = self.gpio.digitalWrite
        self.input = self.gpio.digitalRead
        self.trigger = self.gpio.wiringPiISR
        self.OUT = self.gpio.OUTPUT
        self.IN = self.gpio.INPUT
        self.HIGH = self.gpio.HIGH
        self.LOW = self.gpio.LOW
        self.PUD_UP = self.gpio.PUD_UP
        self.PUD_DOWN = self.gpio.PUD_DOWN
        self.PUD_OFF = self.gpio.PUD_OFF
        self.EDGE_FALLING = self.gpio.INT_EDGE_FALLING
        self.EDGE_RISING = self.gpio.INT_EDGE_RISING
        self.EDGE_BOTH = self.gpio.INT_EDGE_BOTH

    #----------------------------------------------------------------------

    # Implement the setup call via the wiringpi API
    def wiringpi_setup(self, channel, direction, pull_up_down=None):
        self.gpio.pinMode(channel, direction)
        if pull_up_down is None:
            pull_up_down = self.gpio.PUD_OFF
        self.gpio.pullUpDnControl(channel, pull_up_down)

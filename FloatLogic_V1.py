#tells people who the authors are! wow :O
'''
	Authors: Graham Wunder
	init create: 9/10/2025
	last updated: 9/10/2025
'''

import enum from Enum, auto
import time
import board
import digitalio
import busio
import adafruit_rfm9x

#rfm9x code, currently placeholder
RADIO_FREQ_MHZ = 915.0
CS = digitalio.DigitalInOut(board.CE1)
RESET = digitalio.DigitalInOut(board.D25)
spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)
rfm9x = adafruit_rfm9x.RFM9x(spi, CS, RESET, RADIO_FREQ_MHZ)

# the radio is configured in LoRa mode so you can't control sync
# word, encryption, frequency deviation, or other settings
# you can however adjust the transmit power (in dB). the default is 13 dB but
# high power radios like the RFM95 can go up to 23 dB
rfm9x.tx_power = 13

#define the LED
LED = digitalio.DigitalInOut(board.D13)
LED.direction = digitalio.Direction.OUTPUT

#create the floatstate enum for tracking the state of the float
class FloatState(Enum):
	SURFACED = (auto)
	MOVING_UP = (auto)
	MOVING_DOWN = (auto)
	FLOORED = (auto)
	MAINTAINING = (auto)
	IDLE = (auto)

#create the motordirecion enum, self explanatory
class MotorDirection(Enum):
	CLOCKWISE = (auto)
	COUNTERCLOCKWISE = (auto)
	STALLED = (auto)

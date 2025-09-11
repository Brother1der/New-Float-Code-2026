#tells people who the authors are! wow :O
'''
	Authors: Graham Wunder, Elliot Fortner
	init create: 9/10/2025
	last updated: 9/10/2025
'''

#imports all necessary modules. very basic python
import enum from Enum, auto #documentation: https://docs.python.org/3/library/enum.html
import time
import board
import digitalio #documentation: https://docs.circuitpython.org/en/latest/shared-bindings/digitalio/
import busio #documentation: https://docs.circuitpython.org/en/latest/shared-bindings/busio/
import adafruit_rfm9x #documentation: https://docs.circuitpython.org/projects/rfm9x/en/latest/

#counts the number of complete data-sending cycles
sendCycleCount = 0

#company name. Idk why we have this but its in the legacy code but i transfered it
COMPANY_NAME = "Team #3"

#this is the maximum size of the packets sent
MAX_LIST_SIZE = 2000

#these are all from last year. prolly needs to be updated for new task.
#tells the float what depths for tolerance and maintaining(?)
MIN_MAINTAIN_DEPTH = 2.4
MAX_MAINTAIN_DEPTH = 2.6
MIN_TOLERANCE = 2.0
MAX_TOLERANCE = 3.0
MAX_MAINTAINS = 10.0

#controls the radio freq. needs to be updated for actual rfm9x mhz
RADIO_FREQ_MHZ = 915.0

# number of seconds between sending
SEND_INTERVAL = 1.5

#interval before updating list
LIST_UPDATER_INTERVAL = 5

#stuff for the pi. tells program which pins things are on(?) will add to wiki later
CS = digitalio.DigitalInOut(board.CE1)
RESET = digitalio.DigitalInOut(board.D25)
spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)
rfm9x = adafruit_rfm9x.RFM9x(spi, CS, RESET, RADIO_FREQ_MHZ)
LED = digitalio.DigitalInOut(board.D13)
LED.direction = digitalio.Direction.OUTPUT

# the radio is configured in LoRa mode so you can't control sync
# word, encryption, frequency deviation, or other settings
# you can however adjust the transmit power (in dB). the default is 13 dB but
# high power radios like the RFM95 can go up to 23 dB
rfm9x.tx_power = 13

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

#time to go global
psiList = [] #creates empty list for the history of pressure readings
depthList = [] #again, creates empty list, this time for history of depth readings
timeList = [] #another empty list, now for time

psi_surface_start = 0.0
has_maintained = False
maintain_updates = 0

send_float = False
first_send = False
initiateCount = 0
sendCycleCount = 0
currentIndex = 0

float_curr_state = FloatState.SURFACED
motor_direction = MotorDirection.STALLED

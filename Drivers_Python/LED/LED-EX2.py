#!/usr/bin/python

"""
__author__ = "D.Qendri"
__copyright__ = "Copyright 2015 Sensorian"
__license__ = "GPL V3"
__version__ = "1.0"
"""

import RPi.GPIO as GPIO
import time

P0 = 12 # LED pin

"""
	Setups the pins in BCM mode
		
		
	:param none: 
	:returns none : 
"""
def Init():
    GPIO.setwarnings(False) # suppress GPIO used message
    GPIO.setmode(GPIO.BOARD) # use BCM pin numbers
    GPIO.setup(P0, GPIO.OUT) # set LED pin as output

"""
	Turns on the LED on port P0
	
	
	:param none: 
	:returns none : 
"""
def LEDon():
    GPIO.output(P0, GPIO.HIGH)

"""
	Turns off the LED on port P0
	
	
	:param none: 
	:returns none : 
"""
def LEDoff():
    GPIO.output(P0, GPIO.LOW)

"""
	Sets the LED state as HIGH or LOW.
	
	
	:param state: state of the LED, can be 1 for HIGH or 0 for LOW.
	:returns none : 
"""
def SetLED(state):
    if state:
        LEDon()
    else:
        LEDoff()


# if not used as a module (standalone), run this test program 
if __name__ == "__main__":
	Init()
	try:
		while(True):
			SetLED(1)
			time.sleep(0.5)
			SetLED(0)
			time.sleep(0.5)
			
	except KeyboardInterrupt:
		print("Clean exit on CTRL-C")
        GPIO.cleanup()

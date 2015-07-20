#!/usr/bin/python

import sys
import time
import FXOS8700CQR1 as imuSens
from ctypes import *

imuSens = imuSens.FXOS8700CQR1()				#Configure chip in hybrid mode

def main(argv):
	id = imuSens.getID()					#Verify chip ID
	print "Chip ID: 0x%02X. \r\n" % id	
	imuSens.standbyMode()	
	imuSens.activeMode()
		
	id = imuSens.getChipMode()			#Configure sensor as accelerometer
	print "Chip Mode: 0x%02X. \r\n" % id
		
	id = imuSens.getTemperature()
	print "Chip Temperature: 0x%02X. \r\n" % id

	def L1():
		print "Landscape right.\r\n"
		
	def L2():
		print "Landscape left. \r\n"
			
	def P1():
		print "Portrait down.\r\n"
			
	def P2():
		print "Portrait up.\r\n" 
		
	options = {0 : L1,
				1 : L2,
				2 : P1,
				3 : P2
			}

	imuSens.configureAccelerometer()
	imuSens.configureMagnetometer()
	imuSens.configureOrientation();
	modeprevious = 0

	while True:
		if(imuSens.readStatusReg() & 0x80):	
			orienta = imuSens.getOrientation()
			mode = (orienta >> 1) & 0x03	
			
			if (mode != modeprevious):
				options[mode]()
				
			modeprevious = mode
			
	print "Shutting down"	


if __name__ == '__main__':
    try:
        main(sys.argv[1:])
    except KeyboardInterrupt:
        print("")
		

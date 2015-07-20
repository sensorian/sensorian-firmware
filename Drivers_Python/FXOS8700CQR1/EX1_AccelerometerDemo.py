#!/usr/bin/python
import sys
import time
import smbus
import FXOS8700CQR1 as imuSens
from ctypes import *


imuSens = imuSens.FXOS8700CQR1()				#Configure chip in hybrid mode


def main():	
	id = imuSens.getID()					#Verify chip ID
	print "Chip ID: 0x%02X. \r\n" % id
	imuSens.standbyMode()	
	# imuSens.writeByte(0x0B,0x01)		#Set to wake up 	
	imuSens.activeMode()
		
	id = imuSens.getChipMode()			#Configure sensor as accelerometer
	print "Chip Mode: 0x%02X. \r\n" % id
		
	id = imuSens.getTemperature()
	print "Chip Temperature: 0x%02X. \r\n" % id
		
	#id = imuSens.readByte(PL_BF_ZCOMP)
	#print "PL_BF_ZCOMP: 0x%02X. \r\n" % id
		
	imuSens.configureAccelerometer()
	imuSens.configureMagnetometer()
	while True:
		if(imuSens.readStatusReg() & 0x80):
			x,y,z = imuSens.pollAccelerometer()
			print "Accelerometer data x: %d, y: %d, z: %d \r\n"  % (x, y, z)
			# mx,my,mz = imuSens.pollMagnetometer()
			# print "Magnetometer data mx: %d, my: %d, mz: %d \r\n" % (mx, my, mz)
	print "Shutting down"
	
if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print("")


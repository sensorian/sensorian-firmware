#!/usr/bin/python

import sys
import math
import time
import FXOS8700CQR1 as imuSens
from ctypes import *

def pitchAndroll(ax, ay, az):
	# pitch = math.atan2(ax / (math.sqrt(ay*ay + az*az)))
	# roll = math.atan2 (ay / (math.sqrt(ax*ax + az*az)))
	pitch = math.atan2(ax , (math.sqrt(ay*ay + az*az)))
	roll = math.atan2 (-ay, -az)
	return pitch,roll
	
	
def wrap(angle):
	if angle > math.pi:
		angle -= (2*math.pi)
	if angle < -math.pi:
		angle += (2*math.pi)
	if angle < 0:
		angle += 2*math.pi
	return angle

def mag_to_tilt_compensated_heading(bx, by, bz, phi, theta):
	""" 
	Takes in raw magnetometer values, pitch and roll and turns it into a tilt-compensated 
	heading value ranging from -pi to pi (everything in this function should be in radians). 
	"""
	variation = 4.528986*(math.pi/180) 
	Xh = bx * math.cos(theta) + by * math.sin(phi) * math.sin(theta) + bz * math.cos(phi) * math.sin(theta)
	Yh = by * math.cos(phi) - bz * math.sin(phi)
	return wrap((math.atan2(-Yh, Xh) + variation))


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
		
		
	imuSens.configureAccelerometer()
	imuSens.configureMagnetometer()

	while True:
		if(imuSens.readStatusReg() & 0x80):
			x,y,z = imuSens.pollAccelerometer()
			print "Accelerometer data x: %d, y: %d, z: %d \r\n"  % (x, y, z)
			pitch,roll  = pitchAndroll(x,y,z)
			
			print "pitch %f, roll %f \r\n" % (pitch,roll)
			mx,my,mz = imuSens.pollMagnetometer() 
			print "Magnetometer data mx: %d, my: %d, mz: %d \r\n" % (mx, my, mz)
			angle = mag_to_tilt_compensated_heading(mx, my, mz,pitch,roll)
			print "Angle:  %f" % angle			
			print "Heading :  %f" % imuSens.getHeading()
			
	print "Shutting down"	

if __name__ == '__main__':
    try:
        main(sys.argv[1:])
    except KeyboardInterrupt:
        print("")
		

#!/usr/bin/python

import Image
import TFT as display
import sys
import time
import FXOS8700CQR1 as imuSens
from ctypes import *

sensor = CDLL("./libFXO.so")				#Load the low level library
sensor.I2C_Initialize(imuSens.FXOS8700CQR1_ADDRESS)	#Initialize I2C and BCM library

imuSens = imuSens.FXOS8700CQR1()				#Configure chip in hybrid mode
imuSens.standbyMode()	
imuSens.activeMode()

graphicDisplay = display.TFT()
graphicDisplay.initialize()					# Initialize display.
image1 = Image.open('logoscreen.jpg')

def L1():
	print "Landscape left. \r\n"
	image = image1.rotate(90).resize((128, 160))
	graphicDisplay.display(image)
	
def L2():
	print "Landscape right.\r\n"
	image = image1.rotate(270).resize((128, 160))
	graphicDisplay.display(image)

def P1():
	print "Portrait up.\r\n" 
	image = image1.rotate(180).resize((128, 160))
	graphicDisplay.display(image)
	
def P2():
	print "Portrait down.\r\n"
	image = image1.rotate(0).resize((128, 160))
	graphicDisplay.display(image)
		

	
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

 # /****************************************************************************
 # * Copyright (C) 2015 Sensorian
 # *                                                                          *
 # * This file is part of Sensorian.                                          *
 # *                                                                          *
 # *   Sensorian is free software: you can redistribute it and/or modify it   *
 # *   under the terms of the GNU Lesser General Public License as published  *
 # *   by the Free Software Foundation, either version 3 of the License, or   *
 # *   (at your option) any later version.                                    *
 # *                                                                          *
 # *   Sensorian is distributed in the hope that it will be useful,           *
 # *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 # *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 # *   GNU Lesser General Public License for more details.                    *
 # *                                                                          *
 # *   You should have received a copy of the GNU Lesser General Public       *
 # *   License along with Sensorian.                                          *
 # *   If not, see <http://www.gnu.org/licenses/>.                            *
 # ****************************************************************************/
 
 # /**
 # * @file P06-Dynamic Photo Frame/PictureViewer.py
 # * @author D.Qendri
 # * @date 15 July 2014
 # * @brief Picture Viewer reponds to orientation changes.   
 # */

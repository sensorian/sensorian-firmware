#!/usr/bin/python

import time 
from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont
import TFT as display
import APDS9300 as LuxSens
import math

print "=== Ambient light demo test  ===" 
print "********************************"

THRESHOLD = 5

def main():

	AmbientLight = LuxSens.APDS9300()					#Setup Ambient light sensor 
	
	height = display.TFT_HEIGHT
	width = display.TFT_WIDTH	
	graphicDisplay = display.TFT()	
	graphicDisplay.initialize()				# Initialize display.
	graphicDisplay.clear()					# clear screen 
	LampON = Image.open('Lamp-On.jpg')
	LampOff = Image.open('Lamp-Off.jpg')
	
	print 'Resizing...'
	LampON = LampON.rotate(90).resize((128, 160),Image.ANTIALIAS)	# Resize the image and rotate it so it's 128x160 pixels.
	LampOff = LampOff.rotate(90).resize((128, 160),Image.ANTIALIAS)	# Resize the image and rotate it so it's 128x160 pixels.
	print 'Resizing finished.'
	
	
	while True:
		channel1 = AmbientLight.readChannel(1)				#Take a reading from channel one
		channel2 = AmbientLight.readChannel(0)				#Take a reading from channel two
		Lux = AmbientLight.getLuxLevel(channel1,channel2)
			
		if(Lux > THRESHOLD):
			graphicDisplay.clear()
			graphicDisplay.display(LampON)
		else:
			graphicDisplay.clear()
			graphicDisplay.display(LampOff)

if __name__=="__main__":
    main()
    time.sleep(2)

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
 # * @file P01-LuxMeter/App_IntelliLamp.py
 # * @author D.Qendri
 # * @date 15 Jan 2015
 # * @brief Intelligent lamp responds to ambient lamp.   
 # */
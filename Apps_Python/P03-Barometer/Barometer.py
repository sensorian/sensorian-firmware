#!/usr/bin/python
import Image
import ImageDraw
import ImageFont
import time
import os, sys
import sys
import TFT as GLCD
import MPL3115A2 as altibar
from ctypes import *


def main(argv):
	print "========= MPL3115A2 Sensor demo ==========" 
	print "******************************************" 
	
	AltiBar = altibar.MPL3115A2()				#initialize sensor
	AltiBar.ActiveMode()	#puts sensor in active mode
	AltiBar.BarometerMode()	#puts sensor in active mode

	height = GLCD.TFT_HEIGHT
	width = GLCD.TFT_WIDTH

	disp = GLCD.TFT()		# Create TFT LCD display class.
	disp.initialize()		# Initialize display.
	disp.clear()			# Alternatively can clear to a black screen by calling:
	draw = disp.draw()		# Get a PIL Draw object to start drawing on the display buffer
	font = ImageFont.truetype('/usr/share/fonts/truetype/freefont/FreeSansBold.ttf', 14)		# use a truetype font

	
	
	while True:
		press =  AltiBar.ReadBarometricPressure()		#Take a pressure reading
		time.sleep(1)
		draw.text((30, 10), "Pressure (Pa)", font=font)
		draw.text((30, 30), str(press), font=font)		
		disp.display()
		time.sleep(5)
		disp.clear()


if __name__ == '__main__':
    try:
        main(sys.argv[1:])
    except KeyboardInterrupt:
        print("")
		
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
 # * @file P02-Barometer/Barometer.py
 # * @author D.Qendri
 # * @date 13 Sep 2014
 # * @brief Barometer demo.   
 # */
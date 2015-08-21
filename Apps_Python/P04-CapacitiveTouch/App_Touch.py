import Image
import ImageDraw
import ImageFont
import TFT as GLCD
import sys
import smbus
import time
import CAP1203 as capTouch
from ctypes import *

disp = GLCD.TFT()		# Create TFT LCD display class.
disp.initialize()		# Initialize display.
disp.clear()			# Alternatively can clear to a black screen by calling:
draw = disp.draw()		# Get a PIL Draw object to start drawing on the display buffer
font = ImageFont.truetype('/usr/share/fonts/truetype/freefont/FreeSansBold.ttf', 14)		# use a truetype font

RED 	= 0x00ff00
BLUE	= 0xff0000
GREEN	= 0x0000ff
WHITE	= 0xffffff

def PILColorToRGB(pil_color):
    """ convert a PIL-compatible integer into an (r, g, b) tuple """
    hexstr = '%06x' % pil_color
    # reverse byte order
    r, g, b = hexstr[4:], hexstr[2:4], hexstr[:2]
    r, g, b = [int(n, 16) for n in (r, g, b)]
    return (r, g, b)
	
def drawCircle(x,y,r,fill=(0,0,255),outline=(0,255,0)):
	draw.ellipse((x-r, y-r, x+r, y+r), fill,outline)		# Draw a blue ellipse with a green outline.
	
def main():

	CapTouch  = capTouch.CAP1203()
	CapTouch.activeMode()

	id = CapTouch.readID()							#Read ID of Capacitive touch controller 
	print "Chip ID: 0x%04X. \r\n" % id
	time.sleep(0.5)
	
	button = 0x00

	def B1():
		disp.clear(PILColorToRGB(WHITE))
		drawCircle(80,35,20,fill=PILColorToRGB(RED),outline=PILColorToRGB(WHITE))
		disp.display()
		
	def B2():
		disp.clear(PILColorToRGB(WHITE))
		drawCircle(80,80,20,fill=PILColorToRGB(BLUE),outline=PILColorToRGB(WHITE))
		disp.display()
			
	def B3():
		disp.clear(PILColorToRGB(WHITE))
		drawCircle(80,125,20,fill=PILColorToRGB(GREEN),outline=PILColorToRGB(WHITE))
		disp.display()
			
	def NoPress():
		disp.clear(PILColorToRGB(WHITE))
		drawCircle(80,35,20,fill=PILColorToRGB(WHITE),outline=PILColorToRGB(RED))
		drawCircle(80,80,20,fill=PILColorToRGB(WHITE) , outline=PILColorToRGB(BLUE))
		drawCircle(80,125,20,fill=PILColorToRGB(WHITE),outline=PILColorToRGB(GREEN))
		disp.display()
		
	options = { 0 : NoPress,
				1 : B3,
				3 : B1,
				2 : B2
			}
	modeprevious = 190
	mode = 0

	while True:
		button = CapTouch.readPressedButton()
		if (button != 190):
			#print "Button B%d pressed.\r\n"  % int(button)		#For debugging
			if(int(button) != 109):
				mode = int(button)
			else:
				mode  = 0				
			if (mode != modeprevious):
				options[mode]()
		
		disp.clear(PILColorToRGB(WHITE))
		


if __name__=="__main__":
	main()

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
 # * @file P03-CapacitiveTouch/App_Touch.py
 # * @author D.Qendri
 # * @date 15 July 2014
 # * @brief Capacitive Touch demo.   
 # */
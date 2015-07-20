#!/usr/bin/python
import sys
import smbus
import time
import CAP1203 as touch

NoPress = 0

def main():
	print "Capacitive touch demo"
	CapTouch  = touch.CAP1203()

	
	button = 0x00

	id = CapTouch.readID()							#Read ID of Capacitive touch controller 
	print "Chip ID: 0x%04X. \r\n" % id

	while (True):
		button = NoPress							#reset button
		button = CapTouch.readPressedButton()		#poll for new press
		if (button != NoPress):
			print "Button B%d pressed.\r\n"  % int(button)
			time.sleep(0.8)
			
	
if __name__=="__main__":
	try:
		main()
	except KeyboardInterrupt:
		print("...Quitting ...")



#!/usr/bin/python

from PIL import Image
import TFT as display
import time

graphicDisplay = display.TFT()							# Initialize display.
graphicDisplay.initialize()

image1 = Image.open('SleepyCat.jpg')
image2 = Image.open('raspberry_pi.jpg')

while True:
	print 'Loading image...'							# Load an image.
	image1 = image1.rotate(90).resize((128, 160))		# Resize the image and rotate it so it's 240x320 pixels.
	print 'Drawing image'
	graphicDisplay.display(image1)						# Draw the image on the display hardware.
	time.sleep(3)
	
	print 'Loading image...'							# Load another image.
	image2 = image2.rotate(90).resize((128, 160))		# Resize the image and rotate it so it's 240x320 pixels.
	print 'Drawing image'
	graphicDisplay.display(image2)						# Draw the image on the display hardware.
	time.sleep(3)
	

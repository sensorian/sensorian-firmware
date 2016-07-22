#!/usr/bin/python

from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont
import time
import TFT as GLCD

disp = GLCD.TFT()		# Create TFT LCD display class.
disp.initialize()		# Initialize display.
disp.clear()			# Alternatively can clear to a black screen by calling:
draw = disp.draw()		# Get a PIL Draw object to start drawing on the display buffer

height = GLCD.TFT_HEIGHT
width = GLCD.TFT_WIDTH

while True:
	disp.clear()
	draw.ellipse((30, 30, 60, 80), outline=(0,255,0), fill=(0,0,255))				# Draw a blue ellipse with a green outline.
	draw.rectangle((70, 90, 100, 130), outline=(255,255,0), fill=(255,0,255))		# Draw a purple rectangle with yellow outline.
	draw.line((110, 30, 10, 30), fill=(255,255,255))								# Draw a white X.
	draw.line((10, 120, 120, 10), fill=(255,255,255))
	disp.display()
	
	time.sleep(3)
	disp.clear()
	draw.rectangle((0,0,GLCD.TFT_WIDTH,GLCD.TFT_HEIGHT), outline=0, fill=0)			#Draw a black filled box to clear the image.
	
	draw.rectangle((40, 50, 60, 40), outline=255, fill=1)							# Draw a rectangle.
	draw.polygon([(50, 10), (80+20/2, 90), (90+10, 70)], outline=255, fill=0)		# Draw a triangle.

	disp.display()
	time.sleep(3)

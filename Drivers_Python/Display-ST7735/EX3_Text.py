#!/usr/bin/python

from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont
import time
import TFT as GLCD

def draw_rotated_text(image, text, position, angle, font, fill=(255,255,255)):
	draw = ImageDraw.Draw(image)									# Get rendered font width and height.
	width, height = draw.textsize(text, font=font)	
	textimage = Image.new('RGBA', (width, height), (0,0,0,0))		# Create a new image with transparent background to store the text.
	textdraw = ImageDraw.Draw(textimage)							# Render the text.
	textdraw.text((0,0), text, font=font, fill=fill)
	rotated = textimage.rotate(angle, expand=1)						# Rotate the text image.
	image.paste(rotated, position, rotated)							# Paste the text into the image, using it as a mask for transparency.
	

def main():
	height = GLCD.TFT_HEIGHT
	width = GLCD.TFT_WIDTH

	disp = GLCD.TFT()		# Create TFT LCD display class.
	disp.initialize()		# Initialize display.
	disp.clear()			# Alternatively can clear to a black screen by calling:
	draw = disp.draw()		# Get a PIL Draw object to start drawing on the display buffer
	var = 0
	
	while True:
		var += 1
		font = ImageFont.truetype('/usr/share/fonts/truetype/freefont/FreeSansBold.ttf', 14)		# use a truetype font
		draw.text((30, 80), "TFT DEMO", font=font)
		draw.text((40, 100), str(var), font=font)
		disp.display()
		time.sleep(1)
		disp.clear()

		# Move left to right keeping track of the current x position for drawing shapes.
		draw.text((30, 30),    'Sensorian ',  font=font, fill=255)		# Write two lines of text.
		draw.text((40, 50), 'Shield!', font=font, fill=255)
		disp.display()		
		time.sleep(1)
		disp.clear()

if __name__=="__main__":
    main()

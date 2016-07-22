from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont
import TFT as GLCD
import textwrap

LINE_SPACING = 12
LINES_VERTICAL = 10
LINES_HORIZONTAL = 9

height = GLCD.TFT_HEIGHT
width = GLCD.TFT_WIDTH

disp = GLCD.TFT()		# Create TFT LCD display class.
disp.initialize()		# Initialize display.
disp.clear()			# Alternatively can clear to a black screen by calling:
draw = disp.draw()		# Get a PIL Draw object to start drawing on the display buffer
font = ImageFont.truetype('/usr/share/fonts/truetype/freefont/FreeSansBold.ttf', 14) # use a truetype font

def draw_rotated_text(draw, text, position, angle, font, fill=(255,255,255)):
	# Get rendered font width and height.
	#draw = ImageDraw.Draw(image)
	width, height = draw.textsize(text, font=font)
	# Create a new image with transparent background to store the text.
	textimage = Image.new('RGBA', (width, height), (0,0,0,0))
	# Render the text.
	textdraw = ImageDraw.Draw(textimage)
	textdraw.text((0,0), text, font=font, fill=fill)
	# Rotate the text image.
	rotated = textimage.rotate(angle, expand=1)
	# Paste the text into the image, using it as a mask for transparency.
	draw.bitmap(position,rotated)


def screen_print(text):
	line_length = 15
	text = textwrap.wrap(text,line_length)
	if len(text) > LINES_VERTICAL:
		text = text[0:LINES_VERTICAL]
	disp.clear()
	for i in range(0,len(text)):
		draw.text((0,LINE_SPACING*i), text[i], font=font)
	disp.display()


""" Prints text to the TFT-LCD display based on the specified
orientation.
Orientations:
	0: Text is printed so that it is readable with the SD card pointing
	upwards and the screen facing the user.
	
	1: RPi + Sensorian is in direction of +Z towards the user, +Y
		towards ceiling, and +X towards the user's right hand.
		Capacitive touch button texts 'B1', 'B2', 'B3' are in their
		normal reading orientation.
"""
def screen_print_rotated(text,orientation):
	if orientation == 0:
		screen_print(text)
	
	elif orientation == 1:
		draw_position = (0,160)
		draw_angle = 90
		line_length = 20
		text = textwrap.wrap(text,line_length)
		if len(text) > LINES_HORIZONTAL:
			text = text[0:LINES_HORIZONTAL]
		disp.clear()
		for i in range(0,len(text)):
			t_width, t_height = draw.textsize(text[i], font=font)
			draw_rotated_text(draw,text[i],(draw_position[0]+LINE_SPACING*i,draw_position[1]-t_width),draw_angle,font)
		disp.display()

__author__ = "Michael Lescisin"
__copyright__ = "Copyright Sensorian 2015"

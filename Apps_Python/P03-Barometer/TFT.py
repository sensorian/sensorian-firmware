#!/usr/bin/python

"""
__author__ = "D.Qendri"
__copyright__ = "Copyright 2015 Sensorian"
__license__ = "GPL V3"
__version__ = "1.0"
"""

import numbers
import time
from PIL import Image
from PIL import ImageDraw
import RPi.GPIO as GPIO
import spidev as spi

spi = spi.SpiDev()

#Loosely based on some snippets from Adafruit display driver.

# Constants for interacting with display registers.
TFT_WIDTH    = 128		 # Screen Width
TFT_HEIGHT   = 160		 # Scree Height

NOP         = 0x00
SWRESET     = 0x01
RDDID       = 0x04
RDDST       = 0x09
RDDPM       = 0x0A
RDDMADCTL   = 0x0B		# Read Display MADCTL
RDDCOLMOD   = 0x0C		# Read Display Pixel Format
RDDIM       = 0x0D		# Read Display Image Mode
RDDSM       = 0x0E		# Read Display Signal Mode
RDDSDR      = 0x0F		# Read Display Self-Diagnostic Result
SLEEP_IN    = 0x10
SLEEP_OUT   = 0x11
PTLON       = 0x12		# Partial Display Mode On
NORON       = 0x13		# Normal Display Mode On
INVOFF      = 0x20
INVON       = 0x21		# Display Inversion On
GAMSET      = 0x26		# Gamma Set
DISPOFF     = 0x28
DISPON      = 0x29
CASET       = 0x2A
RASET       = 0x2B
RAMWR       = 0x2C
RGBSET      = 0x2D		#Color set 
RAMRD       = 0x2E
PTLAR       = 0x30
SCRLAR      = 0x33		# Scroll Area Set 
TEOFF       = 0x34
TEON        = 0x35
MADCTL      = 0x36
VSCSAD      = 0x37		# Vertical Scroll Start Address of RAM
IDLE_MODE_OFF   = 0x38
IDLE_MODE_ON    = 0x39
COLMOD      = 0x3A		# Interface Pixel Format
RDID1       = 0xDA
RDID2       = 0xDB
RDID3       = 0xDC		# Read ID3 Value
FRMCTR1     = 0xB1		# Frame Rate Control (In normal mode/ Full colors) 
FRMCTR2     = 0xB2		# Frame Rate Control (In Idle mode/ 8-colors)
FRMCTR3     = 0xB3		# Frame Rate Control (In Partial mode/ full colors)
INVCTR      = 0xB4		# Display Inversion Control
DISSET5     = 0xB6 
PWCTR1      = 0xC0
PWCTR2      = 0xC1
PWCTR3      = 0xC2
PWCTR4      = 0xC3
PWCTR5      = 0xC4
VMCTR1      = 0xC5
VMOFCTR     = 0xC7
WRID2       = 0xD1
WRID3       = 0xD2
NVFCTR1     = 0xD9      # NVM Control Status
NVFCTR2     = 0xDE
NVFCTR3     = 0xDF
GMCTRP1     = 0xE0
GMCTRN1     = 0xE1
GCV         = 0xFC
DUMMY       = 0xff

#####################################################

BLACK       = 0x0000
BLUE        = 0x001F
RED         = 0xF800
GREEN       = 0x07E0
CYAN        = 0x07FF
MAGENTA     = 0xF81F
YELLOW      = 0xFFE0  
WHITE       = 0xFFFF

#####################################################

MADCTL_MY  = 0x80
MADCTL_MX  = 0x40
MADCTL_MV  = 0x20
MADCTL_ML  = 0x10
MADCTL_MH  = 0x04

MADCTL_M1  = 0x00
MADCTL_M2  = 0b10000000
MADCTL_M3  = 0b01000000
MADCTL_M4  = 0b11000000
MADCTL_M5  = 0b00100000
MADCTL_M6  = 0b10100000
MADCTL_M7  =  0b01100000
MADCTL_M8  =  0b11100000

MADCTL_RGB  = 0x00
MADCTL_BGR  = 0x08

GAMMA1 = 0x01
GAMMA2 = 0x02
GAMMA3 = 0x04
GAMMA4 = 0x08

PIXEL12BIT = 0x03
PIXEL16BIT = 0x05
PIXEL18BIT = 0x06
#####################################################

DC = 22
RST = 16
TFT_CS = 24

ON = 1
OFF = 0


def color565(r, g, b):
	"""
	Convert red, green, blue components to a 16-bit 565 RGB value. 
	Components should be values 0 to 255.
	
	
	:param r: Red byte.
	:param g: Green byte.
	:param b: Blue byte.
	:returns pixel : 16-bit 565 RGB value
	"""
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def image_to_data(image):
	"""Generator function to convert a PIL image to 16-bit 565 RGB bytes.
	
	
	:param image: PIL image
	:returns imgArray : 
	"""
	pixels = image.convert('RGB').load()
	width, height = image.size
	for y in range(height):
		for x in range(width):
			r,g,b = pixels[(x,y)]
			color = color565(r, g, b)
			yield (color >> 8) & 0xFF
			yield color & 0xFF


class TFT(object):
	"""FW Driver for an ST7735S TFT controller."""

	def __init__(self):
		"""
		Creates a TFT object and configures SPI bus and pins.
		
		
		:param none: 
		:returns none :
		"""
		self.dc = DC
		self.rst = RST
		self.cs = TFT_CS
		self.width = TFT_WIDTH
		self.height = TFT_HEIGHT
		
		spi.open(0,1)             		#will open bus 0, CE1. 
		#spi.max_speed_hz = 8000000
		spi.max_speed_hz = int(16000000)
		
		GPIO.setwarnings(False)
		GPIO.setmode(GPIO.BOARD)	
		GPIO.setup(self.cs, GPIO.OUT)		#Set GPIO24 as output
		GPIO.setup(self.dc, GPIO.OUT)		# Set DC as output.
		GPIO.setup(self.rst, GPIO.OUT)	
		self.CE_DESELECT()
		self.buffer = Image.new('RGB', (self.width, self.height))	# Create an image buffer.
	
	def CE_OUTPUT(self):
		"""
		Set TFT CS pin as output
		
		
		:param none: 
		:returns: none
		"""
		GPIO.setup(self.cs, GPIO.OUT)	#Set GPIO24 as output
			
	def CE_SELECT(self):
		"""
		Select TFT CS pin.
		
		
		:param none: 
		:returns: none
		"""
		GPIO.output(self.cs, 0)
			
	def CE_DESELECT(self):
		"""
		Deselect TFT CS pin.
		
		
		:param none: 
		:returns: none
		"""
		GPIO.output(self.cs, 1)

	def send(self, data, dataOrCmd=True, length=4096):
		"""
		Writes a byte or array of bytes to the display. 
		dataOrCmd parameter controls if byte should be interpreted as display data (True)/ command data otherwise.  
		Length is an optional size of bytes to write in a single SPI transaction, with a default of 4096. 
		
		
		:param data: Single byte or an array of bytes.
		:param dataOrCmd: Flag for command or data mode
		:param length: size of array
		:returns none :
		"""
		# Set DC low for command, high for data.
		GPIO.output(self.dc, dataOrCmd)
		# Convert scalar argument to list so either can be passed as parameter.
		if isinstance(data, numbers.Number):
			data = [data & 0xFF]
		# Write data a chunk at a time.
		for start in range(0, len(data), length):
			end = min(start+length, len(data))
			spi.writebytes(data[start:end])

	def command(self, data):
		"""
		Write a byte or array of bytes to the display as command data.
		
		
		:param data: Single byte command.
		:returns none :
		"""
		self.CE_SELECT()	
		self.send(data, False)
		self.CE_DESELECT()	

	def data(self, data):
		"""
		Write a byte or array of bytes to the display as display data.
		
		
		:param data: Data byte
		:returns none :
		"""
		self.CE_SELECT()	
		self.send(data, True)
		self.CE_DESELECT()	

	def reset(self):
		"""
		Resets the display, (RST) reset pin must be connected.
		
		
		:param none: 
		:returns none :
		"""
		if self._rst is not None:
			GPIO.output(self.rst,1)
			time.sleep(0.005)
			GPIO.output(self.rst,0)
			time.sleep(0.02)
			GPIO.output(self.rst,1)
			time.sleep(0.150)

	def initialize(self):
		"""
		Intializes the display controller and prepares the it for any subsequent operations. 
		
		
		:param none: 
		:returns none :
		"""
	
		GPIO.setup(self.dc, GPIO.OUT)
		GPIO.setup(self.rst, GPIO.OUT)
		GPIO.setup(TFT_CS, GPIO.OUT)
		
		GPIO.output(self.dc, 0)
		GPIO.output(self.rst, 1)
		
		self.command(SWRESET) 
		time.sleep(0.015)
		self.command(SLEEP_OUT) 
		time.sleep(0.60)

		self.command(FRMCTR1) 			# Set ST7735S Frame Rate
		self.data(0x01) 
		self.data(0x2C) 
		self.data(0x2D) 
		self.command(FRMCTR2) 
		self.data(0x01) 
		self.data(0x2C) 
		self.data(0x2D) 
		self.command(FRMCTR3) 
		self.data(0x01) 
		self.data(0x2C) 
		self.data(0x2D) 
		self.data(0x01) 
		self.data(0x2C) 
		self.data(0x2D) 
		
		self.command(INVCTR)  		# Display Inversion Control
		self.data(0x07) 
		self.command(PWCTR1) 
		self.data(0xA2) 
		self.data(0x02) 
		self.data(0x84) 
		self.command(PWCTR2) 		
		self.data(0XC5) 
		self.command(PWCTR3) 		# Power Control 3 (in Normal mode/ Full colors)
		self.data(0x0A) 
		self.data(0x00) 
		self.command(PWCTR4) 		# Power Control 4 (in Idle mode/ 8-colors)
		self.data(0x8A) 
		self.data(0x2A) 
		self.command(PWCTR5) 
		self.data(0x8A) 
		self.data(0xEE) 
		
		self.command(VMCTR1)  # ST7735S Power Sequence
		self.data(0x0E) 
		self.command(INVOFF) 
		self.command(MADCTL)  #MX, MY, RGB mode
		self.data(0xC0) 
		self.command(COLMOD)  #65k mode
		self.data(0x05) 
		
		self.command(CASET) 
		self.data(0x00) 
		self.data(0x7F) 
		self.command(RASET) 
		self.data(0x00) 
		self.data(0x9F) 
		
		self.command(GMCTRP1) 			# ST7735S Gamma Sequence
		self.data(0x02) 
		self.data(0x1c) 
		self.data(0x07) 
		self.data(0x12) 
		
		self.data(0x37) 
		self.data(0x32) 
		self.data(0x29) 
		self.data(0x2d) 
		
		self.data(0x29) 
		self.data(0x25) 
		self.data(0x2B) 
		self.data(0x39) 
		
		self.data(0x00) 
		self.data(0x01) 
		self.data(0x03) 
		self.data(0x10) 
		  
		self.command(GMCTRN1) 
		self.data(0x03) 
		self.data(0x1d) 
		self.data(0x07) 
		self.data(0x06) 
		
		self.data(0x2E) 
		self.data(0x2C) 
		self.data(0x29) 
		self.data(0x2D) 
		
		self.data(0x2E) 
		self.data(0x2E) 
		self.data(0x37) 
		self.data(0x3F) 
		
		self.data(0x00) 
		self.data(0x00) 
		self.data(0x02) 
		self.data(0x10) 

		self.command(NORON)         # Normal display on, no args, w/delay 10ms 0x13
		time.sleep(0.100) 			# End ST7735S Gamma Sequence
		self.command(DISPON)  		# Display on
		

	def setAddrWindow(self, x0=0, y0=0, x1=None, y1=None):
		"""
		Set the pixel address window for proceeding drawing commands. 
		
		
		:param x0: x0 and x1 should define the minimum and maximum x pixel bounds.
		:param y0:
		:param x1: y0 and y1 should define the minimum and maximum y pixel bound.
		:param y1:
		:returns none :
		"""
		if x1 is None:
			x1 = self.width-1
		if y1 is None:
			y1 = self.height-1
		self.command(CASET)		# Column addr set
		self.data(x0 >> 8)
		self.data(x0)					# XSTART 
		self.data(x1 >> 8)
		self.data(x1)					# XEND
		self.command(RASET)		# Row addr set
		self.data(y0 >> 8)
		self.data(y0)					# YSTART
		self.data(y1 >> 8)
		self.data(y1)					# YEND
		self.command(RAMWR)		# write to RAM

	def display(self, image=None):
		"""
		Write the provided image to the hardware. If no image parameter is provided the display buffer will be written to the hardware.  
		If an image is provided, it should be RGB format and the same dimensions as the display hardware.
		
		
		:param image: picture image
		:returns none :
		"""
		# By default write the internal buffer to the display.
		if image is None:
			image = self.buffer
		# Set address bounds to entire display.
		self.setAddrWindow()
		# Convert image to array of 16bit 565 RGB data bytes.
		# Unfortunate that this copy has to occur, but the SPI byte writing
		# function needs to take an array of bytes and PIL doesn't natively
		# store images in 16-bit 565 RGB format.
		pixelbytes = list(image_to_data(image))
		# Write data to hardware.
		self.data(pixelbytes)

	def clear(self, color=(0,0,0)):
		"""
		Clear the image buffer to the specified RGB color (default black).
		
		
		:param color: Background color. 
		:returns none :
		"""
		width, height = self.buffer.size
		self.buffer.putdata([color]*(width*height))

	def draw(self):
		"""
		Return a PIL ImageDraw instance for 2D drawing on the image buffer.
		
		
		:param none: 
		:returns none :
		"""
		return ImageDraw.Draw(self.buffer)

	def invert(self,status):
		"""Disables color inversion on the display.
		
		
		:param status:  Color inversion status.
		:returns none :
		"""
		if (status == False):
			self.command(INVOFF)
		else:
			self.command(INVON)
	
	def setRotation(self,mode):	
		"""Sets the display text orientation. Mirrored modes are 
			also supported on top of portrait and landscape modes.
			
			
		:param mode: orientation data   
		:returns none :
		"""
		self.command(MADCTL)
		if (mode == 0x00):
			 self.data(MADCTL_MY | MADCTL_MX| MADCTL_BGR)	#portrait
		elif (mode == 0x01):
			 self.data(MADCTL_MV |MADCTL_ML| MADCTL_BGR)	#Landscape mode reflected 
		elif (mode == 0x02):
			 self.data(MADCTL_MY | MADCTL_BGR)				#Portrait mode reflected 
		elif (mode == 0x03):
			 self.data(MADCTL_MX | MADCTL_BGR)				#Portarit mode inverted and reflected
		elif (mode == 0x04):
			 self.data(MADCTL_MV | MADCTL_BGR | MADCTL_MX)	#Landscape mode inverted 
		elif (mode == 0x05):
			 self.data(MADCTL_ML | MADCTL_BGR)				#Portrait inverted 
		elif (mode == 0x06):
			 self.data(MADCTL_MV|MADCTL_MY | MADCTL_BGR)	#Landscape mode 
		else :
			 self.data(MADCTL_M8 | MADCTL_BGR)				#Landscape mode reflected and inverted

	def setGamma(self,gamma):
		"""
		Sets the  gamma mode of the display.
		
		
		:param gamma:  Is from 1 to 4.
		:returns none :
		"""
		self.command(NORON)
		if(gamma == 1):
			self.data(GAMMA1)
		elif(gamma == 2):
			self.data(GAMMA2)
		elif(gamma == 3):
			self.data(GAMMA3)
		else:
			self.data(GAMMA4)
			 

	def setPartialArea(self,start,end):
		"""
		Sets the  partial scroll area. This function is used in conjuction with the scrollArea function.
		
		
		:param start:  start of patial area
		:param end:  end of patial area
		:returns none :
		"""
		self.command(PTLON)
		self.data(start)				# start row
		self.data(start >> 8)
		self.data(end)					# end row
		self.data(end >> 8)
		self.command(PTLON)
	
	
	def scrollArea(self,tfa,vsa,bfa,firstline):
		"""
		Sets the  scroll area.
		
		
		:param tfa:  top fixed area row
		:param vsa:  vertical scrolling area
		:param bfa:  bottom fixed area row
		:param firstline: Beginning of  first line
		:returns none :
		"""
		self.command(SCRLAR)
		self.data(tfa)				# start row
		self.data(tfa >> 8)
		self.data(vsa)					# end row
		self.data(vsa >> 8)
		self.data(bfa)					# end row
		self.data(bfa >> 8)
		self.command(VSCSAD)
		self.data(0x00)
		self.data(firstline)				# which line in the Frame Memory will be written as the first line after the last line of the Top Fixed Area
		
		
	def fullDisplay(self):
		"""
		Sets the  display in full screen mode.
		
		
		:param none: 
		:returns none :
		"""
		self.command(NORON)

	def setColorMode(slef,mode):
		"""
		Sets the  pixel color depth. This can be 12,16 or 18 bit.
		
		
		:param mode: Constant number
		:returns none :
		"""
		self.command(COLMOD)
		self.data(mode)
		
	def	sleep(self):
		"""
		Return a PIL ImageDraw instance for 2D drawing on the image buffer.
		
		
		:param none: 
		:returns none :
		"""
		self.command(SLEEP_IN)
		time.sleep(0.005)

	def wakeUp(self):
		"""
		Wakes the display from sleep mode.
		
		
		:param none: 
		:returns none :
		"""
		self.command(SLEEP_OUT)
		time.sleep(0.120)

	def idleMode(self,onoff):
		"""
		Puts the display or takes it out of idle mode 
		
		
		:param onoff: idle mode status
		:returns none :
		"""
		if(onoff == 0):
			self.command(SIDLE_MODE_OFF)
		else:
			self.command(SIDLE_MODE_ON)
		
	def turnOff(self):
		"""
		Blanks out the display.
		
		
		:param none: 
		:returns none :
		"""
		self.command(DISPOFF)

	def turnOn(self):
		"""
		This function turn on the display from idle mode.
		
		
		:param none: 
		:returns none :
		"""
		self.command(DISPON)

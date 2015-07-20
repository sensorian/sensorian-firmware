#!/usr/bin/python
import sys
import smbus
import time
bus = smbus.SMBus(1)

"""
__author__ = "D.Qendri"
__copyright__ = "Copyright 2015 Sensorian"
__license__ = "GPL V3"
__version__ = "1.0"
"""

APDS9300ADDR = 0x29					#Address of ambient light sensor

#Register definitions
CONTROL           = 0x00			#Control of basic functions
TIMING            = 0x01
THRESHLOWLOW      = 0x02
THRESHLOWHIGH     = 0x03
THRESHHIGHLOW     = 0x04
THRESHHIGHHIGH    = 0x05
INTERRUPT         = 0x06			#Interrupt control 
CRC               = 0x08
ID                = 0x0A
DATA0LOW          = 0x0C
DATA0HIGH         = 0x0D
DATA1LOW          = 0x0E
DATA1HIGH         = 0x0F

#COMMAND REG
COMMAND           = 0x80
CMD_CLEAR_INT     = 0x40
CMD_WORD          = 0x20

#CONTROL REG
POWERON 		  = 0x03
POWEROFF          = 0x00

#Timing Reg
GAIN              = 0x10
MANUAL            = 0x08
INTEG13_7MS       = 0x00
INTEG101MS        = 0x01
INTEG402MS        = 0x02

#INTERRUPT REG
INTERR_THRSHOLD   = 0x01
INTERR_DIS        = 0x00
INTERR_EN         = 0x10

class APDS9300(object):
	""" 
	Ambient Light Sensor. Class functions for the APDS9300 ambient light sensor. 
	"""
	
	def __init__(self):
		"""
		Powers on the sensor 
		
		
		:param none: 
		:returns: none
		"""
		self._address = APDS9300ADDR
		self.setpowerState(POWERON)				#Power on sensor
		self.setGain(1)
		self.setSamplingTime(0x02)

	def setpowerState(self,state):
		"""
		Configures the power state of the chip.
		
		
		:param state:  Denotes the power state of the sensor ,set to 0/1 to turn off/on.
		:returns: none
		"""
		self.writeRegister(CONTROL|CMD_CLEAR_INT|CONTROL,state)		#Turn on or off the sensor

	def setGain(self,gain):
		"""
		Set the gain of the internal photodiode sensors, can be 1x or 16x.
		
		
		:param gain: Gain type can be GAIN_1 or GAIN_16 for gain of 1 or 16x
		:returns: none
		"""
		value = 0
		if (gain == 1):
			value  = self.readByte(COMMAND|TIMING)      #Write to TIMING register
			value &= 0b11101111
			self.writeByte(COMMAND|TIMING)       		#Write to TIMING register
			self.writeByte(value)
		elif (gain == 16):
			self.writeByte(COMMAND|TIMING)				#Write to TIMING register
			self.writeByte(GAIN)
		else :
			self.writeByte(COMMAND|TIMING)				#Write to TIMING register
			self.writeByte(GAIN)

	def setSamplingTime(self,sampling_time):
		"""
		Configure the sampling time of the sensor.
		
		
		:param sampling_time: Can be 0x00,0x01,0x02
		:returns: none
		"""
		self.writeByte(TIMING|COMMAND)
		self.writeByte(sampling_time)

	def readChannel(self,channel):
		"""
		Read raw data from channel 0 or channel 1
		
		
		:param channel: Value of 0 or 1.
		:returns data : Channel data as raw number.
		"""
		if(channel == 1):
			self.writeByte(COMMAND|CMD_CLEAR_INT|CMD_WORD|DATA0LOW)
			channel = self.readWord(0x00)
		else :
			self.writeByte(COMMAND|CMD_CLEAR_INT|CMD_WORD|DATA1LOW)
			channel = self.readWord(0x00)
		return channel

	def getLuxLevel(self,ch0, ch1):
		"""
		Returns LUX output from the raw sensor data.
		
		
		:param ch0: Channel 0 value 
		:param ch1: Channel 1 value 
		:returns: Lux Float value of light intensity
		"""
		k = 0
		if(ch0 !=0):
			k = float(ch1/ch0)
			
		Lux=0

		if((k>=0) and (k<=0.52)):
			Lux=(0.0315*ch0)-(0.0593*ch0*pow(k,1.4))
		elif( (k>0.52) and (k<=0.65)):
			Lux=(0.0229*ch0)-(0.0291*ch1)
		elif((k>0.65) and (k<=0.80)): 
			Lux=(0.0157*ch0)-(0.0180*ch1)
		elif((k>0.80) and (k<=1.30)):
			Lux=(0.00338*ch0)-(0.00260*ch1)
		else :
			Lux=0
		return Lux
		
	def setIntLowThreshold(self,lowvalue):
		"""
		Set the low level data threshold for the interrupt.
		
		
		:param lowvalue: Low interrupt threshold value 
		:returns: none
		"""
		self.writeWord((THRESHLOWLOW|COMMAND|CMD_WORD),lowvalue)	#use the write word protocol to write these 16 bit values

	def setIntHighThreshold(self,highvalue):
		"""
		Set the high level data threshold for the interrupt.
		
		
		:param highvalue: High interrupt threshold value 
		:returns: none
		"""
		self.writeWord((THRESHHIGHLOW| COMMAND | CMD_WORD),highvalue)

	def clearInterrupt(self):
		"""
		Clear any active interrupts.
		
		
		:param: none
		:returns: none 
		"""
		self.writeByte(COMMAND|CMD_CLEAR_INT)			#Set clear bit to 1

	def setInterruptState(self,enable=False):
		"""
		Enable or disable the interrupts on the sensor.
		
		
		:param enable: Enables or diable interrupt. Default is False.
		:returns: none 
		"""
		if enable == True:
			self.writeRegister(COMMAND|INTERRUPT,INTERR_EN)
		else:
			self.writeRegister(COMMAND|INTERRUPT,INTERR_DIS)

	def chipID(self):
		"""
		Read the unique chip ID.
		
		
		:param none: 
		:returns: partID ID of chip
		"""
		partID = self.readByte(COMMAND|ID)
		return partID
		
	def writeByte(self,data):
		"""
		Write a byte value. Assumes the register pointer is predefined.
		
		
		:param data: Data byte to write on the I2C bus 
		:returns: none
		"""
		bus.write_byte(self._address,data)
		return 0

	def writeRegister(self,reg,data):
		"""
		Write a byte value to a register.
		
		
		:param reg: Register  address
		:param data: Data byte to be written on register. 
		:returns: none
		"""
		bus.write_byte_data(self._address,reg,data)

	def writeWord(self,reg, data):
		"""
		Write a word value to a register.
		
		
		:param reg: Register  address
		:param data: Data word to be written on the register. 
		:returns: none
		"""
		buffer = [(data >> 8) & 0xFF, data & 0xFF]
		bus.write_i2c_block_data(self._address, reg, buffer)

	def readByte(self,reg):
		"""
		Read a byte value from a register.
		
		
		:param reg: Register  address
		:returns: Register byte content
		"""
		x = bus.read_byte_data(self._address, reg)
		return x

	def readWord(self,reg):
		"""
		Read a word value from a register.
		
		
		:param reg: Register  address
		:returns: Register word content
		"""
		x = bus.read_word_data(self._address, reg)
		return x
		
#!/usr/bin/python

import sys
import smbus
import time
bus = smbus.SMBus(1)

APDS9300ADDR = 0x29

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
	'''Class for APDS9300 ambient light sensor'''
	
	def __init__(self):
		'''Initial configuration of the sensor'''
		self._address = APDS9300ADDR
		self.setpowerState(POWERON)				#Power on sensor	
		self.readByte(COMMAND | CONTROL)		#Value read should be 0x13

	def setpowerState(self,state):
		'''Set the power state of the chip , 0/1 to turn off/on '''
		cmd  = POWERON if state else POWERON		
		self.writeRegister(COMMAND | CONTROL,cmd)				#Turon on or off the sensor

	def setGain(self,gain):
		'''Sets the gain of the photodiode sensors, can be 1 or 16'''
		value = 0
		if (gain == 1):
			self.writeByte(TIMING|COMMAND|GAIN)
		elif gain == 16:
			value  = self.readByte(TIMING)
			value &= ~GAIN
			self.writeByte(TIMING|COMMAND|value)
		else :
			self.writeByte(TIMING|COMMAND|GAIN)
		return val

	def setSamplingTime(self,sampling_time):
		'''Set the sampling time of the sensor'''
		self.writeByte(TIMING|COMMAND|sampling_time)

	def readChannel(self,channel):
		'''Read the raw data from channel 0 or channel 1'''
		if(channel == 1):
			self.writeByte(COMMAND|CMD_CLEAR_INT|CMD_WORD|DATA0LOW)
			channel = self.readWord(0x00)
		else :
			self.writeByte(COMMAND|CMD_CLEAR_INT|CMD_WORD|DATA1LOW)
			channel = self.readWord(0x00)
		return channel

	def getLuxLevel(self,ch0, ch1):
		'''Convert the raw data to a LUX value'''
		Lux=0
		
		try:
			k = float(ch1/ch0)
		except ZeroDivisionError:
			k = 0

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
		'''Set the low level threshold for the interrupt'''
		self.writeWord((THRESHLOWLOW|COMMAND|CMD_WORD),lowvalue)	#use the write word protocol to write these 16 bit values

	def setIntHighThreshold(self,highvalue):
		'''Set the high level threshold for the interrupt'''
		self.writeWord((THRESHHIGHLOW| COMMAND | CMD_WORD),highvalue)

	def clearInterrupt(self):
		'''Clear any interrupts'''
		self.writeByte(COMMAND|CMD_CLEAR_INT)			#Set clear bit to 1

	def setInterruptState(self,enable=False):
		'''Enable or disable the interrupts on the sensor'''
		if enable == True:
			self.writeRegister(COMMAND|INTERRUPT,INTERR_EN)
		else:
			self.writeRegister(COMMAND|INTERRUPT,INTERR_DIS)

	def chipID(self):
		'''Read the chip unique ID'''
		partID = self.readByte(COMMAND|ID)
		return partID
		
	def writeByte(self,data):
		'''Write a byte value '''
		bus.write_byte(self._address,data)
		return 0

	def writeRegister(self,reg,data):
		'''Write a byte value to a register'''
		bus.write_byte_data(self._address,reg,data)

	def writeWord(self,reg, data):
		'''Write a word value to a register'''
		buffer = [(data >> 8) & 0xFF, data & 0xFF]
		bus.write_i2c_block_data(self._address, reg, buffer)

	def readByte(self,reg):
		'''Read a byte value from a register'''
		x = bus.read_byte_data(self._address, reg)
		return x

	def readWord(self,reg):
		'''Read a word from a register'''
		x = bus.read_word_data(self._address, reg)
		return x
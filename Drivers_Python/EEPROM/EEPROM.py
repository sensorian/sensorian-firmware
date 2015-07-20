#!/usr/bin/python

"""
__author__ = "D.Qendri"
__copyright__ = "Copyright 2015 Sensorian"
__license__ = "GPL V3"
__version__ = "1.0"
"""

import sys
import smbus
import time

bus = smbus.SMBus(1)

EEPROMaddress = 0x57	  # EEPROM address

EEPROM_WRITE  = 0xae      #  DEVICE ADDR for EEPROM (writes)   
EEPROM_READ  = 0xaf       #  DEVICE ADDR for EEPROM (reads)  
RTCC_WRITE   = 0xde       #  DEVICE ADDR for RTCC MCHP  (writes) 
RTCC_READ    = 0xdf       #  DEVICE ADDR for RTCC MCHP  (reads)
SRAM_PTR     = 0x20       #  pointer of the SRAM area (RTCC) 
EEPROM_SREG  = 0xff       #  STATUS REGISTER in the  EEPROM

BP0    =  0x04
BP1    =  0x08

class PROTECTION_t:
	"""Class type for EEPROM protection settings."""
	QUARTER = 1
	HALF = 2
	ALL = 3

class EEPROM(object):
	"""Class object of the MCP79410 EEPROM chip."""
	
	EEPROM_START = 0x00
	EEPROM_END   = 0x7F
	EEPROM_PROTECTED_START 	= 0xF0
	EEPROM_PROTECTED_END    = 0xF7

	EEPROM_SREG  = 0xff       #  STATUS REGISTER in the  EEPROM

	def __init__(self):
		"""
		Initializes the EEPROM object.
		
		
		:param none: 
		:returns none:
		"""
		self._address = EEPROMaddress		
		
	def EEPROM_Write(self,mem_address,ee_data):
		"""
		Write a single byte in the I2C EEPROM.
		
		
		:param mem_address:  Memory register to write to.
		:returns ee_data: Byte data to write.
		"""
		self.writeRegister(mem_address,ee_data)
		
	def EEPROM_WriteProtected(self,mem_address,ee_data):
		"""
		Writes to the special protected EEPROM space.
		
		
		:param mem_address:  Memory register to write to.
		:returns ee_data: Byte data to write.
		"""
		self.writeRegister(mem_address,ee_data)
		
	def EEPROM_Read(self,mem_address):
		"""
		The function reads a data byte from the EEPROM.
		
		
		:param mem_address: Address of register.
		:returns reg: Register content.
		"""
		return self.readRegister(mem_address)
			
	def EEPROM_SetBlockProtection(self,protSetting):
		"""
		This function sets the EEPROM Block protection settings.
		
		
		:param mem_address: EEPROM address to write to.
		:returns reg: Page buffer.
		"""
		self.writeRegister(EEPROM_SREG,protSetting) 
		
	def EEPROM_ReadPage(self,mem_address):
		"""
		This function reads a single page from the RTCC EEPROM.
		
		
		:param mem_address: EEPROM address to write to
		:returns: buffer: Page buffer
		"""
		buffer = bus.read_i2c_block_data(self._address,mem_address,8)
		return buffer
		
	def EEPROM_WritePage(self,mem_address,buffer):
		"""
		This function writes a single page to the RTCC EEPROM.
		
		
		:param mem_address: EEPROM address to write to
		:returns: buffer: Page buffer
		"""
		bus.write_i2c_block_data(self._address,mem_address,buffer)							#Enable clock

	def writeRegister(self,rtcc_reg,dat):
		"""
		Write a new value on the register.
		
		
		:param rtcc_reg: Address of the register.
		:param dat:  Byte value to be written on the register.
		:returns: none
		"""
		bus.write_byte_data(self._address, rtcc_reg, dat)

	def readRegister(self,rtcc_reg):
		"""
		Read the value of the register.
		
		
		:param rtcc_reg: Address of the register.
		:returns: Register byte content 
		"""
		result = bus.read_byte_data(self._address, rtcc_reg) & 0xFF
		return result
		

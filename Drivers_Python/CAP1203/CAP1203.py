#!/usr/bin/python


__author__ = "D.Qendri"
__copyright__ = "Copyright 2015 Sensorian"
__license__ = "GPL V3"
__version__ = "1.0"


import sys
import time
import smbus
from ctypes import *

CAP1203ADDR  = 0x28					#Capacitive sensor address
CAP = CDLL("./libCAP.so")

class CAP1203(object):
	'''Capacitive touch controller. Class for CAP1203 touch sensor.'''
	MAIN_CTRL_REG = 0x00
	SENSOR_INPUTS = 0x03

	
	def  __init__(self):
		"""
		Configure sensor for capacitive touch.
		
		
		:param none: 
		:returns: none 
		"""
		self._address = CAP1203ADDR	
		CAP.I2C_Initialize()
		CAP.CAP1203_Initialize()

		
	def  activeMode(self):
		"""
		Set the capacitive controller in active mode.
		
		
		:param none: 
		:returns: none
		"""
		CAP.CAP1203_ActiveMode()

		
	def  standbyMode(self):
		"""
		Set the capacitive controller in standby mode.
		
		
		:param none: 
		:returns: none
		"""
		CAP.CAP1203_StandbyMode()
		
	def  deepSleep(self):
		"""
		Put the capacitive controller in deep sleep mode.
		
		
		:param none: 
		:returns: none
		"""
		CAP.CAP1203_DeepSleep()		

	def resumeFromDeepSleep(self):
		"""
		Take the capacitive controller out of deep sleep mode.
		
		
		:param none: 
		:returns: none
		"""
		CAP1203_ResumeFromDeepSleep()

	def  configureMultiTouch(self,number,mulchan):
		"""
		Configure the capacitve touch for multitouch inputs.
		
		
		:param number: number of simultaneous touches 2 or 3
		:param chan: One of the three channels.
		:returns: none
		"""
		CAP.CAP1203_Write(self._address,MULTITOUCH,number|MULTBLK_EN)      							  #Set number of simultaneous touches
		CAP.CAP1203_Write(self._address,MULTIPATCONF,MTP_EN|MTP_TH1|MTP_TH0|COMP_PTRN|MTP_ALERT)      #Enable multitouch
		CAP.CAP1203_Write(self._address,MULTIPATTERN,mulchan)
		
	def multitouchEvent(self):
		"""
		Return true if a multi-touch event happened.
		
		
		:param none: 
		:returns: true if a multi-touch event happened 
		"""
		return CAP.CAP1203_MultitouchEvent()
		

	def setPowerButton(self,button):
		"""
		Configure the button for power button mode.
		
		
		:param button: One of the three buttons
		:returns: none
		"""
		CAP.CAP1203_SetPowerButton(button)

		
	def	 readPressedButton(self):
		"""
		Read the pressed button, one of buttons 1 to 3.
		
		
		:param none: 
		:returns: key - Button pressed
		"""
		return CAP.CAP1203_ReadPressedButton()

	def getStatusReg(self):
		"""Read the status register.
		
		
		:param none: 
		:returns: status - Contents of status register
		"""
		status = CAP.CAP1203_Read(GEN_STATUS);
		return status;

	def  enableInterrupt(self,pin):
		"""
		Enable the interrupt mode.
		
		
		:param pin: Enables interrupt on the specific pin
		:returns: none
		"""
		CAP.CAP1203_EnableInterrupt(pin)

		
	def  setSensitivity(self,sensitivity):
		"""
		Configure the sensitivity of the controller.
		
		
		:param sensitivity: 
		:returns: none
		"""
		CAP.CAP1203_SetSensitivity(sensitivity)
		
	def  checkSensorStatus(self):
		"""
		Check the current sensor status.
		
		
		:param none: 
		:returns: status - Current chip status
		"""
		CAP.CAP1203_CheckSensorStatus()

		
	def  clearInterrupt(self):
		"""
		Clear any active interrupts.
		
		
		:param none: 
		:returns: none
		"""
		CAP.CAP1203_ClearInterrupt()
		
	def  readID(self):
		"""
		Read the sensor manufacturer ID.
		
		
		:param none: 
		:returns: id - Sensor ID as a number
		"""
		id = CAP.CAP1203_ReadID()
		return id

	def  write(self,address,reg,data):
		"""
		Write a byte to the register.
		
		
		:param address: Sensor address
		:param reg: Register address to write to.
		:param data: Data to be written on byte sized register.
		:returns: none
		"""
		return CAP.CAP1203_Write(address,reg,data)
		
	def  read(self,address,reg):
		"""
		Read the contents of the register.
		
		
		:param address: Sensor address
		:param reg: Register address to read from.
		:returns: data - Data content of register address
		"""
		return CAP.CAP1203_Read(address,reg)


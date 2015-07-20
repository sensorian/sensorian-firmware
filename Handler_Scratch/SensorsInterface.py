#!/usr/bin/env python
# -*- coding: utf-8 -*-

from ctypes import *
import time
import calendar

lib_sensorian = CDLL("./libsensorian.so")

def setupSensorian():
	lib_sensorian.setupSensorian()

def ledOn():
	lib_sensorian.LED_on()

def ledOff():
	lib_sensorian.LED_off()

def getAmbientLight():
	return lib_sensorian.getAmbientLight()


mpl_last_polled = -1

#Ensure Thermometer is not polled too often
def getTemperature():
	global mpl_last_polled
	if mpl_last_polled == -1:
		print "Polling MPL3115A2"
		lib_sensorian.pollMPL()
		mpl_last_polled = time.time()
	
	elif time.time() - mpl_last_polled > 30:
		print "Polling MPL3115A2"
		lib_sensorian.pollMPL()
		mpl_last_polled = time.time()
	
	return lib_sensorian.getTemperature()


#Ensure Altimeter is not polled too often
def getAltitude():
	global mpl_last_polled
	if mpl_last_polled == -1:
		print "Polling MPL3115A2"
		lib_sensorian.pollMPL()
		mpl_last_polled = time.time()
	
	elif time.time() - mpl_last_polled > 30:
		print "Polling MPL3115A2"
		lib_sensorian.pollMPL()
		mpl_last_polled = time.time()
	
	return lib_sensorian.getAltitude()

def getBarometricPressure():
	global mpl_last_polled
	if mpl_last_polled == -1:
		print "Polling MPL3115A2"
		lib_sensorian.pollMPL()
		mpl_last_polled = time.time()
	
	elif time.time() - mpl_last_polled > 30:
		print "Polling MPL3115A2"
		lib_sensorian.pollMPL()
		mpl_last_polled = time.time()
	
	return lib_sensorian.getBarometricPressure()


def getTouchpad():
	return lib_sensorian.CAP1203_ReadPressedButton()


fxos_last_polled = -1
#Ensure Accelerometer is not polled too often
def getAccelerometer():
	global fxos_last_polled
	if fxos_last_polled == -1:
		#Poll
		lib_sensorian.pollFXOS()
		fxos_last_polled = time.time()
	elif time.time() - fxos_last_polled > 1:
		lib_sensorian.pollFXOS()
		fxos_last_polled = time.time()
	
	ac_x = lib_sensorian.getAccelX()
	ac_y = lib_sensorian.getAccelY()
	ac_z = lib_sensorian.getAccelZ()
	return (ac_x, ac_y, ac_z)

#Ensure Magnetometer is not polled too often
def getMagnetometer():
	global fxos_last_polled
	if fxos_last_polled == -1:
		#Poll
		lib_sensorian.pollFXOS()
		fxos_last_polled = time.time()
	elif time.time() - fxos_last_polled > 1:
		lib_sensorian.pollFXOS()
		fxos_last_polled = time.time()
	
	mag_x = lib_sensorian.getMagX()
	mag_y = lib_sensorian.getMagY()
	mag_z = lib_sensorian.getMagZ()
	return (mag_x, mag_y, mag_z)

def getRTCCtime():
	#Poll RTCC
	lib_sensorian.poll_rtcc()
	#Get year,month,date,hour,minute,second
	year = lib_sensorian.get_rtcc_year()
	month = lib_sensorian.get_rtcc_month()
	date = lib_sensorian.get_rtcc_date()
	hour = lib_sensorian.get_rtcc_hour()
	minute = lib_sensorian.get_rtcc_minute()
	second = lib_sensorian.get_rtcc_second()
	return (year,month,date,hour,minute,second)

#Warning: completely ignores 'second' argument.
def setRTCCtime(year,month,date,hour,minute,second):
	#Check that the arguments are reasonable
	if year < 1900:
		print "Invalid year"
		return
	
	if month not in range(1,13):
		print "Invalid month"
		return
	
	if date not in range(1,32):
		print "Invalid date"
		return
	
	if hour not in range(0,24):
		print "Invalid hour"
		return
	
	if minute not in range(0,60):
		print "Invalid minute"
		return
	
	if second not in range(0,60):
		print "Invalid second"
		return
	
	#Get the weekday for the given year, month, date
	w_day = calendar.weekday(year,month,date)
	#Set this on the Sensorian RTCC
	lib_sensorian.set_rtcc_datetime(((year+1900)-2000) % 100, month, date,w_day, hour, minute ,0)


def setRTCCalarm(year,month,date,hour,minute,second,mode):
	#Check that the arguments are reasonable
	if year < 1900:
		print "Invalid year"
		return
	
	if month not in range(1,13):
		print "Invalid month"
		return
	
	if date not in range(1,32):
		print "Invalid date"
		return
	
	if hour not in range(0,24):
		print "Invalid hour"
		return
	
	if minute not in range(0,60):
		print "Invalid minute"
		return
	
	if second not in range(0,60):
		print "Invalid second"
		return
	
	#Get the weekday for the given year, month, date
	w_day = calendar.weekday(year,month,date)
	#Set this on the Sensorian RTCC alarm
	lib_sensorian.set_rtcc_alarm(((year+1900)-2000) % 100, month, date,w_day, hour, minute ,second,mode)

def pollRTCCalarm():
	return lib_sensorian.poll_rtcc_alarm()

def resetRTCCalarm():
	lib_sensorian.reset_alarm()


__author__ = "Michael Lescisin"
__copyright__ = "Copyright Sensorian 2015"

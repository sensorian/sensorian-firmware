#!/usr/bin/python
import Image
import ImageDraw
import ImageFont
import time
import os, sys
import sys
import TFT as GLCD
import MPL3115A2 as altibar
from ctypes import *

HOSTNAME = 'smtp-mail.outlook.com'
USERNAME = ''
PASSWORD = ''
RECEPIENT_ADDRESS = ''
SUBJECT = 'Subject:Sensor data.\n'

""" Get email credentials
	:param none: 
	:returns none : 
"""
def LoginCredentials():
	USERNAME = raw_input('Enter your email address: ')
	PASSWORD = raw_input('Enter your email password: ')
	RECEPIENT_ADDRESS = raw_input('Enter the recepient address: ')
	return USERNAME,PASSWORD,RECEPIENT_ADDRESS
	
	
def main():

	AltiBar = altibar.MPL3115A2()				#initialize sensor
	AltiBar.ActiveMode()						#puts sensor in active mode
	AltiBar.BarometerMode()						#puts sensor in active mode
	time.sleep(2)
	
	press =  AltiBar.ReadBarometricPressure()	#Take a pressure reading

	EMAIL_BODY = SUBJECT + 'Barometric Pressure: ' + str(press) + " Pa.\r\n"
	
	server = smtplib.SMTP(HOSTNAME, 587)
	server.ehlo()
	server.starttls()
	server.login(USERNAME, PASSWORD)	
	server.sendmail(USERNAME, [RECEPIENT_ADDRESS],EMAIL_BODY)
	server.quit()
	
	print "Email sent. Logging Out..." 


if __name__ == '__main__':
    try:
		print 'To exit the program press Ctrl-C.'
		USERNAME,PASSWORD,RECEPIENT_ADDRESS = LoginCredentials()
		main()
    except KeyboardInterrupt:
		print 'Quitting...'
		GPIO.cleanup()

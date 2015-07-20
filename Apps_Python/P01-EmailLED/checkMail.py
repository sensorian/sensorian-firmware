#!/usr/bin/env python
from imapclient import IMAPClient
import time
import RPi.GPIO as GPIO

HOSTNAME = 'imap-mail.outlook.com'
MAILBOX = 'Inbox'
USERNAME = ''
PASSWORD = ''
NEWMAIL_OFFSET = 1   
MAIL_CHECK_PERIOD = 60 # check mail every 60 seconds
SENSORIAN_LED = 12 	 # LED pin
already_loggedIn = False

"""Setup the pins in BCM mode
	:param none: 
	:returns none : 
"""
def LED_Setup():
    GPIO.setwarnings(False) # suppress GPIO used message
    GPIO.setmode(GPIO.BOARD) # use BCM pin numbers
    GPIO.setup(SENSORIAN_LED, GPIO.OUT) # set LED pin as output

"""Turns on the LED on port SENSORIAN_LED
	:param none: 
	:returns none : 
"""
def LED_On():
    GPIO.output(SENSORIAN_LED, GPIO.HIGH)

"""Turns off the LED on port SENSORIAN_LED
	:param none: 
	:returns none : 
"""
def LED_Off():
    GPIO.output(SENSORIAN_LED, GPIO.LOW)

"""Sets the LED state as HIGH or LOW
	:param state: state of the LED, can be 1 for HIGH or 0 for LOW.
	:returns none : 
"""
def SetLED(state):
    if state:
        LED_On()
    else:
        LED_Off()

""" Get email credentials
	:param none: 
	:returns none : 
"""
def LoginCredentials():
	USERNAME = raw_input('Enter your email address: ')
	PASSWORD = raw_input('Enter your email password: ')
	return USERNAME,PASSWORD
	
""" Check for new emails
	:param none: 
	:returns none : 
"""
def CheckEmail():
	server = IMAPClient(HOSTNAME, use_uid=True, ssl=True)
	server.login(USERNAME, PASSWORD)
	
	print('Logging in as ' + USERNAME)
	select_info = server.select_folder(MAILBOX)
	print('%d messages in INBOX' % select_info['EXISTS'])
 
	folder_status = server.folder_status(MAILBOX, 'UNSEEN')
	newmails = int(folder_status['UNSEEN'])
 
	print "You have", newmails, "new emails!"
 
	if newmails >= NEWMAIL_OFFSET:
		LED_On()
	else:
		LED_Off()
		
	time.sleep(MAIL_CHECK_FREQ)

if __name__ == "__main__":
	LED_Setup()
	LED_Off()
	if(already_loggedIn == False):
		USERNAME, PASSWORD = LoginCredentials()
		already_loggedIn = True
		
	try:
		print 'To exit the program press Ctrl-C.'
		while True:
			CheckEmail()
	except KeyboardInterrupt:
		GPIO.cleanup()
		
__author__      = "Dhimiter Qendri"
__copyright__   = "Copyright Sensorian 2015"


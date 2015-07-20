#!/usr/bin/python
import time 
import sys
import RPi.GPIO as GPIO
import APDS9300 as LuxSens

print "=== Ambient light asynchronous interrupt demo  ===" 
print "********************************" 

AmbientLight = LuxSens.APDS9300()					#Setup Ambient light sensor 
print "Chip ID: 0x%02X. \r\n" % AmbientLight.chipID()

INT_PIN = 17

# handle the button event
def AmbientLightEventHandler (pin):
	print "Interrupt from Ambient Light sensor"
	GPIO.output(25,True)		    # turn the green LED on
	time.sleep(1)				    # turn the green LED on   
	GPIO.output(25,False)			# turn the green LED off
	
# main function
def main():

	GPIO.setmode(GPIO.BCM)				  				# Use the chip's pin numbering scheme
	GPIO.setup(INT_PIN,GPIO.IN)						    # Setup pin 23 as an input
	GPIO.add_event_detect(INT_PIN,GPIO.FALLING)			#When interrupt light sensor pin goes high take care of the event
	GPIO.add_event_callback(INT_PIN,AmbientLightEventHandler)
	
	AmbientLight.clearInterrupt()			#Clear interrupts
	AmbientLight.setGain(1)			#Sets gain to 1

	while True: 	
		time.sleep(1)
		channel1 = AmbientLight.readChannel(1)			#Take a reading from channel one
		print "Channel 1 value: %d." % channel1		
		channel2 = AmbientLight.readChannel(0)			#Take a reading from channel two
		print "Channel 2 value: %d" % channel2
		Lux = AmbientLight.getLuxLevel(channel1,channel2)
		print "Lux output: %d." % Lux

	GPIO.cleanup()	
	
if __name__=="__main__":
	try:
		main()
	except KeyboardInterrupt:
		print("")


#!/usr/bin/python
import time 
import APDS9300 as LuxSens
import sys

def main(argv):
	print "=== Ambient light demo test  ===" 
	print "********************************"
	AmbientLight = LuxSens.APDS9300()					#Setup Ambient light sensor 
	id = AmbientLight.chipID()
	print "Chip ID: 0x%02X. \r\n" % id

	while True: 	
		time.sleep(1)
		channel1 = AmbientLight.readChannel(1)				#Take a reading from channel one
		print "Channel 1 value: %d." % channel1		
		channel2 = AmbientLight.readChannel(0)				#Take a reading from channel two
		print "Channel 2 value: %d" % channel2
		Lux = AmbientLight.getLuxLevel(channel1,channel2)
		print "Lux output: %d." % Lux

	

if __name__ == '__main__':
    try:
        main(sys.argv[1:])
    except KeyboardInterrupt:
        print("")
		

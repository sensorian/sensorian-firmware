#!/usr/bin/python

import sys
import time
import MPL3115A2 as altibar
from ctypes import *

	
def main(argv):
	print "=== MPL3115A2 Sensor demo ===" 
	print "******************************************" 
	AltiBar = altibar.MPL3115A2()				#initialize sensor
	AltiBar.ActiveMode()	#puts sensor in active mode
	id  = AltiBar.GetID()		#Verify chip id
	print "Chip ID: %02x" % id
	print "MPL3115A2 Altimeter/Barometer/Temperature sensor demo.\r\n"
	AltiBar.StandbyMode()
	time.sleep(1)
	AltiBar.ActiveMode()	#puts sensor in active mode
	while True:   
		temp = AltiBar.ReadTemperature()		#Take a temperature reading
		print "Temperature : %3.4f degree Celsius.\r\n" %  temp
		time.sleep(1)
		

if __name__ == '__main__':
    try:
        main(sys.argv[1:])
    except KeyboardInterrupt:
        print("")
		

#!/usr/bin/python
import sys
import smbus
import time
import MCP79410RTCC as rtc

def main(argv):
	print "=== Real Time Clock and Calendar  ===" 
	print "*************************************" 
	RTC = rtc.MCP79410()			#create RTCC object

	while True: 
		rtcTime = RTC.GetTime() 			#get time
		tmr = "Time is: " + str(rtcTime.hour) + ":" + str(rtcTime.min)+ ":" + str(rtcTime.sec)  	#convert to string and print it
		print tmr	
		dat = "Date is: " + str(rtcTime.year) + "/" + str(rtcTime.month)+ "/" + str(rtcTime.date)  	#convert to string and print it
		print dat	
		time.sleep(1)

if __name__ == '__main__':
    try:
        main(sys.argv[1:])
    except KeyboardInterrupt:
        print("")
		

__author__      = "Dhimiter Qendri"
__copyright__   = "Copyright June 2015"
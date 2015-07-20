#!/usr/bin/python
import sys
import smbus
import time
import MCP79410RTCC as rtc

def main(argv):
	print "=== Real Time Clock Baackup Battery mode  ===" 
	print "*************************************" 

	RTC = rtc.MCP79410()			#create RTCC object

	if(RTC.IsVBatEnabled()):
		print "Battery mode Enabled.\r\n"
	else:
		print "Battery mode Disabled. Enabling ..\r\n"
		RTC.MCP79410_EnableVbat()
		
	powerDown_Time = RTC.GetPowerDownTime()
	print "Power down time: " + str(powerDown_Time.month) + ":" + str(powerDown_Time.date) + ":" + str(powerDown_Time.hour) + ":" + str(powerDown_Time.min)
		
	powerUp_Time = RTC.GetPowerUpTime()
	print "Power up time: " + str(powerUp_Time.month) + ":" + str(powerUp_Time.date) + ":" + str(powerUp_Time.hour) + ":" + str(powerUp_Time.min)
		
	if(RTC.CheckPowerFailure() == True):
		print "There was a power failure.\r\n"
	else:
		print "No power failure.\r\n"

	while True: 
		rtcTime = RTC.GetTime() 			#get time
		tmr = "Time is: " + str(rtcTime.hour) + ":" + str(rtcTime.min)+ ":" + str(rtcTime.sec)  	#convert to string and print it
		print tmr
		time.sleep(1)
		

if __name__ == '__main__':
    try:
        main(sys.argv[1:])
    except KeyboardInterrupt:
        print("")
		
__author__      = "Dhimiter Qendri"
__copyright__   = "Copyright June 2015"
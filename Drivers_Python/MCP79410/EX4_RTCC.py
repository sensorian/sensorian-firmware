#!/usr/bin/python
import sys
import smbus
import time
import MCP79410RTCC as rtc

print "=== Real Time Clock and Calendar  ===" 
print "*************************************" 

RTC = rtc.MCP79410()			#create RTCC object, initialize RTCC with system time and date 
Alarm = rtc.Alarm()
Match = rtc.Match()
Polarity = rtc.Polarity()
Mode = rtc.Mode()

RTC.ClearInterruptFlag(Alarm.ZERO)

alarm_time = rtc.RTCC_Struct(0, 59, 0, 0, 0, 0, 0)

RTC.SetAlarmTime(alarm_time,Alarm.ZERO)				#Set alarm time
RTC.SetAlarmMatch(Match.MINUTES_MATCH,Alarm.ZERO)			#Alarm ZERO will trigger on minutes match
RTC.SetAlarmMFPPolarity(Polarity.LOWPOL,Alarm.ZERO)			#Configure Alarm pin polarity as HIGH
RTC.SetMFP_Functionality(Mode.ALARM_INTERRUPT)		#Set alaram interrupt
	
pval = RTC.ReadPinStatus(MPL_PIN);
print "MPL pin level is %d\r\n" % pval

while True: 

	s = RTC.GetAlarmStatus(Alarm.ZERO);		#Check alarm status 
	print ("Alarm interrupt status %d\r\n" % int(s))
	if(s != 0):		
		#Read alarm pin
		pval = ReadPinStatus(MPL_PIN)		# This works only when the RTCC operates from battery input. Otherwise the MPF pin will read 0 despite the 
		print "MPL pin level is %d\r\n" % pval	#fact that the interrupt will be active
		time.sleep(1)

	rtcTime = RTC.GetTime() 			#get time
	tmr = "Time is: " + str(rtcTime.hour) + ":" + str(rtcTime.min)+ ":" + str(rtcTime.sec)  	#convert to string and print it
	print tmr	
	time.sleep(1)
	
__author__      = "Dhimiter Qendri"
__copyright__   = "Copyright June 2015"
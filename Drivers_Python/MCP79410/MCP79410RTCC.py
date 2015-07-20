#!/usr/bin/python

"""
__author__ = "D.Qendri"
__copyright__ = "Copyright 2015 Sensorian"
__license__ = "GPL V3"
__version__ = "1.0"
"""

import sys
import smbus
import time

bus = smbus.SMBus(1)

MCP79410address = 0x6f 	  # SRAM board address 
MCP79410EEPROMaddress = 0x57	  # EEPROM address

EEPROM_WRITE  = 0xae      #  DEVICE ADDR for EEPROM (writes)   
EEPROM_READ  = 0xaf       #  DEVICE ADDR for EEPROM (reads)  
RTCC_WRITE   = 0xde       #  DEVICE ADDR for RTCC MCHP  (writes) 
RTCC_READ    = 0xdf       #  DEVICE ADDR for RTCC MCHP  (reads)
SRAM_PTR     = 0x20       #  pointer of the SRAM area (RTCC) 
EEPROM_SR    = 0xff       #  STATUS REGISTER in the  EEPROM

SEC          = 0x00       #  address of SECONDS      register 
MIN          = 0x01       #  address of MINUTES      register 
HOUR         = 0x02       #  address of HOURS        register 
DAY          = 0x03       #  address of DAY OF WK    register 
STAT         = 0x03       #  address of STATUS       register 
DATE         = 0x04       #  address of DATE         register  
MNTH         = 0x05       #  address of MONTH        register 
YEAR         = 0x06       #  address of YEAR         register 
CTRL         = 0x07       #  address of CONTROL      register 
CAL          = 0x08       #  address of CALIB        register 
ULID         = 0x09       #  address of UNLOCK ID    register

ALM0SEC      = 0x0a       #  address of ALARM0 SEC   register 
ALM0MIN      = 0x0b       #  address of ALARM0 MIN   register 
ALM0HR       = 0x0c       #  address of ALARM0 HOUR  register 
ALM0WDAY	= 0x0d       #  address of ALARM0 CONTR register
ALM0DAT      = 0x0e       #  address of ALARM0 DATE  register 
ALM0MTH      = 0x0f       #  address of ALARM0 MONTH register

ALM1SEC      = 0x11       #  address of ALARM1 SEC   register 
ALM1MIN      = 0x12       #  address of ALARM1 MIN   register 
ALM1HR       = 0x13       #  address of ALARM1 HOUR  register 
ALM1WDAY      = 0x14       #  address of ALARM1 CONTR register
ALM1DAT      = 0x15       #  address of ALARM1 DATE  register 
ALM1MTH      = 0x16       #  address of ALARM1 MONTH register

PWRDNMIN     = 0x18       #  address of T_SAVER MIN(VDD->BAT)
PWRDNHOUR    = 0x19       #  address of T_SAVER HR (VDD->BAT) 
PWRDNDATE    = 0x1a       #  address of T_SAVER DAT(VDD->BAT) 
PWRDNMTH     = 0x1b       #  address of T_SAVER MTH(VDD->BAT)

PWRUPMIN     = 0x1c       #  address of T_SAVER MIN(BAT->VDD)
PWRUPHOUR    = 0x1d       #  address of T_SAVER HR (BAT->VDD) 
PWRUPDATE    = 0x1e       #  address of T_SAVER DAT(BAT->VDD) 
PWRUPMTH     = 0x1f       #  address of T_SAVER MTH(BAT->VDD)

##################GLOBAL CONSTANTS RTCC - INITIALIZATION##################

PM              =  0x20       #  post-meridian bit (HOUR)
HOUR_FORMAT     =  0x40       #  Hour format
OUT_PIN         =  0x80       #  = b7 (CTRL)
SQWEN            =  0x40      #  SQWE = b6 (CTRL)
ALM_NO          =  0x00       #  no alarm activated        (CTRL)
ALM_0           =  0x10       #  ALARM0 is       activated (CTRL)
ALM_1           =  0x20       #  ALARM1 is       activated (CTRL)
ALM_01          =  0x30       #  both alarms are activated (CTRL)
MFP_01H         =  0x00       #  MFP = SQVAW(01 HERZ)      (CTRL)
MFP_04K         =  0x01       #  MFP = SQVAW(04 KHZ)       (CTRL)
MFP_08K         =  0x02       #  MFP = SQVAW(08 KHZ)       (CTRL)
MFP_32K         =  0x03       #  MFP = SQVAW(32 KHZ)       (CTRL)
MFP_64H         =  0x04       #  MFP = SQVAW(64 HERZ)      (CTRL)

ALMx_POL        =  0x80       #  polarity of MFP on alarm  (ALMxCTL)
ALMxC_SEC       =  0x00       #  ALARM compare on SEC      (ALMxCTL)
ALMxC_MIN       =  0x10       #  ALARM compare on MIN      (ALMxCTL)
ALMxC_HR        =  0x20       #  ALARM compare on HOUR     (ALMxCTL)
ALMxC_DAY       =  0x30       #  ALARM compare on DAY      (ALMxCTL)
ALMxC_DAT       =  0x40       #  ALARM compare on DATE     (ALMxCTL)
ALMxC_ALL       =  0x70       #  ALARM compare on all param(ALMxCTL)
ALMx_IF         =  0x08       #  MASK of the ALARM_IF      (ALMxCTL)
OSCON           =  0x20       #  State of the oscillator(running or not)
VBATEN          =  0x08       #  Enable battery for back-up

OSCRUN          =  0x20       #  State of the oscillator(running or not)
PWRFAIL         =  0x10
VBATEN          =  0x08       #  Enable battery for back-up
VBAT_DIS        =  0x37       #  Disable battery back-up

START_32KHZ     =  0x80       #  Start crystal: ST = b7 (SEC)
LP              =  0x20       #  Mask for the leap year bit(MONTH REG)
HOUR_12         =  0x40       #  12 hours format   (HOUR)
LPYR            =  0x20

##################################################################################
ALM1MSK2        =   0x40
ALM1MSK1        =   0x20
ALM1MSK0        =   0x10

ALM0MSK2        =   0x40
ALM0MSK1        =   0x20
ALM0MSK0        =   0x10


class RTCC_Struct(object): 
	"""RTCC time object."""
	def __init__(self,sec,min,hour,weekday,date,month,year):
		self.sec = sec
		self.min = min
		self.hour = hour
		self.weekday = weekday
		self.date = date
		self.month = month
		self.year = year

class Alarm:
	"""
	Class representation of Alarms for RTCC chip.
	"""
	ZERO = 1
	ONE = 2
	
class PMAM_t:
	"""
	Class representation for PM/AM settings for RTCC chip.
	"""
	AMT = 0
	PMT = 1

class Match:
	"""
	Class representation of Alarm match settings for RTCC chip.
	"""
	SECONDS_MATCH = 0
	MINUTES_MATCH = 1
	HOURS_MATCH = 2
	WEEKDAY_MATCH = 3
	DATE_MATCH = 4
	FULL_DATE_MATCH = 5
	
class Polarity:
	"""
	Class representation of Polarity settings for RTCC chip.
	"""
	LOWPOL = 0
	HIGHPOL = 1

class Mode:
	"""
	Class representation of Mode settings for RTCC chip.
	"""
	GPO = 0
	ALARM_INTERRUPT =1
	SQUARE_WAVE = 2

class MCP79410(object):
	"""
	Representation of an MCP79410 RTCC chip.
	"""
	
	def __init__(self):
		"""
		Initializes the RTCC object with the current local time.
		
		
		:param none: 
		:returns none:
		"""
		self._address = MCP79410address
		localtime = time.localtime(time.time())
		rtc_time=RTCC_Struct(localtime.tm_sec,localtime.tm_min,localtime.tm_hour,localtime.tm_wday,localtime.tm_mday,localtime.tm_mon,(localtime.tm_year-2000))

		self.SetHourFormat(24)
		self.EnableVbat()				#Enable the battery back-up	
		self.EnableOscillator()			#Start RTC clock
		self.SetTime(rtc_time)
		
	def EnableOscillator(self):
		"""
		Enables the clock oscillator. This must be enabled in order for the RTCC to run.
		
		
		:param none: 
		:returns none:
		"""
		ST_bit = self.readRegister(DAY);      			#Read day + OSCON bit
		ST_bit = ST_bit | START_32KHZ;
		self.writeRegister(SEC,ST_bit)
		
	def DisableOscillator(self):
		"""
		Disables the clock oscillator.The RTCC does not operate once the oscillator is stopped.
		
		
		:param none: 
		:returns none:
		"""
		ST_bit = self.readRegister(DAY);      				#Read day + OSCON bit
		ST_bit = ST_bit & ~START_32KHZ;
		self.writeRegister(SEC,ST_bit)
		
	def IsRunning(self):
		"""
		Checks if the on-chip clock is running.
		
		
		:returns ClockStatus: TRUE if clock is running , FALSE otherwise.
		"""
		mask = self.readRegister(DAY);
		if((mask & OSCRUN) == OSCRUN):             			#If oscillator = already running, do nothing.
			return True
		else:
			return False
			
	def GetTime(self):
		"""
		Returns a time object with the current time from the RTCC.
		
		
		:param none: 
		:returns current_time : This is an RTCC class object that contains the time. 
		"""
		seconds = self.bcd2dec( self.readRegister(SEC) &(~START_32KHZ)) # mask out ST bit
		minutes = self.bcd2dec( self.readRegister(MIN))
		
		hour_t = self.readRegister(HOUR)
		if((hour_t & HOUR_12) == HOUR_12):
			(hour_t & 0x1F) 
		else:
			(hour_t & 0x3F)
			
		hours = self.bcd2dec(hour_t)		
		weekday = self.bcd2dec( self.readRegister(DAY) & ~(OSCRUN|PWRFAIL|VBATEN))
		date = self.bcd2dec( self.readRegister(DATE))   
		month = self.bcd2dec( self.readRegister(MNTH) & ~(LPYR))  
		year = self.bcd2dec( self.readRegister(YEAR))
		
		rtc_time=RTCC_Struct(seconds,minutes,hours,weekday,date,month,year)
		
		return rtc_time
	
	def SetTime(self,RTCtime):
		"""
		Initializes the RTCC with a specific time contained in the time structure.
		
		
		:param RTCtime: RTCC class object that contains the time to be set.
		:returns: none
		"""
		sec = self.readRegister(SEC)		#Seconds
		min = 0								#Minutes
		hour = self.readRegister(HOUR)		#Hours
		weekday = self.readRegister(DAY)	#Weekday
		date = 0;							#Date
		month = self.readRegister(MNTH)		#Month
		year = 0							#Year
		
		if((sec & START_32KHZ) == START_32KHZ):					#Seconds register
			sec = self.dec2bcd(RTCtime.sec)| START_32KHZ
		else:
			sec = self.dec2bcd(RTCtime.sec)
		
		min = self.dec2bcd(RTCtime.min)				#Minutes
	
		if(( hour & HOUR_12) ==  HOUR_12):				#Hour register
			hour = self.dec2bcd(RTCtime.hour) | HOUR_12
		else:
			hour = self.dec2bcd(RTCtime.hour)
	
		if(( hour & PM) ==  PM):
			hour = hour | PM
		
		weekday &= 0x38											#Mask 3 upper bits
		weekday |=  self.dec2bcd(RTCtime.weekday) 			#Weekday	
		date =  self.dec2bcd(RTCtime.date)					#Date
	
		if((month & LPYR) == LPYR):								#Month 
			month = self.dec2bcd(RTCtime.month) | LPYR
		else:
			month = self.dec2bcd(RTCtime.month)
			
		year = self.dec2bcd(RTCtime.year)					#Year
		
		self.writeRegister(SEC,sec)
		self.writeRegister(MIN,min)  
		self.writeRegister(HOUR,hour)
		self.writeRegister(DAY,weekday)
		self.writeRegister(DATE,date)  
		self.writeRegister(MNTH,month)   
		self.writeRegister(YEAR,year)

	def  SetHourFormat(self,format):
		"""
		Sets the hour format according to standard or military format.
		
		
		:param format:  Type of format is either 24 or 12.
		:returns: none
		"""
		self.DisableOscillator()						#Disable clock
		Format_bit = self.readRegister(HOUR)			#Read hour format bit	
		
		if(format == 24):
			Format_bit &= ~HOUR_FORMAT					#Set format to H24 (military) 
		else:
			Format_bit |= HOUR_FORMAT					#Set format to H12
		
		self.writeRegister(HOUR,Format_bit)				#START bit is located in the Sec register
		self.EnableOscillator()							#Enable clock
	
	def SetPMAM(self, meridian):
		"""
		Sets the meridian mode.
		
		
		:param meridian: PMAM_t object ,either PMT or AMT settings
		:returns: none
		"""
		self.DisableOscillator()						#Diable clock
		PMAM_bit = self.readRegister(HOUR)				#Read meridian bit	
		if(meridian == PMAM_t.AMT):
			PMAM_bit &= ~PM								#Set AM
		else:
			PMAM_bit |= PM								#Set PM
		
		self.writeRegister(HOUR,PMAM_bit)				#Update PM/AM meridian bit
		self.EnableOscillator();						#Enable clock
	
	def GetPMAM(self):
		"""
		Get AM or PM for 12 hour format.
		
		
		:returns meridian: PM/AM flag
		"""
		bHourBuffer = self.readRegister(HOUR)
		return (bHourBuffer & 0x20)
		
	def EnableAlarm(self, alarm,config):				#dest = RTCC_ALM0/RTCC_ALM1
		"""
		Enables the chosen alarm.
		
		
		:param alarm: Alarm object , One of the two alarms, ZERO or ONE.
		:param config:
		:returns none:
		"""
		control = self.readRegister(CTRL)
		if (alarm == Alarm.ZERO):
			ALARMREG = (control | ALM_0)
		else:
			ALARMREG = (control | ALM_1)
			
		self.writeRegister(CTRL,control)				#enable alarm control bit		
		day = self.readRegister(control)				#Set address to the alarm config/day register 
		AlmarmCfg = ((day & 0x07) | (config & 0xF0))
		self.writeRegister(ALARMREG,AlmarmCfg)
		
	def DisableAlarm(self, alarm):						#alarm = RTCC_ALM0/RTCC_ALM1
		"""
		Disables one of the two alarms. 
		
		
		:param alarm: Alarm object , One of the two alarms, ZERO or ONE.
		:returns none:
		"""
		temp = self.readRegister(CTRL)					#read control register
		if (alarm == Alarm.ZERO):
			cfg = (temp & 0xEF)							#disables either Alrm1 or Alrm0
		else:
			cfg = (temp & 0xDF)
		self.writeRegister(CTRL,cfg)					#update control register
	
	def GetAlarmStatus(self,alarm):
		"""
		Gets the status of the alarm interrupt flag.
		
		
		:param alarm: One of the two alarms, ZERO or ONE.
		:returns status: TRUE if alarm enabled , FALSE if disabled.
		"""
		if(alarm == Alarm.ZERO):		
			temp = self.readRegister(ALM0WDAY)			#Read WKDAY register for ALRAM 0  
		else:
			temp = self.readRegister(ALM1WDAY)			#Read WKDAY register for ALRAM 1
		
		if ((temp & ALMx_IF) == ALMx_IF): 
			return True
		else:
			return False
	
	def ClearInterruptFlag(self,alarm):
		"""
		Selects which alarm Interrupt flag should be cleared .
		
		
		:param alarm: One of the two alarms
		:returns: none
		"""
		if (alarm == Alarm.ZERO):
			temp = self.readRegister(ALM0WDAY)			#Read WKDAY register for ALRAM 0   
			temp &= (~ALMx_IF)							#Clear 4-th bit 
			self.writeRegister(ALM0WDAY,temp)			#Enable backup battery mode
		else:
			temp = self.readRegister(ALM1WDAY)			#Read WKDAY register for ALRAM 1
			temp &= (~ALMx_IF)							#Clear 4-th bit
			self.writeRegister(ALM1WDAY,temp)			#Enable backup battery mode

	
	def SetAlarmTime(self,time,alarm):
		"""
		Sets the alarm time for one of the two alarms.
		
		
		:param time: RTCC Class encapsulating time settings. 
		:param alarm:  One of the two alarms,either alarm ZERO or ONE.
		:returns: none
		"""	
		sec = self.dec2bcd(time.sec);
		min =  self.dec2bcd(time.min);
		hour =  self.dec2bcd(time.hour);
		weekday =  self.dec2bcd(time.weekday);
		date =  self.dec2bcd(time.date);
		month =  self.dec2bcd(time.month);
	
		if(alarm == Alarm.ZERO):
			self.writeRegister(ALM0SEC,sec)
			self.writeRegister(ALM0MIN,hour)
			self.writeRegister(ALM0HR,weekday)
			self.writeRegister(ALM0WDAY,date)
			self.writeRegister(ALM0DAT,month)
			self.writeRegister(ALM0MTH,month)
		else:
			self.writeRegister(ALM1SEC,sec|START_32KHZ)
			self.writeRegister(ALM1MIN,min)
			self.writeRegister(ALM1HR,hour)
			self.writeRegister(ALM1WDAY,weekday)
			self.writeRegister(ALM1DAT,date) 
			self.writeRegister(ALM1MTH,month)
		
	def GetAlarmTime(self,time,alarm):
		"""
		Get the alarm time for one of the two alarms.
		
		
		:param time: RTCC object
		:param alarm: One of the two alarms
		:returns: none
		"""
		seconds = self.readRegister(ALARMREG,sec)
		hours = self.readRegister(ALARMREG,hour)
		AlarmDay = self.readRegister(ALARMREG,day)
		AlarmDate = self.readRegister(ALARMREG,date)
		alarmTime=RTCC_Struct(seconds,minutes,hours,AlarmDay,AlarmDate,0,0)
		return alarmTime
	
	def SetAlarmMFPPolarity(self,MFP_pol,alarm):
		"""
		Get the alarm time on the reg.
		
		
		:param MFP_pol:   Multi function pin polarity
		:param alarm:  One of the two alarms
		:returns: none
		"""
		Polarity_bit = 0;	
		if(alarm == Alarm.ZERO):
			Polarity_bit = self.readRegister(ALM0WDAY)      #Read hour format bit	
		else:
			Polarity_bit = self.readRegister(ALM1WDAY)      #Read hour format bit	
		
		if(MFP_pol == Polarity.LOWPOL):
			Polarity_bit &= ~ALMx_POL			#Set MFP LOW
		else:
			Polarity_bit |= ALMx_POL			#Set MFP HIGH
	
		if(alarm == Alarm.ZERO):
			self.writeRegister(ALM0WDAY,Polarity_bit)		#Update polarity bit for Alarm 0
		else:
			self.writeRegister(ALM1WDAY,Polarity_bit)		#Update polarity bit for Alarm 1

		
	def SetAlarmMatch(self, match,alarm):
		"""
		Get the alarm time on the reg.
		
		
		:param match: second, hour or mintue match from Match class
		:param alarm: one of the two alarms
		:returns: none
		"""
		AlarmRegister = 0
		if(alarm == Alarm.ZERO):	
			AlarmRegister = ALM0WDAY
		else:
			AlarmRegister = ALM1WDAY
	
		match_bits = self.readRegister(AlarmRegister)

		if (match == Match.SECONDS_MATCH):
			match_bits &= ~(ALM0MSK2|ALM0MSK1|ALM0MSK0)
			self.writeRegister(AlarmRegister,match_bits)	#Minutes match
		elif (match == Match.MINUTES_MATCH):
			match_bits |= ALM0MSK0
			self.writeRegister(AlarmRegister,match_bits)	#Minutes match
		elif (match == Match.HOURS_MATCH):
			match_bits |= ALM0MSK1
			self.writeRegister(AlarmRegister,match_bits)	#Hours match
		elif (match == Match.WEEKDAY_MATCH):
			match_bits |= ALM0MSK1|ALM0MSK0
			self.writeRegister(AlarmRegister,match_bits)	#Day of week match
		elif (match == Match.DATE_MATCH):
			match_bits |= ALM0MSK2
			self.writeRegister(AlarmRegister,match_bits)	#Date match
		elif (match == Match.FULL_DATE_MATCH):
			match_bits |= ALM0MSK2|ALM0MSK1|ALM0MSK0
			self.writeRegister(AlarmRegister,match_bits)	#Sec, Minutes Hours, Date match
		else :
			match_bits |= ALM0MSK0
			self.writeRegister(AlarmRegister,match_bits)	#Minutes match
			
	
	def SetMFP_Functionality(self,mode):
		"""
		This function sets the MFP pin mode.
		
		
		:param mode: Mode of the MFP pin.
		:returns: none
		"""
		MFP_bits = self.readRegister(CTRL)
	
		if(mode ==	Mode.GPO):				#For GPO clear SQWEN, ALM0EN, ALM1EN
			MFP_bits &= ~(SQWEN|ALM_0|ALM_1)
			self.writeRegister(CTRL,MFP_bits)
		elif (mode ==	Mode.ALARM_INTERRUPT): 	#For ALARM Interrupts clear SQWEN and set either ALM0EN or ALM1EN
			MFP_bits &= SQWEN
			MFP_bits |= ALM_0
			self.writeRegister(CTRL,MFP_bits)
		elif (mode ==	Mode.SQUARE_WAVE) :		#For SQUARE WAVE set SQWEN 
			MFP_bits &= ~(ALM_0|ALM_1)
			MFP_bits |= SQWEN
			self.writeRegister(CTRL,MFP_bits)
		else:								#ALARM Interrupts 
			MFP_bits &= SQWEN
			MFP_bits |= ALM_0
			self.writeRegister(CTRL,MFP_bits)

	
	def SetMFP_GPOStatus(self,status):
		"""
		Sets the MFP output logic level when the pin is configured as GPO 
		
		
		:param status: Polarity of MFP pin , Asserted output state of MFP is a logic low level for LOW and opposite for HIGH
		:returns: none
		"""
		gpo_bit = self.readRegister(CTRL)		#General Purpose Output mode only available when (SQWEN = 0, ALM0EN = 0, and ALM1EN = 0):
	
		if(status == LOW):		
			gpo_bit = OUT_PIN				#MFP signal level is logic low
			self.writeRegister(CTRL,gpo_bit)
		else:								#MFP signal level is logic high
			gpo_bit |= OUT_PIN
			self.writeRegister(CTRL,gpo_bit)	
	
	
	def CheckPowerFailure(self):
		"""
		Checks if there was a power failure.
		
		
		:param none: 
		:returns PowerFail: TRUE if there was a power failure, FALSE otherwise.
		"""
		PowerFailure_bit = self.readRegister(DAY);     #Read meridian bit	
		PowerFail = 0
	
		if((PowerFailure_bit & PWRFAIL)  == PWRFAIL):
			PowerFail = 1
		else:
			PowerFail = 0
		
		PowerFailure_bit &= ~PWRFAIL			#Clear Power failure bit
		self.writeRegister(DAY,PowerFailure_bit)		#Update PM/AM meridian bit
		return PowerFail
				
	def IsVBatEnabled(self):	
		"""
		Check if the VBAT is enabled.
		
		
		:param none: 
		:returns status: True is battery mode is enabled , False otherwise.
		"""
		temp = self.readRegister(DAY)		#The 3rd bit of the RTCC_RTCC day register controls VBATEN   	
		if((temp & VBATEN) == VBATEN):
			return True;
		else:
			return False;
			
	def EnableVbat(self):
		"""
		Enables battery backup mode.
		
		
		:param none: 
		:returns: none
		"""
		temp = self.readRegister(DAY)		#The 3rd bit of the RTCC_RTCC day register controls VBATEN   
		temp = (temp | VBATEN)				#Set 3rd bit to enable backup battery mode
		self.writeRegister(DAY,temp)		#Enable backup battery mode

	def DisableVbat(self):
		"""
		Disables the backup battery functionality.
		
		
		:param none: 
		:returns none:
		"""
		temp = self.readRegister(DAY)			#The 3rd bit of the RTCC_RTCC day register controls VBATEN   
		temp = (temp & VBAT_DIS)			#Clear 3rd bit to disable backup battery mode
		self.writeRegister(DAY,temp)			#Enable backup battery mode	
		
	def GetPowerUpTime(self):
		"""
		Returns the time the system powered up.
		
		
		:param none: 
		:returns powerup_time: Power up time structure.
		"""
		powerup_time = RTCC_Struct(0,0,0,0,0,0,0)
		powerup_time.min = self.bcd2dec( self.readRegister(PWRUPMIN))    
		powerup_time.hour = self.bcd2dec( self.readRegister(PWRUPHOUR))
		powerup_time.date = self.bcd2dec( self.readRegister(PWRUPDATE))   
		powerup_time.month = self.bcd2dec( self.readRegister(PWRUPMTH))
		return powerup_time
	
		
	def GetPowerDownTime(self):
		"""
		This function returns the power-down time of the system.
		
		
		:param none: 
		:returns powerdown_time: Power down time structure.
		"""
		powerdown_time = RTCC_Struct(0,0,0,0,0,0,0)
		powerdown_time.min = self.bcd2dec( self.readRegister(PWRDNMIN))   
		powerdown_time.hour = self.bcd2dec( self.readRegister(PWRDNHOUR))
		powerdown_time.date = self.bcd2dec( self.readRegister(PWRDNDATE))
		powerdown_time.month = self.bcd2dec( self.readRegister(PWRDNMTH))
		return powerdown_time

##################################################################################################
	def dec2bcd(self,num):
		"""
		Convert the value from decimal to Binary Coded Decimal (BCD).
		
		
		:param num: Number to convert to BCD
		:returns bcd: BCD representation of number.
		"""
		return ((num/10 * 16) + (num % 10))

	def bcd2dec(self,num):
		"""
		Convert the value from Binary Coded Decimal (BCD) to Decimal.
		
		
		:param num: Number to convert to decimal.
		:returns dec: Decimal representation of number.
		"""
		return ((num/16 * 10) + (num % 16))
		
	def bcdtobin(self,num):
		"""
		Convert the value from BCD to binary.
		
		
		:param num:  Number to convert to binary.
		:returns bin: Binary representation of number.
		"""
		return (((num) & 0x0f) + ((num) >> 4) * 10)
		
	def writeRegister(self,rtcc_reg,dat):
		"""
		Write a new value on the register.
		
		
		:param rtcc_reg: Address of the register.
		:param dat:  Byte value to be written on the register.
		:returns: none
		"""
		bus.write_byte_data(self._address, rtcc_reg, dat)

	def readRegister(self,rtcc_reg):
		"""
		Read the value of the register.
		
		
		:param rtcc_reg: Address of the register.
		:returns: Register byte content 
		"""
		result = bus.read_byte_data(self._address, rtcc_reg) & 0xFF
		return result

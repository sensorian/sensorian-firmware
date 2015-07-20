/****************************************************************************
 * Copyright (C) 2015 Sensorian
 *                                                                          *
 * This file is part of Sensorian.                                          *
 *                                                                          *
 *   Sensorian is free software: you can redistribute it and/or modify it   *
 *   under the terms of the GNU Lesser General Public License as published  *
 *   by the Free Software Foundation, either version 3 of the License, or   *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   Sensorian is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU Lesser General Public License for more details.                    *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with Sensorian.                                          *
 *   If not, see <http://www.gnu.org/licenses/>.                            *
 ****************************************************************************/
 
/**
 * @file MCP79410_RTCC/Example1/MCP79410.c
 * @author D.Qendri
 * @date 30 Dec 2014
 * @brief MCP79410 driver.
 */
 
#include "MCP79410.h"
#include "i2c.h"
#include <time.h>
#include <stdlib.h>

//#define __DEBUG_RTCC__

/// \defgroup RealTimeClock Real Time Clock and Calendar
/// These functions allow the user to leverage the RTCC time keeping capabilities and alarm settings.
/// @{

/**
 * @brief Initializes the RTCC with the system time.
 * @return none
 */
void MCP79410_Initialize(void)
{
	MCP79410_SetHourFormat(H24);				//Set hour format to military time standard
	MCP79410_EnableVbat();						//Enable battery backup
	
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);	
	RTCC_Struct curr_time = {tm.tm_sec,tm.tm_min,tm.tm_hour,tm.tm_wday,tm.tm_mday,tm.tm_mon+1, ((tm.tm_year+1900)-2000)};
	#ifdef __DEBUG_RTCC__
		printf("MCP79410_Initialize DEBUG now: %d-%d-%d %d:%d:%d\n", tm.tm_year, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);	
	#endif
	MCP79410_SetTime(&curr_time);	
	MCP79410_EnableOscillator();				//Start clock by enabling oscillator
}

/**
 * @brief Enables the clock oscillator. This must be enabled in order for the RTCC to run.
 * @return none
 */
void MCP79410_EnableOscillator(void)
{
	unsigned char ST_bit = MCP79410_Read(DAY);      //Read day + OSCON bit
	ST_bit = ST_bit | START_32KHZ;
	MCP79410_Write(SEC,ST_bit);						//START bit is located in the Sec register
}

/**
 * @brief Disables the clock oscillator.The RTCC does not operate once the oscillator is stopped.
 * @return none
 */
void MCP79410_DisableOscillator(void)
{
	unsigned char ST_bit = MCP79410_Read(DAY);      //Read day + OSCON bit
	ST_bit = ST_bit & ~START_32KHZ;
	MCP79410_Write(SEC,ST_bit);						//START bit is located in the Sec regist
}

/**
 * @brief Checks if the chip clock is running.
 * @return ClockStatus Variable is TRUE if clock is running , FALSE otherwise.
 */
unsigned char MCP79410_IsRunning(void)
{
	unsigned char mask = MCP79410_Read(DAY);
	
	if((mask & OSCRUN) == OSCRUN)             //If oscillator = already running, do nothing.
	{
		return TRUE;
	}else{
		return FALSE;
	}
}

/**
 * @brief The function returns a time structure with the current time from the RTCC.
 * @return current_time This is an RTCC_Struct pointer. Refer to the documenation of this data structure.
 */
RTCC_Struct* MCP79410_GetTime(void)
{
	RTCC_Struct *current_time = (RTCC_Struct *) malloc(sizeof(RTCC_Struct));
	
    current_time->sec	= MCP79410_bcd2dec( MCP79410_Read(SEC) & (~START_32KHZ));
	current_time->min = MCP79410_bcd2dec( MCP79410_Read(MIN));    
	
	unsigned char hour_t = MCP79410_Read(HOUR);
	
	hour_t = ((hour_t & HOUR_12) == HOUR_12)? (hour_t & 0x1F) : (hour_t & 0x3F);				//hour is in 24 hour format
	
	current_time->hour = MCP79410_bcd2dec(hour_t);
	current_time->weekday = MCP79410_bcd2dec( MCP79410_Read(DAY) & ~(OSCRUN|PWRFAIL|VBATEN));
    current_time->date = MCP79410_bcd2dec( MCP79410_Read(DATE));    
    current_time->month = MCP79410_bcd2dec( MCP79410_Read(MNTH) & ~(LPYR));    
	current_time->year = MCP79410_bcd2dec( MCP79410_Read(YEAR)); 
	
	return current_time;
}

/**
 * @brief This function initializes the RTCC with a specific time contained in the time structure.
 * @param time RTCC struct that contains the time to be set
 * @return none
 */
void MCP79410_SetTime(RTCC_Struct *time)
{
	unsigned char sec = MCP79410_Read(SEC);						//Seconds
	unsigned char min = 0;										//Minutes
	unsigned char hour = MCP79410_Read(HOUR);					//Hours
	unsigned char weekday = MCP79410_Read(DAY);					//Weekday
	unsigned char date = 0;										//Date
	unsigned char month = MCP79410_Read(MNTH);					//Month
	unsigned char year = 0;										//Year
		
	if((sec & START_32KHZ) == START_32KHZ)						//Seconds register
	{
		sec = MCP79410_dec2bcd(time->sec)| START_32KHZ;
	}else{
		sec = MCP79410_dec2bcd(time->sec);
	}
	
	min = MCP79410_dec2bcd(time->min);							//Minutes
	
	if(( hour & HOUR_12) ==  HOUR_12)							//Hour register
	{
		hour = MCP79410_dec2bcd(time->hour) | HOUR_12;
	}else{
		hour = MCP79410_dec2bcd(time->hour);
	}
	
	if(( hour & PM) ==  PM)
	{
		hour = hour | PM;
	}
	
	weekday &= 0x38;											//Mask 3 upper bits
	weekday |=  MCP79410_dec2bcd(time->weekday) ;				//Weekday
	
	date =  MCP79410_dec2bcd(time->date);						//Date
	
	if((month & LPYR) == LPYR)									//Month 
	{
		month = MCP79410_dec2bcd(time->month) | LPYR;
	}else{
		month = MCP79410_dec2bcd(time->month);
	}
	
	year = MCP79410_dec2bcd(time->year);						//Year
	
    MCP79410_Write(SEC,sec);
	MCP79410_Write(MIN,min);    
	MCP79410_Write(HOUR,hour); 
	MCP79410_Write(DAY,weekday);
    MCP79410_Write(DATE,date);    
    MCP79410_Write(MNTH,month);    
	MCP79410_Write(YEAR,year);
}

 /**
 * @brief This function sets the hour format 
 * @param format Type of format is Format_t.
 * @return none
 */
void MCP79410_SetHourFormat(Format_t format)
{
	MCP79410_DisableOscillator();					//Diable clock
	unsigned char Format_bit = MCP79410_Read(HOUR); //Read hour format bit	
	if(format == H24)
	{
		Format_bit &= ~HOUR_FORMAT;					//Set format to H12 (military) 
	}else{
		Format_bit |= HOUR_FORMAT;					//Set format to H12
	}	
	MCP79410_Write(HOUR,Format_bit);				//START bit is located in the Sec register
	MCP79410_EnableOscillator();					//Enable clock
}

 /**
 * @brief This function sets the meridian mode
 * @param meridian Set either PM or AM settings.
 * @return none
 */
void MCP79410_SetPMAM(PMAM_t meridian)
{
	MCP79410_DisableOscillator();					//Diable clock
	unsigned char PMAM_bit = MCP79410_Read(HOUR);   //Read meridian bit	
	if(meridian == AMT)
	{
		PMAM_bit &= ~PM;							//Set AM
	}else{
		PMAM_bit |= PM;								//Set PM
	}	
	MCP79410_Write(HOUR,PMAM_bit);					//Update PM/AM meridian bit
	MCP79410_EnableOscillator();					//Enable clock
}

 /**
 * @brief This function sets the alarm time.
 * @param alarm Enables one of the two alarms.
 * @return none
 */
void MCP79410_EnableAlarm(Alarm_t alarm)
{
	unsigned char ctrl_bits = MCP79410_Read(CTRL);
	if(alarm == ZERO)
	{	
		ctrl_bits |= ALM_0;
		MCP79410_Write(CTRL,ctrl_bits);
	}else{
		ctrl_bits |= ALM_1;
		MCP79410_Write(CTRL,ctrl_bits);
	}
}

/**
 * @brief This function disables one of the two alarms. 
 * @param alarm Disables one of the two alarms.
 * @return none
 */
void	MCP79410_DisableAlarm(Alarm_t alarm)
{
	unsigned char ctrl_bits = MCP79410_Read(CTRL);
	if(alarm == ZERO)
	{	
		ctrl_bits &= ~ALM_0;
		MCP79410_Write(CTRL,ctrl_bits);
	}else{
		ctrl_bits &= ~ALM_1;
		MCP79410_Write(CTRL,ctrl_bits);
	}
}

/**
 * @brief This function gets the status of the alarm interrupt flag
 * @param alarm One of the two alarms, ZERO or ONE.
 * @return none
 */
 AlarmStatus_t MCP79410_GetAlarmStatus(Alarm_t alarm)
{
	AlarmStatus_t status;
	unsigned char temp;     
	if(alarm == ZERO)
	{
		temp = MCP79410_Read(ALM0WDAY);			//Read WKDAY register for ALRAM 0  
	}else{
		temp = MCP79410_Read(ALM1WDAY);			//Read WKDAY register for ALRAM 1
	}
	return status = (AlarmStatus_t)((temp & ALMx_IF) == ALMx_IF)? SET : NOT_SET;
}

/**
 * @brief Clears the interrupt alarm flag
 * @param alarm Selects which alarm Interrupt flag should be cleared  
 * @return none
 */
void MCP79410_ClearInterruptFlag(Alarm_t alarm)
{
	unsigned char temp;     
	if(alarm == ZERO)
	{
		temp = MCP79410_Read(ALM0WDAY);			//Read WKDAY register for ALRAM 0   
		temp &= (~ALMx_IF);						//Clear 4-th bit 
		MCP79410_Write(ALM0WDAY,temp);			//Enable backup battery mode
	}else{
		temp = MCP79410_Read(ALM1WDAY);			//Read WKDAY register for ALRAM 1
		temp &= (~ALMx_IF);						//Clear 4-th bit
		MCP79410_Write(ALM1WDAY,temp);			//Enable backup battery mode
	}
}

 /**
 * @brief This function sets the alarm time for one of the two alarms.
 * @param *time Pointer of type RTCC_Struct encapsulating time settings. 
 * @param alarm One of the two alarms,either alarm ZERO or ONE.
 * @return none
 */
void MCP79410_SetAlarmTime(RTCC_Struct *time, Alarm_t alarm)
{
	unsigned char sec = MCP79410_dec2bcd(time->sec);
	unsigned char min =  MCP79410_dec2bcd(time->min);
	unsigned char hour =  MCP79410_dec2bcd(time->hour);
	unsigned char weekday =  MCP79410_dec2bcd(time->weekday);
	unsigned char date =  MCP79410_dec2bcd(time->date);
	unsigned char month =  MCP79410_dec2bcd(time->month);
		
	if(alarm == ZERO)
	{	
		MCP79410_Write(ALM0SEC,sec|START_32KHZ);
		MCP79410_Write(ALM0MIN,min);    
		MCP79410_Write(ALM0HR,hour); 
		MCP79410_Write(ALM0WDAY,weekday);
		MCP79410_Write(ALM0DATE,date);    
		MCP79410_Write(ALM0MTH,month);   
	}else{
		MCP79410_Write(ALM1SEC,sec|START_32KHZ);
		MCP79410_Write(ALM1MIN,min);    
		MCP79410_Write(ALM1HR,hour); 
		MCP79410_Write(ALM1WDAY,weekday);
		MCP79410_Write(ALM1DATE,date);    
		MCP79410_Write(ALM1MTH,month);
	}
}
 
/**
 * @brief This function sets the MFP polarity either high or low 
 * @param MFP_pol polarity of MFP pin , Asserted output state of MFP is a 
 *		  logic low level for LOW and opposite for HIGH
 * @param alarm One of the two alarms, either alarm ZERO or ONE.
 * @return none
 */
void MCP79410_SetAlarmMFPPolarity(Polarity_t MFP_pol,Alarm_t alarm)
{
	unsigned char Polarity_bit = 0;
	
	if(alarm == ZERO)
	{	
		Polarity_bit = MCP79410_Read(ALM0WDAY);      //Read hour format bit	
	}
	else{
		Polarity_bit = MCP79410_Read(ALM1WDAY);      //Read hour format bit	
	}
	
	if(MFP_pol == LOW)
	{
		Polarity_bit &= ~ALMx_POL;			//Set MFP LOW
	}else{
		Polarity_bit |= ALMx_POL;			//Set MFP HIGH
	}	
	
	if(alarm == ZERO)
	{
		MCP79410_Write(ALM0WDAY,Polarity_bit);		//Update polarity bit for Alarm 0
	}else{
		MCP79410_Write(ALM1WDAY,Polarity_bit);		//Update polarity bit for Alarm 1
	}
}
 
/**
 * @brief Alarm Asserts on Match on seconds, minutes ,hours depending on match parameter
 * @param match Match setting
 * @param alarm Select either alarm ZERO or ONE
 * @return none
 */
void MCP79410_SetAlarmMatch(Match_t match,Alarm_t alarm)
{	
	unsigned char AlarmRegister = 0;
	if(alarm == ZERO)
	{
		AlarmRegister = ALM0WDAY;
	}else{
		AlarmRegister = ALM1WDAY;
	}
	
	unsigned char match_bits = MCP79410_Read(AlarmRegister);

	switch(match)
	{
		case SECONDS_MATCH :
			match_bits &= ~(ALM0MSK2|ALM0MSK1|ALM0MSK0);
			MCP79410_Write(AlarmRegister,match_bits);	//Minutes match
			break;
		case MINUTES_MATCH :
			match_bits |= ALM0MSK0;
			MCP79410_Write(AlarmRegister,match_bits);	//Minutes match
			break;
		case HOURS_MATCH :
			match_bits |= ALM0MSK1;
			MCP79410_Write(AlarmRegister,match_bits);	//Hours match
			break;
		case WEEKDAY_MATCH :
			match_bits |= ALM0MSK1|ALM0MSK0;
			MCP79410_Write(AlarmRegister,match_bits);	//Day of week match
			break;
		case DATE_MATCH :
			match_bits |= ALM0MSK2;
			MCP79410_Write(AlarmRegister,match_bits);	//Date match
			break;
		case FULL_DATE_MATCH :
			match_bits |= ALM0MSK2|ALM0MSK1|ALM0MSK0;
			MCP79410_Write(AlarmRegister,match_bits);	//Sec, Minutes Hours, Date match
			break;
		default :
			match_bits |= ALM0MSK0;
			MCP79410_Write(AlarmRegister,match_bits);	//Minutes match
			break;
	}	
}

/**
 * @brief This function sets the MFP pin mode
 * @param mode Mode of the MFP pin.
 * @return none
 */
void MCP79410_SetMFP_Functionality(MFP_t mode)
{	
	unsigned char MFP_bits = MCP79410_Read(CTRL);
	
	switch(mode)
	{
		case GPO :				//For GPO clear SQWEN, ALM0EN, ALM1EN
			MFP_bits &= ~(SQWEN|ALM_0|ALM_1);
			MCP79410_Write(CTRL,MFP_bits);
			break;		
		case ALARM_INTERRUPT : 	//For ALARM Interrupts clear SQWEN and set either ALM0EN or ALM1EN
			MFP_bits &= SQWEN;
			MFP_bits |= ALM_0;
			MCP79410_Write(CTRL,MFP_bits);
			break;			
		case SQUARE_WAVE :		//For SQUARE WAVE set SQWEN 
			MFP_bits &= ~(ALM_0|ALM_1);
			MFP_bits |= SQWEN;
			MCP79410_Write(CTRL,MFP_bits);
			break;			
		default:				//ALARM Interrupts 
			MFP_bits &= SQWEN;
			MFP_bits |= ALM_0;
			MCP79410_Write(CTRL,MFP_bits);
			break;	
	}
}

/**
 * @brief This function sets the MFP output logic level when the pin is configured as GPO 
 * @param status Polarity of MFP pin , Asserted output state of MFP is a logic low level for LOW and opposite for HIGH
 * @return none
 */
void MCP79410_SetMFP_GPOStatus(Polarity_t status)
{
	unsigned char gpo_bit = MCP79410_Read(CTRL);		//General Purpose Output mode only available when (SQWEN = 0, ALM0EN = 0, and ALM1EN = 0):
	
	if(status == LOW)
	{
		gpo_bit = OUT_PIN;				//MFP signal level is logic low
		MCP79410_Write(CTRL,gpo_bit);
	}else{								//MFP signal level is logic high
		gpo_bit |= OUT_PIN;
		MCP79410_Write(CTRL,gpo_bit);
	}
}

 /**
 * @brief This function checks if there was a power failure
 * @return none
 */
unsigned char MCP79410_CheckPowerFailure(void)
{
	unsigned char PowerFailure_bit = MCP79410_Read(DAY);      //Read meridian bit	
	unsigned char PowerFail;
	
	if((PowerFailure_bit & PWRFAIL)  == PWRFAIL)
	{
		PowerFail = TRUE;
	}else{
		PowerFail = FALSE;
	}
	PowerFailure_bit &= ~PWRFAIL;		//Clear Power failure bit
	MCP79410_Write(DAY,PowerFailure_bit);		//Update PM/AM meridian bit

	return PowerFail;
}

 /**
 * @brief This function checks if battery mode is enabled
 * @return status True is battery mode is enabled , False otherwise.
 */
unsigned char 	MCP79410_IsVbatEnabled(void)
{
	unsigned char temp;
    temp = MCP79410_Read(DAY);		//The 3rd bit of the RTCC_RTCC day register controls VBATEN   
	
	if((temp & VBATEN) == VBATEN)
	{
		return TRUE;
	}else{
		return FALSE;
	}
}
 
 /**
 * @brief This function enables backup battery mode
 * @return none
 */
void MCP79410_EnableVbat(void)
{
	unsigned char temp;
    temp = MCP79410_Read(DAY);		//The 3rd bit of the RTCC_RTCC day register controls VBATEN   
    temp = (temp | VBATEN);			//Set 3rd bit to enable backup battery mode
    MCP79410_Write(DAY,temp);		//Enable backup battery mode
}

 /**
 * @brief This function disables the backup battery functionality
 * @return none
 */
void MCP79410_DisableVbat(void)
{
	unsigned char temp;
    temp = MCP79410_Read(DAY);		//The 3rd bit of the RTCC_RTCC day register controls VBATEN   
    temp = (temp & VBAT_DIS);		//Clear 3rd bit to disable backup battery mode
    MCP79410_Write(DAY,temp);		//Enable backup battery mode	
}

 /**
 * @brief This function returns the power-up time of the RTCC
 * @return powerup_time Struct of type RTCC_Struct containg power up time.
 */
RTCC_Struct* MCP79410_GetPowerUpTime(void)
{
	RTCC_Struct *powerup_time = (RTCC_Struct *) malloc(sizeof(RTCC_Struct));
	
	powerup_time->min = MCP79410_bcd2dec( MCP79410_Read(PWRUPMIN));    
	powerup_time->hour = MCP79410_bcd2dec( MCP79410_Read(PWRUPHOUR)); 
    powerup_time->date = MCP79410_bcd2dec( MCP79410_Read(PWRUPDATE));    
    powerup_time->month = MCP79410_bcd2dec( MCP79410_Read(PWRUPMTH));   
	
	return powerup_time;
}

 /**
 * @brief This function returns the power-down time of the RTCC
 * @return powerdown_time Struct of type RTCC_Struct containg power down time.
 */
RTCC_Struct* MCP79410_GetPowerDownTime(void)
{
	RTCC_Struct *powerdown_time = (RTCC_Struct *) malloc(sizeof(RTCC_Struct));
	
	powerdown_time->min = MCP79410_bcd2dec( MCP79410_Read(PWRDNMIN));    
	powerdown_time->hour = MCP79410_bcd2dec( MCP79410_Read(PWRDNHOUR)); 
    powerdown_time->date = MCP79410_bcd2dec( MCP79410_Read(PWRDNDATE));    
    powerdown_time->month = MCP79410_bcd2dec( MCP79410_Read(PWRDNMTH));   
	
	return powerdown_time;
}

/**
 * @brief Convert Binary Coded Decimal (BCD) to Decimal
 * @param num Number to be converted to BCD
 * @return bcd BCD value of decimal number.
 */
unsigned char MCP79410_dec2bcd(unsigned char num)
{
  return ((num/10 * 16) + (num % 10));
}

/**
 * @brief Convert Binary Coded Decimal (BCD) to Decimal
 * @param num Number to be converted to decimal
 * @return dec Decimal value of BCD number
 */
unsigned char MCP79410_bcd2dec(unsigned char num)
{
  return ((num/16 * 10) + (num % 16));
}

/// @}

/**
 * @brief The below function writes a data byte in the I2C RTCC
 * @param rtcc_reg Chip register to write to.
 * @param time_var Byte data for register.
 * @return none
 */
void MCP79410_Write(unsigned char rtcc_reg, unsigned char time_var)
{
    I2C_WriteByteRegister(rtcc_reg,time_var);
}  

/**
 * The function reads a data byte from the I2C RTCC
 * @param rtcc_reg Address of register.
 * @return reg Register content.
 */
unsigned char  MCP79410_Read(unsigned char rtcc_reg)
{
    return I2C_ReadByteRegister(rtcc_reg);    	
}
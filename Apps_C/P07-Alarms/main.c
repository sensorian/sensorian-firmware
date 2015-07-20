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
 * @file MCP79410_RTCC/Example4/main.c
 * @author D.Qendri
 * @date 30 Dec 2014
 * @brief RTCC Alarm demo. This program enables the alarm on the MFP pin.
 * 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>		
#include "MCP79410.h"
#include "MPL3115A2.h"
#include "Utilities.h"
#include "i2c.h"

int main(int argc, char **argv)
{
	float temp = 0;
	I2C_Initialize(MCP79410_ADDRESS);					//Initialize I2C and setup chip address
	
	RTCC_Struct *current_time = (RTCC_Struct *) malloc(sizeof(RTCC_Struct));
	RTCC_Struct *alarm_time = (RTCC_Struct *) malloc(sizeof(RTCC_Struct));	
	RTCC_Struct user_alarm = {2, 57, 0, 0, 0, 0, 0};	//Change minutes value for match
	alarm_time = &user_alarm;
	
	//MCP79410_Initialize();								//Initialize RTCC with system time and date 
	MCP79410_ClearInterruptFlag(ZERO);	
	MCP79410_SetAlarmTime(alarm_time,ZERO);				//Set alarm time
	MCP79410_SetAlarmMatch(MINUTES_MATCH,ZERO);			//Alarm ZERO will trigger on minutes match
	MCP79410_SetAlarmMFPPolarity(LOWPOL,ZERO);			//Configure Alarm pin polarity as HIGH
	MCP79410_SetMFP_Functionality(ALARM_INTERRUPT);		//Set alaram interrupt
	PinLevel_t pval = ReadPinStatus(MPL_PIN);
	printf("MPL pin level is %d\r\n",(unsigned char)pval);
	
	pinModeOutput(LED_PIN);								//Set LED pin as output
	
	while(1)
	{
		AlarmStatus_t alrm_flag = MCP79410_GetAlarmStatus(ZERO);		//Check alarm status 
		printf("Alarm interrupt status %d\r\n",(unsigned int)alrm_flag);
		if(alrm_flag != 0)
		{
			digitalWrite(LED_PIN, HIGH);				//Let there be light
			delay_ms(500); 
			I2C_Initialize(MPL3115A2_ADDRESS);			//Initialize I2C peripheral
			delay_ms(500);
			MPL3115A2_Initialize();						//Initialize the sensor
			delay_ms(500);
			temp = MPL3115A2_ReadTemperature();	//Take a temperature reading
			temp = MPL3115A2_ReadTemperature();
			printf("Temperature : %0.2f degree Celsius.\r\n", temp);
			delay_ms(500);  
			I2C_Initialize(MCP79410_ADDRESS);
			delay_ms(500);
			alrm_flag = 0;								//Reset alarm flag for next alarm 
			MCP79410_ClearInterruptFlag(ZERO);
			delay_ms(1000);
			digitalWrite(LED_PIN, LOW);					//Back to waiting time
			printf("Alarm :%d\r\n",(unsigned int)alrm_flag);
		}
		current_time = MCP79410_GetTime();
		printf("now: %d-%d-%d %d:%d:%d\n", current_time->year, current_time->month, current_time->date, current_time->hour, current_time->min, current_time->sec);		
		delay_ms(1000);
	}
	
	I2C_Close();				//Return I2C pins to default status

	return 0;
}




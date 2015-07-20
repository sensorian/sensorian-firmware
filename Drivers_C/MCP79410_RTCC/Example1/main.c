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
 * @file MCP79410_RTCC/Example1/main.c
 * @author D.Qendri
 * @date 30 Dec 2014
 * @brief Configure RTCC with system time.
 */
 
#include <stdio.h>
#include <stdbool.h>	
#include <stdlib.h>				
#include <time.h>	
#include "MCP79410.h"
#include "Utilities.h"
#include "i2c.h"

int main(int argc, char **argv)
{
	RTCC_Struct *current_time = (RTCC_Struct *) malloc(sizeof(RTCC_Struct));
	
	I2C_Initialize(MCP79410_ADDRESS);			//Initialize I2C and setup chip address	
	
	if(MCP79410_IsRunning())					//If clock is already running stop it 
	{
		MCP79410_DisableOscillator();		
	}
	
	MCP79410_Initialize();						//Initialize RTCC with system time and date 

	while(1)
	{	
		current_time = MCP79410_GetTime();
		printf("now: %d-%d-%d %d:%d:%d\n", current_time->year, current_time->month, current_time->date, current_time->hour, current_time->min, current_time->sec);		
		delay_ms(1000);
		break;
		
	}
	
	I2C_Close();								//Return I2C pins to default status

	return 0;
}



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
 * @file /APDS-9300/Example2/main.c
 * @author D.Qendri
 * @date 28 May 2015
 * @brief Demonstrates how to change the sensor gain.     
 */
 
#include <stdio.h>
#include <unistd.h>
#include "APDS9300.h"
#include "Utilities.h"
#include "i2c.h"

int main(int argc, char **argv)
{
	unsigned int i = 0;
	unsigned int channel1,channel2;
	
	I2C_Initialize(APDS9300ADDR);				//Initialize I2C and setup chip address	
	AL_Initialize();
	
	while(1)
	{	
		AL_SetGain(GAIN_1);    					//Set gain to 1x , take 10 consecutive readings		
		for(i=0;i<10;i++)
		{			
			channel1 = AL_ReadChannel(CH0);		
			channel2 = AL_ReadChannel(CH1);		
			printf("Gain 1: %d\t%d.\r\n" ,channel1,channel2);		
			delay_ms(500);			
		}
		
		AL_SetGain(GAIN_16);					//Set gain to 16x , take 10 consecutive readings		
		for(i=0;i<10;i++)
		{			
			channel1 = AL_ReadChannel(CH0);				
			channel2 = AL_ReadChannel(CH1);				
			printf("Gain 16: %d\t%d.\r\n" ,channel1,channel2);	
			delay_ms(500);
		}
	}	
	I2C_Close();								//Return I2C pins to default status
	return 0;
}
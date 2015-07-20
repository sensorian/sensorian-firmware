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
 * @file /APDS-9300/Example1/main.c
 * @author D.Qendri
 * @date 28 May 2015
 * @brief Demonstrates how to take an ambient light reading from the sensor.    
 */

#include <stdio.h>
#include <unistd.h>
#include "APDS9300.h"
#include "Utilities.h"
#include "i2c.h"

int main(int argc, char **argv)
{
	float lux =0;
	
	I2C_Initialize(APDS9300ADDR);							//Initialize I2C controller
	AL_Initialize();
	printf("Ambient light Chip ID: 0x%02X. \r\n",AL_ChipID());
	
	AL_SetGain(GAIN_16);									//Gain of 16x for low lighting conditions
	AL_SetSamplingTime(S3);
	AL_Clear_Interrupt();
	delay_ms(1000);
	
	while(1)
	{
		unsigned int channel1 = AL_ReadChannel(CH0);		//Take a reading from channel one
		printf("Channel one value: %d.\r\n" ,channel1);		
		unsigned int channel2 = AL_ReadChannel(CH1);		//Take a reading from channel two
		printf("Channel two value: %d.\r\n" ,channel2);
		lux = AL_Lux(channel1,channel2);
		printf("Lux value: %f.\r\n" ,lux);
		delay_ms(1000);
	}
	
	I2C_Close();								//Return I2C pins to default status

	return 0;
}
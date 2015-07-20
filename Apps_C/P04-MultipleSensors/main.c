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

 
#include <stdio.h>
#include <unistd.h>
#include "APDS9300.h"
#include "MPL3115A2.h"
#include "i2c.h"
#include "Utilities.h"

int main(int argc, char **argv)
{
	unsigned char i=3;
	
	I2C_Initialize(APDS9300ADDR);									//Initialize I2C with light sensor address
	
	char id = AL_Initialize();										//Setup Ambient light sensor 
	printf("Chip ID: 0x%02X. \r\n",id);
	delay_ms(1000);	
	
	while(i>0)
    {
		unsigned int channel1 = AL_ReadChannel(CH0);				//Take a reading from channel one
		printf("Channel one value: %d.\r\n" ,channel1);		
		unsigned int channel2 = AL_ReadChannel(CH1);				//Take a reading from channel two
		printf("Channel two value: %d.\r\n" ,channel2);
		
		delay_ms(1000);
		i--;
	}
	
	i = 3;
	
	I2C_Initialize(MPL3115A2_ADDRESS);								//Address barometer I2C sensor 
	MPL3115A2_StandbyMode();
	MPL3115A2_Initialize();											//Initialize the sensor 
	MPL3115A2_ActiveMode();											//Configure the sensor for active mode	
	id  = MPL3115A2_ID();											//Verify chip id
	printf("Chip ID: 0x%02X . \r\n", id);
	
	while(i>0)
    {	
		float temp = MPL3115A2_ReadTemperature();					//Take a temperature reading
		printf("Temperature : %0.2f degree Celsius.\r\n", temp);
		bcm2835_delay(500);
		
		MPL3115A2_StandbyMode();
		MPL3115A2_AltimeterMode();
        float alt = MPL3115A2_ReadAltitude();						//Take an altimeter reading
		printf("Altimeter: %0.2f m above sea level.\r\n", alt/100);
		bcm2835_delay(500);		
		MPL3115A2_StandbyMode();
		i--;
	}
	
	I2C_Close();													//Return I2C pins to default status

	return 0;
}


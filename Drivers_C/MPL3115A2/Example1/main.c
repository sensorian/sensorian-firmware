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
 * @file MPL3115A2/Example1/main.c
 * @author D.Qendri
 * @date 30 Arpil 2015
 * @brief Example on how to use the sensor API for printing live temperature data on the console.
 *
 */
 
#include <bcm2835.h>
#include <stdio.h>
#include "MPL3115A2.h"
#include "i2c.h"
#include "Utilities.h"

int main(int argc, char **argv)
{
	printf("MPL3115A2 Altimeter/Barometer/Temperature sensor demo.\r\n");
	
	I2C_Initialize(MPL3115A2_ADDRESS);			//Initialize I2C peripheral
	MPL3115A2_Initialize();		//Initialize the sensor 	
	MPL3115A2_ActiveMode();		//Configure the sensor for active mode
	
	unsigned char id  = MPL3115A2_ID();	//Verify chip id
	printf("Chip ID: 0x%02X . \r\n", id);
	
    while(1)
    {	     
		float temp = MPL3115A2_ReadTemperature();//Take a temperature reading
		printf("Temperature : %0.2f degree Celsius.\r\n", temp);
		delay_ms(500);        
    }

return 0;
}


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
 * @file MPL3115A2/Example3/main.c
 * @author D.Qendri
 * @date 30 April 2015
 * @brief Temperature calibration by adding/substracting offset factor.
 */
 
 
#include <bcm2835.h>
#include <stdio.h>
#include "MPL3115A2.h"
#include "i2c.h"
#include "Utilities.h"

int main(int argc, char **argv)
{
	I2C_Initialize(MPL3115A2_ADDRESS);		//Initialize I2C and setup chip address	
	MPL3115A2_Initialize();
	MPL3115A2_ActiveMode();	
	
	printf("MPL3115A2 Temperature Calibration demo.\r\n");
	
	unsigned char id  = MPL3115A2_ID();	//Verify chip id
	printf("Chip ID: 0x%02X . \r\n", id);
	unsigned char va = 0;
	float temp = 0;
	
    while(1)
    {	  	   
		temp = MPL3115A2_ReadTemperature();//Take a temperature reading
		printf("Temperature 1 : %0.2f degree Celsius.\r\n", temp);
		delay_ms(500);
		
		MPL3115A2_StandbyMode();
		MPL3115A2_SetTempOffset(-50);
		delay_ms(500);
		MPL3115A2_ActiveMode();	
		delay_ms(500);
		
		temp = MPL3115A2_ReadTemperature();//Take a new temperature reading
		printf("Temperature 2 : %0.2f degree Celsius.\r\n", temp);
		delay_ms(500);
		
		MPL3115A2_StandbyMode();
		MPL3115A2_SetTempOffset(-128);
		delay_ms(500);
		MPL3115A2_ActiveMode();	
		delay_ms(500);
		
		va = MPL3115A2_ReadByte(OFF_T);
		printf("Reg: %x. \r\n", va);        
    }

return 0;
}


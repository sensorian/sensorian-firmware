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
 * @file /CAP1203/Example1/main.c
 * @author D.Qendri
 * @date 28 May 2015
 * @brief Shows how to read single touch presses from the three on board buttons.
 */
 
#include <bcm2835.h>
#include "i2c.h"
#include "CAP1203.h"
#include "Utilities.h"

int main(int argc, char **argv)
{
	unsigned char button = 0x00;
	
	I2C_Initialize(CAP1203ADDR);
    CAP1203_Initialize();

	int id = CAP1203_ReadID();							//Read ID of Capacitive touch controller 
	printf("Chip ID: 0x%04X. \r\n",id);

	while(1)
	{
		button = CAP1203_ReadPressedButton();
		
		if(button != 0)
		{
			printf("Button B%d pressed.\r\n" ,button);
		}
		delay_ms(500);
		
	}
    I2C_Close();
	
    return 0;
}


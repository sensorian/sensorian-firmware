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
#include "bcm2835.h"
#include "Utilities.h"
#include "SPI.h"
#include "TFT.h"
#include "GFX.h"
#include "i2c.h"
#include "CAP1203.h"

extern const unsigned char FontASCII8X16[] ;

int main(void)
{
	SPI_Initialize();				//Initialize SPI peripheral
	TFT_Initialize();
	I2C_Initialize(CAP1203ADDR);
	
    CAP1203_Initialize();
	
	unsigned char button = 0x00;
	
	#ifdef __DEBUG__
		int id = CAP1203_ReadID();							//Read ID of Capacitive touch controller 
		printf("Chip ID: 0x%04X. \r\n",id);
	#endif
	
	TFT_Background(WHITE);
	TFT_Circle(80,35,20,RED);
	TFT_Circle(80,80,20,GREEN);
	TFT_Circle(80,125,20,BLUE);
	  
	while(1)
	{
		button = CAP1203_ReadPressedButton();
			switch(button)
			{
				case 1:
					TFT_Disk(80,125,10,BLUE);
					printf("Button B1 pressed.\r\n");
					break;
				case 2:
					TFT_Disk(80,80,10,GREEN);
					printf("Button B2 pressed.\r\n");
					break;					
				case 3:
					TFT_Disk(80,35,10,RED);
					printf("Button B3 pressed.\r\n");
					break;				
				default:
					TFT_Circle(80,35,20,RED);				//Paint two circles for an annular ring
					TFT_Circle(80,80,20,GREEN);
					TFT_Circle(80,125,20,BLUE);	
					TFT_Circle(80,35,19,RED);
					TFT_Circle(80,80,19,GREEN);
					TFT_Circle(80,125,19,BLUE);
					
					TFT_Disk(80,35,10,WHITE);				//Clear interior
					TFT_Disk(80,80,10,WHITE);
					TFT_Disk(80,125,10,WHITE);
					break;
			}	
		delay_ms(100);		
    }
    return 0;
}

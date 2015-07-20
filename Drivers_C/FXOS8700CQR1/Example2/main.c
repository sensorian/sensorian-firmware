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
 * @file FXOS8700CQR1/Example2/main.c
 * @author D.Qendri
 * @date 30 May 2015
 * @brief Demonstrates embedded orientation capabilities.
 *
 */
 
 
#include <stdio.h>
#include "Utilities.h"
#include "FXOS8700CQ.h"
#include "i2c.h"

int main(int argc, char **argv)
{	
	char orienta = 0;
	
	I2C_Initialize(FXOS8700CQ_ADDRESS);						//Initialize I2C and setup chip address		
	FXOS8700CQ_Initialize();				//Setup chip in hybrid mode	
	FXOS8700CQ_StandbyMode();	
	FXOS8700CQ_WriteByte(SYSMOD,0x01);		//Set to wake up 	
	FXOS8700CQ_ActiveMode();
	char id = FXOS8700CQ_ReadByte(PL_BF_ZCOMP);
	printf("PL_BF_ZCOMP: 0x%02X. \r\n",id);
	
	FXOS8700CQ_ConfigureOrientation();
	
	unsigned char mode = 0;
	while(1)
	{	
		if(FXOS8700CQ_ReadStatusReg() & 0x80)
		{	
			orienta = FXOS8700CQ_GetOrientation();		 
			printf("\r\nRegister: %02X \r\n", orienta);
			
			mode = (orienta >> 1) & 0x03;			//mask the rets of the bits
			
			switch(mode)
			{			
				case 0:
					printf("Landscape right.\r\n");
					break;
				case 1:
					printf("Landscape left. \r\n");
					break;				
				case 2:
					printf("Portrait down.\r\n");
					break;
				case 3:
					printf("Portrait up.\r\n");
					break;
				default:           
					printf("Nothing here");
					break;					
			}			
			delay_ms(500);	
		}			
	}
	
	printf("Shutting down");	
	I2C_Close();				//Return I2C pins to default status

	return 0;
}

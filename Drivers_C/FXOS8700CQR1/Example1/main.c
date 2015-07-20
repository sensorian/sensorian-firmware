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
 * @file FXOS8700CQR1/Example1/main.c
 * @author D.Qendri
 * @date 30 May 2015
 * @brief Print magnetometer data to terminal.
 *
 */
 
#include <stdio.h>
#include "Utilities.h"
#include "FXOS8700CQ.h"
#include "i2c.h"

int main(int argc, char **argv)
{
	unsigned char id = 0;
	
	rawdata_t accelerometerBuffer;	
	rawdata_t magnetometerBuffer;
	
	I2C_Initialize(FXOS8700CQ_ADDRESS);			//Initialize I2C and setup chip address		
	FXOS8700CQ_Initialize();	//Setup chip in hybrid mode
	
	id = FXOS8700CQ_ID();		//Verify chip ID
	printf("Chip ID: 0x%02X. \r\n",id);
	
	FXOS8700CQ_StandbyMode();
	
	FXOS8700CQ_WriteByte(SYSMOD,0x01);		//Set to wake up 
	
	FXOS8700CQ_ActiveMode();
	
	id = FXOS8700CQ_GetChipMode();	//Setup sensor as accelerometer
	printf("Chip Mode: 0x%02X. \r\n",id);	
	
	id = FXOS8700CQ_ReadByte(PL_BF_ZCOMP);
	printf("PL_BF_ZCOMP: 0x%02X. \r\n",id);
	
	while(1)
	{
		 if(FXOS8700CQ_ReadStatusReg() & 0x80)
		 {	
			FXOS8700CQ_GetData(&accelerometerBuffer,&magnetometerBuffer);
			printf("Magnetometer data x: %d, y: %d, z: %d \r\n" , magnetometerBuffer.x, magnetometerBuffer.y, magnetometerBuffer.z);	
		 }
	}
	
	printf("Shutting down");	
	I2C_Close();				//Return I2C pins to default status

	return 0;
}
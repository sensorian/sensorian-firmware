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
 * @file FXOS8700CQR1/Example3/main.c
 * @author D.Qendri
 * @date 30 May 2015
 * @brief Demonstrates embedded tap detection capabilities.
 * 		Tap and double tap based API functions.
 *
 */
 
#include <stdio.h>
#include "FXOS8700CQ.h"
#include "i2c.h"

int main(int argc, char **argv)
{
	I2C_Initialize(FXOS8700CQ_ADDRESS);						//Initialize I2C and setup chip address		
	FXOS8700CQ_Initialize();								//Setup sensor in hybrid mode	

	FXOS8700CQ_ConfigureSingleTapMode();

	char Int_SourceSystem = 0;
	char Int_SourcePulse = 0;
	
	while(1)
	{	
		Int_SourceSystem =FXOS8700CQ_ReadByte(INT_SOURCE);			//Read interrupt source
		
		if((Int_SourceSystem & 0x08) == 0x08)
		{
			printf("Tap event detected.\r\n");							//Perform an Action since Pulse Flag has been Set
			Int_SourcePulse = FXOS8700CQ_ReadByte(PULSE_SRC);			//Read the Pulse Status Register to clear the system interrupt
			printf("Pulse %02x.\r\n",Int_SourcePulse);
		}	
	}

	I2C_Close();				//Return I2C pins to default status

	return 0;
}
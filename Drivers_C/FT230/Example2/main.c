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
 * @file FT230/Example2/main.c
 * @author D.Qendri
 * @date 30 May 2015
 * @brief Echo example with a serial terminal.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include "Serial.h"

int main(int argc, char* argv[]) 
{
    unsigned int ser = SerialOpen(115200);      //Open serial port with a 115200 baud rate
	int a = 0;
    SerialFlush(ser);				//Clear TX and RX buffer, can be called periodically

    SerialPuts(ser,"Serial communication using the Sensorian Shield.\r\n");
    SerialPrintf(ser,"You are using device number %d.\r\n",ser);
	SerialFlush(ser);
	
	while(1)
	{	if(SerialDataAvail(ser))
		{
		a = SerialGetchar(ser);
		SerialPrintf(ser,"You sent %c.\r\n",(char)(a));
		if(a == 'b') break;
		}
	}
    SerialPrintf(ser,"Closing serial port ...");
    SerialClose(ser);				//Close serial port
	
    return 0;
}


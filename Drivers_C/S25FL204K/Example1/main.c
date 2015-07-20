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
 * @file S25FL204K/Example1/main.c
 * @author D.Qendri
 * @date 30 May 2015
 * @brief Read status registers and manufacturer ID.
 */
 
#include <stdio.h>
#include "bcm2835.h"
#include "S25FL204K.h"
#include "SPI.h"

int main(void)
{
	SPI_Initialize();								//Initialize SPI peripheral
	S25FL_Setup();                                  //Setup flash memory
	S25FL_SReg_Write(0x00);                         //Remove protection from all blocks writes to SR1

	printf("NOR Flash memory demo.\r\n");
	printf("This example will read the chip factory ID \r\n and display the contents of the status registers.\r\n");

	unsigned int id = S25FL_ReadID();           	//read id - 0x01 ox16
	printf("Manufacturer id: %d \n",id); 
	unsigned long a = S25FL_Jedec_ID_Read();    	//0x01 0x40 0x17
	printf("Jedec ID: %ld \n",a);

	unsigned char cell = S25FL_SReg_Read(STAT_REG1);
	printf("Status register one: 0x%02X \n",cell); 
	cell = S25FL_SReg_Read(STAT_REG2);
	printf("Status register two: 0x%02X \n",cell); 
	cell = S25FL_SReg_Read(STAT_REG3);
	printf("Status register three: 0x%02X \n",cell);

	printf("End of demo.");
	SPI_Close();					//Close SPI peripheral

return 0;
}


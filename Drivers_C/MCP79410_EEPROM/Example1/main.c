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
 * @file MCP79410_EEPROM/Example1/main.c
 * @author D.Qendri
 * @date 30 May 2015
 * @brief Reading and writing from the 1 Kbit EEPROM.
 *
 */
 
#include <stdio.h>
#include <stdbool.h>	
#include <stdlib.h>				
#include <time.h>	
#include "EEPROM.h"
#include "Utilities.h"
#include "i2c.h"

#define DEL 100

int main(int argc, char **argv)
{
	char wr_buff1[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x7,0x08};
	char wr_buff2[8] = {0xa1,0xb2,0xc3,0xd4,0xe5,0xf6,0xa7,0x0a8};
	char rd_buffer[8] = {0};
	unsigned char i = 0;
	
	I2C_Initialize(EEPROM_ADDRESS);

	printf("Status reg: 0x%2x. \r\n", EEPROM_Read(EEPROM_SREG));
	
	EEPROM_Write(EEPROM_START,0xae);
	delay_ms(DEL);
	EEPROM_Write(0x01,0x99);
	delay_ms(DEL);
	EEPROM_Write(0x02,0xa9);
	delay_ms(DEL);
	EEPROM_Write(0x03,0xc9);
	delay_ms(DEL);
	EEPROM_Write(0x04,0xd9);
	delay_ms(DEL);
	
	printf("Reading individual bytes. \r\n");
	printf("EE0 0x%2x. \r\n", EEPROM_Read(EEPROM_START));
	printf("EE1 0x%2x. \r\n", EEPROM_Read(EEPROM_START+1));
	printf("EE2 0x%2x. \r\n", EEPROM_Read(EEPROM_START+2));
	printf("EE3 0x%2x. \r\n", EEPROM_Read(EEPROM_START+3));
	printf("EE4 0x%2x. \r\n", EEPROM_Read(EEPROM_START+4));
	
	printf("Writing individual pages. \r\n");
	EEPROM_WritePage(EEPROM_START,wr_buff1);
	delay_ms(DEL);
	printf("Reading individual pages. \r\n");
	
	EEPROM_ReadPage(EEPROM_START,rd_buffer);
	for(i =0;i<8;i++)
	{
		printf("EE%d 0x%2x. \r\n", i,rd_buffer[i]);
	}
	
	printf("Writing individual pages. \r\n");
	EEPROM_WritePage(EEPROM_START+8,wr_buff2);
	delay_ms(DEL);
	printf("Reading individual pages. \r\n");
	EEPROM_ReadPage(EEPROM_START+8,rd_buffer);
	for(i =0;i<8;i++)
	{
		printf("EE%d 0x%2x. \r\n", i,rd_buffer[i]);
	}
	
	I2C_Close();								//Return I2C pins to default status

	return 0;
}



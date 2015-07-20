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
 * @file MCP79410_EEPROM/Example1/i2c.h
 * @author D.Qendri
 * @date 30 May 2015
 * @brief I2C driver header.
 *
 */
 
#ifndef __I2C1_H__
#define __I2C1_H__

#include <stdio.h>
#include "bcm2835.h"

#define baudrate		100000

void 			I2C_Initialize(unsigned char address);

void 			I2C_WriteByte(char byte);;
void 			I2C_WriteByteRegister(unsigned char reg,unsigned char data);
void 			I2C_WriteWordRegister(unsigned char reg, unsigned char* data);
void 			I2C_WriteByteArray(char reg, char* data, unsigned int length);

unsigned char 	I2C_ReadByteRegister(char reg);
void 			I2C_ReadByteArray(char reg,char *buffer,unsigned int  length);
unsigned int 	I2C_ReadWordRegister(char reg);
unsigned int 	I2C_ReadWordRegisterRS(char reg);
unsigned int 	I2C_ReadWordPresetPointer(void);

void 			I2C_Close(void);

#endif


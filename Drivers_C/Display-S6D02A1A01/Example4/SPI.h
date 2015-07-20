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
 * @file Display/Example4/SPI.h
 * @author D.Qendri
 * @date 30 May 2015
 * @brief SPI library header.   
 */
 
#ifndef __SPI_H__

#include "bcm2835.h"

void            SPI_Initialize(void);
unsigned char   SPI_Write(unsigned char data);
void 			SPI_Write_Array(char* DATA, unsigned int length);
unsigned char 	SPI_Read(void);
void 			SPI_Read_Array(char* sArray, char* rArray, char length);
void 			SPI_Close(void);

#endif

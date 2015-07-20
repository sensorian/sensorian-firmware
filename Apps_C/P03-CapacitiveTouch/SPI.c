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
 * @file Display/Example1/SPI.c
 * @author D.Qendri
 * @date 30 May 2015
 * @brief SPI library.   
 */
 
#include "SPI.h"
#include <stdio.h>

/**
 *@brief Initializes the SPI peripheral
 *@return none
 */
void SPI_Initialize(void)
{	if (!bcm2835_init())
	{
		printf("BCM libray error.\n");			//Should be run with the sudo cmd
	}
	bcm2835_spi_begin();						//Configure SPI pins
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      	//Configure bit order
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);  			//Set clock polarity and phase CPOL=0, CPHA=0
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_8);	//SPI baud rate at 244 Khz
	bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE);			//Control CE0 in software
	printf("SPI initialized...\n");
}

/**
 *@brief Writes a byte of data to the SPI bus
 *@param data Byte data to write on bus
 *@return result Result of data written on SPI bus.
 */
unsigned char SPI_Write(unsigned char data)
{
	return bcm2835_spi_transfer(data);
}

/**
 *@brief Reads a byte of data from the SPI bus
 *@return result Result of data written on SPI bus.
 */
unsigned char SPI_Read(void)
{
	unsigned char data = bcm2835_spi_transfer(0xff);
	return data;
}

/**
 *@brief Writes an array of bytes to the SPI bus
 *@param buff Pointer to array of char data.
 *@param length Length of data array.
 *@return none
 */
void SPI_Write_Array(char* buff, unsigned int length)
{
	bcm2835_spi_writenb(buff,length);
}

/**
 *@brief Reads an array of data from the SPI bus
 *@param sArray Buffer of data to send  
 *@param rArray Buffer of data to send  
 *@param length Length of data buffer
 *@return none
 */
void SPI_Read_Array(char* sArray, char* rArray, char length)
{
	bcm2835_spi_transfernb(sArray,rArray,length);
}

/**
 *@brief Close SPI bus, should not be called if another SPI device is being used
 *@return none
 */
void SPI_Close(void)
{
	bcm2835_spi_end();
}

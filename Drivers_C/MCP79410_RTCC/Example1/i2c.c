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
 * @file MCP79410_RTCC/Example1/i2c.c
 * @author D.Qendri
 * @date 30 May 2015
 * @brief I2C driver.
 *
 */
 
#include <stdlib.h>
#include "i2c.h"

/**
 *@brief Initializes the I2C peripheral
 *@param address Address the I2C peripheral is communicating with.
 *@return none
 */
void I2C_Initialize(unsigned char address)
{    					
	if (!bcm2835_init())						//Configure I2C pins
	{
		printf("BCM libray error.\n");
	}
	bcm2835_i2c_end();		//Close I2C peripheral to reconfigure it
	
	bcm2835_i2c_begin();						//Set pins as I2C
	bcm2835_i2c_set_baudrate(baudrate);			//Set I2C baudrate
	bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_2500);		//100 Khz
	bcm2835_i2c_setSlaveAddress(address);	//Set device address
}

/**
 *@brief Writes a byte value to the I2C bus. This assumes the register pointer is preset.
 *@param bdata	Send single byte to I2C bus
 *@return none
 */
void I2C_WriteByte(char bdata)
{	char data = bdata;
	bcm2835_i2c_write(&data, 1);
}

/**
 *@brief Writes a byte value to a register address
 *@param reg Address of sensor register.
 *@param data Data byte to be written on register.
 *@return none
 */
void I2C_WriteByteRegister(unsigned char reg,unsigned char data)
{
	unsigned char wr_buf[2];

	wr_buf[0] = reg;
	wr_buf[1] = data;

	bcm2835_i2c_write((const char *)wr_buf, 2);
}

/**
 *@brief Writes a word value (16 bit) to a register address.
 *@param reg Address of sensor register.
 *@param data Data word to be written on word size register.
 *@return none
 */
void I2C_WriteWordRegister(unsigned char reg, unsigned char* data)
{
	unsigned char wr_buf[3];
	
	wr_buf[0] = reg;
	wr_buf[1] = data[0];
	wr_buf[2] = data[1];

	bcm2835_i2c_write((const char *)wr_buf, 3);
}

/**
 *@brief Writes a buffer array to the registers
 *@param reg	Address of sensor register, address autoincrements
 *@param data	Pointer to byte data buffer array
 *@param length	length of buffer array
 *@return none
 */
void I2C_WriteByteArray(char reg, char* data, unsigned int length)
{
	char* wr_buf = (char*) malloc(sizeof(char) * length);
	if (wr_buf==NULL) 
	{
		printf("Error allocating memory!\n"); //print an error message
	}
	
	wr_buf[0] = reg;
	for(unsigned int i = 1;i<length;i++)
	{
		wr_buf[i] = data[i];
	}

	bcm2835_i2c_write((const char *)wr_buf, length);
}

/**
 *@brief Reads a byte from a register
 *@param reg Address of sensor register.
 *@return val Byte value of register.
 */
unsigned char I2C_ReadByteRegister(char reg)
{
	char val = 0;
 
	bcm2835_i2c_read_register_rs(&reg,&val,1);
	
	return val;
 }
 
 /**
 *@brief Initializes the I2C peripheral
 *@param reg	Address of sensor register, address autoincrements
 *@param *buffer	Pointer to byte data buffer array
 *@param length	length of buffer array
 *@return none
 */
void I2C_ReadByteArray(char reg,char *buffer,unsigned int  length)
{	
	bcm2835_i2c_read_register_rs(&reg,buffer,length);
}

 /**
 *@brief Readm result from a word length register
 *@param reg register to read from
 *@return val Word value of register
 */
unsigned int I2C_ReadWordRegisterRS(char reg)
{
   	char cmd[1] = {reg}; 
	char receive[2] = {0};
	bcm2835_i2c_write_read_rs(cmd,1,receive,2);
	
	return (receive[0]<<8)|receive[1];
}

/**
 *@brief Read the value of a register that has already been select via the address pointer
 *@return Data Value of preset register
 */
unsigned int I2C_ReadWordPresetPointer(void)
{
	char val[2] = {0}; 
	bcm2835_i2c_read(val,2);
	unsigned int data = (val[0] << 8)|val[1];
	
	return data;
 }
 
 /**
 *@brief Closes the I2C peripheral
 *@return none
 */
 void I2C_Close(void)
 {
	bcm2835_i2c_end();
}


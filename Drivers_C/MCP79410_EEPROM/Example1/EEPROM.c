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
 * @file MCP79410_EEPROM/Example1/EEPROM.c
 * @author D.Qendri
 * @date 30 May 2015
 * @brief EEPROM driver.
 *
 */
 
#include "EEPROM.h"
#include "i2c.h"
#include <time.h>
#include <stdlib.h>

/// \defgroup eeprom EEPROM 
/// These functions allow the user to use the 1 Kbit nonvolatile EEPROM memory of the MCP979140.
/// @{


/**
 * @brief This function writes a single byte in the I2C EEPROM.
 * @param mem_address Memory register to write to.
 * @param ee_data Byte data for register.
 * @return none
 */
void EEPROM_Write(unsigned char mem_address, unsigned char ee_data)
{
    I2C_WriteByteRegister(mem_address,ee_data);
}  


/**
 * @brief The function writes to the protected EEPROM space.
 * @param mem_address Memory register to write to.
 * @param ee_data Content to write on protected EEPROM space.
 * @return none
 */
void  EEPROM_WriteProtected(unsigned char mem_address, unsigned char ee_data)
{
	I2C_WriteByteRegister(EEUNLOCK,0x55);
	I2C_WriteByteRegister(EEUNLOCK,0xaa);
    I2C_WriteByteRegister(mem_address,ee_data);
}  


/**
 * @brief The function reads a data byte from the EEPROM
 * @param mem_address Address of register.
 * @return reg Register content.
 */
unsigned char  EEPROM_Read(unsigned char mem_address)
{
    return I2C_ReadByteRegister(mem_address);    	
}


/**
 * @brief This function sets the Block protection settings.
 * @param protSetting  EEPROM protection settings
 * @return none
 */
void EEPROM_SetBlockProtection(ProtectionSettings_t protSetting)
{
	I2C_WriteByteRegister(EEPROM_SREG,protSetting);
}


/**
 * @brief This function writes a single page to the RTCC EEPROM
 * @param mem_address  EEPROM address to write to
 * @param buffer Page buffer
 */
void  EEPROM_WritePage(char mem_address,char *buffer)
{
	I2C_WriteByteArray(mem_address,buffer,8);
}

/**
 * @brief This function reads a single page from the RTCC EEPROM
 * @param mem_address  EEPROM address to read to
 * @param buffer Page buffer
 */
void  EEPROM_ReadPage(char mem_address,char *buffer)
{
	I2C_ReadByteArray(mem_address,buffer,8);
}

/// @}
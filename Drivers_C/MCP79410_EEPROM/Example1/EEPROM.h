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
 * @file MCP79410_EEPROM/Example1/EEPROM.h
 * @author D.Qendri
 * @date 30 May 2015
 * @brief EEPROM driver header.
 *
 */
 
#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "bcm2835.h"

/*! \def MCP79410_ADDRESS
    \brief The I2C address of the MCP79410 EEPROM
*/

#define  EEPROM_ADDRESS         0x57

#define  EEPROM_WRITE           0xae       //  DEVICE ADDR for EEPROM (writes)   
#define  EEPROM_READ            0xaf       //  DEVICE ADDR for EEPROM (reads)  
#define  EEUNLOCK               0x09

#define  EEPROM_START           0x00
#define  EEPROM_END             0x7F

#define  EEPROM_PROTECTED_START 0xF0
#define  EEPROM_PROTECTED_END   0xF7

#define  BP0                    0x04
#define  BP1                    0x08

/************************* RTCC Memory map ****************************/

#define  EEPROM_WRITE           0xae       //  DEVICE ADDR for EEPROM (writes)   
#define  EEPROM_READ            0xaf       //  DEVICE ADDR for EEPROM (reads)  

#define  SRAM_PTR               0x20       //  pointer of the SRAM area (RTCC) 
#define  EEPROM_SREG            0xff       //  STATUS REGISTER in the  EEPROM

/*********************************************************************/

/*! ProtectionSettings_t class */
typedef enum PROTECTION {QUARTER = 0x01, /*!< The first quarter of the EEPROM space is protected */
						HALF = 0x02, 	/*!< Half of the EEPROM space is protected */
						ALL = 0x03		/*!< All the EEPROM space is protected */
						} ProtectionSettings_t;

#define TRUE	1
#define FALSE	0

/***************************Function definitions********************************************/

void            EEPROM_Write(unsigned char mem_address, unsigned char ee_data); 
void            EEPROM_WriteProtected(unsigned char mem_address, unsigned char ee_data);
unsigned char   EEPROM_Read(unsigned char mem_address); 
void            EEPROM_SetBlockProtection(ProtectionSettings_t protSetting);
void            EEPROM_ReadPage(char mem_address,char *buffer);
void            EEPROM_WritePage(char mem_address,char *buffer);


#endif


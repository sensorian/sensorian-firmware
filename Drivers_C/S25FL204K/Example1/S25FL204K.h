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
 * @file S25FL204K/Example1/S25FL204K.h
 * @author D.Qendri
 * @date 30 May 2015
 * @brief S25FL204K driver header.
 */
 
#ifndef S25FL204K_H
#define	S25FL204K_H

#include "bcm2835.h"

/******************************* Commands ************************************/
#define CMD_RDSR1     	(unsigned char) 0x05
#define CMD_RDSR2     	(unsigned char) 0x35
#define CMD_RDSR3     	(unsigned char) 0x33
#define CMD_WREN    	(unsigned char) 0x06
#define CMD_EWSR     	(unsigned char) 0x50            //write enable volatile
#define CMD_WRDI		(unsigned char) 0x04
#define CMD_SETBURST    (unsigned char) 0x77
#define PAGE_PROGRAM	(unsigned char) 0x02

#define CMD_READ_DATA   (unsigned char) 0x03
#define CMD_READ_HS  	(unsigned char) 0x0B

#define ERASE_SECTOR	(unsigned char) 0x20
#define ERASE_BLOCK32   (unsigned char) 0x52
#define ERASE_BLOCK64	(unsigned char) 0xD8
#define ERASE_CHIP		(unsigned char) 0x60

#define CMD_DEEPSLP     (unsigned char) 0xB9
#define CMD_RESDPD		(unsigned char) 0xAB            //release from sleep
#define CMD_RDID     	(unsigned char) 0x90
#define JEDEC			(unsigned char)	0x9F
#define CMD_RDSFDP      (unsigned char) 0x5A
#define CMD_RDSECREG    (unsigned char) 0x48
#define CMD_ERSECREG    (unsigned char) 0x44            //erase security reg
#define CMD_EHLD		(unsigned char) 0xAA

#define S64MANID        (unsigned char) 0x01
#define S64DEVID        (unsigned char) 0x16

#define CMD_RSID		(unsigned char) 0x88
#define CMD_PSID		(unsigned char)	0xA5
#define CMD_LSID		(unsigned char)	0x85

				//resume from deep sleep
#define CMD_WRSR     	(unsigned char) 0x01            //write status register
#define STAT_REG1       (unsigned char)	0x05            //status register number one
#define STAT_REG2       (unsigned char)	0x35            //status register number two
#define STAT_REG3       (unsigned char)	0x33            //status register mumber three

/***********************Memory map*********************************************/

//sector 4 kbyte
//block 64 kbyte

#define SECTOR_SIZE         4096        //0x000-0xfff
#define SECTOR_COUNT        128
#define PAGE_NUM            2048        //16 pages per sector
#define PAGE_INCR           0x100       //256 bytes


/******************************** Chip Pins ***********************************/

#define CE_OUTPUT()		bcm2835_gpio_fsel(RPI_V2_GPIO_P1_26,BCM2835_GPIO_FSEL_OUTP);	//Set CE0 as output
#define CE_DESELECT() 	bcm2835_gpio_write(RPI_V2_GPIO_P1_26, HIGH);					
#define CE_SELECT()		bcm2835_gpio_write(RPI_V2_GPIO_P1_26, LOW);

 	
void            S25FL_Setup(void);
void            S25FL_WriteByte(unsigned char data, unsigned long address);
unsigned char   S25FL_ReadByte(unsigned long address);
void		    S25FL_WriteArray(unsigned long address, unsigned char* pData, unsigned int arrayLength);
void            S25FL_ReadArray(unsigned long address, unsigned char* pData, unsigned int arrayLength);

void            S25FL_ReadPage(unsigned long page,unsigned char buffer[]);

unsigned long   S25FL_Jedec_ID_Read();
unsigned int   	S25FL_ReadID();

void            S25FL_ChipErase(void);
void            S25FL_SectorErase(unsigned long address);
void            S25FL_BlockErase(unsigned long address);

void 			S25FL_SetBlockProtection(unsigned char prot);
void            S25FL_WriteEnable();
unsigned char   S25FL_IsWriteBusy();

unsigned char   S25FL_SReg_Read(unsigned char statReg);
void            S25FL_SReg_Write(unsigned char statReg);

void 			S25FL_DeepSleep(void);
void 			S25FL_WakeUp(void);

#endif	/* S25FL204K_H */


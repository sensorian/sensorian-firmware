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
 * @file S25FL204K/Example1/S25FL204K.c
 * @author D.Qendri
 * @date 30 May 2015
 * @brief S25FL204K driver.
 */
 
#include "S25FL204K.h"
#include "SPI.h"

/// \defgroup storage Serial Flash Storage
/// These functions allow you the use the serial flash storage.
/// @{

/**
 *@brief Setup SPI and IOs connected to Serial FLASH
 *@return none
 */	
void S25FL_Setup(void)
{
	CE_OUTPUT();				//Set CE0 pin as output
	CE_DESELECT();				//Deselect CE0 pin
}

/**
 *@brief Write a byte to the address specified (24 bit address)
 *@param data Byte data to be written on a specific address
 *@param address 24 bit address of the flash memory
 *@return none
 */
void S25FL_WriteByte(unsigned char data, unsigned long address)
{
    CE_SELECT();                                //Enable device
    SPI_Write(CMD_WREN);						//Send WREN command
    CE_DESELECT();                              //Disable device

    CE_SELECT();                                //Start SPI communication
    SPI_Write(PAGE_PROGRAM);					//Program page
    SPI_Write((unsigned char)(address >> 16) & 0x000000FF);	//Send 3 address bytes
    SPI_Write((unsigned char)(address >>  8) & 0x000000FF);
    SPI_Write((unsigned char)(address        & 0x000000FF));
    SPI_Write(data);
    CE_DESELECT();                   			//Disable device

    while ((S25FL_SReg_Read(CMD_RDSR1) & 0x01) == 0x01)	//Waste time until not busy
    {

    }
}

/**
 *@brief Read a byte from the specified 24 bit address
 *@param address Destination Address 000000H - 7FFFFFH
 *@return mybyte Data located on address
 */
unsigned char S25FL_ReadByte(unsigned long address)
{
    unsigned char mybyte = 0;
    CE_SELECT();                                 //Enable device
    SPI_Write(CMD_READ_DATA);                    //Read command,Send 3 address bytes
    SPI_Write((unsigned char)(address >> 16) & 0x000000FF);
    SPI_Write((unsigned char)(address >>  8) & 0x000000FF);
    SPI_Write((unsigned char)(address        & 0x000000FF));
    mybyte = SPI_Read();
    CE_DESELECT();                               //Disable device
    return mybyte;                               //Return one byte read
}

/**
 *@brief Write data array at the specified address , this procedure does page programming.
 *	     The destination address should be provided. This function is not safe so the user has to check the arguments
 *@param address Destination Address 000000H - 7FFFFFH
 *@param pData Pointer to array of data
 *@param arrayLength  Length of pointer to array of data
 *@return none
 */
void S25FL_WriteArray(unsigned long address, unsigned char* pData, unsigned int arrayLength)
{
    unsigned int i = 0;
    CE_SELECT();						//Enable device
    SPI_Write(CMD_WREN);				//Send WREN command
    CE_DESELECT();						//Disable device

    CE_SELECT();						//Enable device
    SPI_Write(PAGE_PROGRAM); 			//Send Byte Program command
    SPI_Write((unsigned char)(address >> 16) & 0xFF);
    SPI_Write((unsigned char)(address >>  8) & 0xFF);
    SPI_Write(0x00);

    for (i=0;i<arrayLength;i++)
    {
        SPI_Write(pData[i]);			//Send byte to be programmed 
    }
    CE_DESELECT();				//Disable device
 
    //Wait Busy
    while ((S25FL_SReg_Read(CMD_RDSR1) & 0x01) == 0x01)	//Waste time until not busy
    {
    }
}

/**
 *@brief Read an array of data from the given address
 *@param address Destination Address 000000H - 7FFFFFH
 *@param pData Pointer to array of data
 *@param arrayLength Length of pointer to array of data
 *@return none
 */
void S25FL_ReadArray(unsigned long address, unsigned char* pData, unsigned int arrayLength)
{
    unsigned int i = 0;

    CE_SELECT();                                    //Enable device
    SPI_Write(CMD_READ_DATA);                       //Read command
    SPI_Write((unsigned char)(address >> 16));
    SPI_Write((unsigned char)(address >>  8));
    SPI_Write((unsigned char) address);

    for (i = 0; i <arrayLength; i++)                //Read until no_bytes is reached
    {
    	pData[i] = SPI_Read();                      //Receive bytes
    }
    CE_DESELECT();                                  //Disable device

    //Wait Busy
    while (S25FL_SReg_Read(CMD_RDSR1) & 0x01)      //Waste time until not busy
    {
    }
}

/**
 *@brief This function reads the  Manufacturer ID code
 *		 This procedure Reads the manufacturer's ID and device ID.
 *@return deviceID 16 bit device id
 */
unsigned int S25FL_ReadID(void)
{
    unsigned int deviceID = 0;
    unsigned char man, dev = 0;
    CE_SELECT();                      //Start SPI communication
    SPI_Write(CMD_RDID);              //Send read ID command (90h or ABh)
    SPI_Write(0x00);                  //Send dummy byte
    SPI_Write(0x00);                  //Send dummy byte
    SPI_Write(0x00);                  //Send 0x00
    man = SPI_Read();                 //Send either manufacturer ID or device ID
    dev = SPI_Read();
    deviceID = (man << 8) | dev;      //Receive byte
    CE_DESELECT();                    //Disable device
    return deviceID;
}

 /**
 *@brief This procedure Reads the manufacturer's ID (BFh), memory type (25h)
 * and device ID (4Bh).  It will use 9Fh as the JEDEC ID command.
 *@return deviceID 16 bit device id
 */
unsigned long S25FL_Jedec_ID_Read(void)
{
    unsigned long int jedid = 0;
    unsigned char byte1, byte2, byte3 = 0;
    CE_SELECT();                                //Enable device
    SPI_Write(JEDEC);                           //Send JEDEC ID command (9Fh)    
    byte1 = SPI_Read();
    byte2 = SPI_Read();
    byte3 = SPI_Read();
    jedid = ((unsigned long)byte1 << 16)| (byte2 << 8) | byte3;
    CE_DESELECT();                              //Disable device
    return jedid;
}

/**
 *@brief Erase all chip contents. Erase is slow on flash chips. 
 *@return none
 */
void S25FL_ChipErase(void)
{
    CE_SELECT();				//Enable device
    SPI_Write(CMD_WREN);		//Send WREN command
    CE_DESELECT();				//Disable device
	
    CE_SELECT();                //Enable device
    SPI_Write(ERASE_CHIP);		//Send Sector Erase command
    CE_DESELECT();				//Disable device
}

/**
 *@brief This function erases a 4Kb sector (it erase a page actually)
 *@param address Address of the sector. Should start on a even boundary.
 *@return none
 */
void S25FL_SectorErase(unsigned long address)
{
    CE_SELECT();				//Enable device
    SPI_Write(CMD_WREN);		//Send WREN command
    CE_DESELECT();				//Disable device

    CE_SELECT();				//Enable device
    SPI_Write(ERASE_SECTOR);            	//Send Sector Erase command
    SPI_Write((address & 0x0000FFFF)>>8);
    SPI_Write(address & 0x000000FF);
    SPI_Write(0x00);
    CE_DESELECT();              //Disable device
}

/**
 *@brief This function erases a 64Kb block 
 *@param address Block address.
 *@return none
 */
void S25FL_BlockErase(unsigned long address)
{
    CE_SELECT();					//Enable device
    SPI_Write(CMD_WREN);			//Send WREN command
    CE_DESELECT();					//Disable device

    CE_SELECT();					//Enable device
    SPI_Write(ERASE_BLOCK64);       //Send Sector Erase command
    SPI_Write((unsigned char)(address >> 16) & 0x000000FF);
    SPI_Write((unsigned char)(address >>  8) & 0x000000FF);
    SPI_Write((unsigned char)(address        & 0x000000FF));
    CE_DESELECT();                              //Disable device
}

/**
 *@brief This function reads status register and checks BUSY bit for a write in progress operation.It waits until the device is no longer busy.
 *@return status Returns 1 if flash is in busy mode , 0 otherwise.
 */
unsigned char S25FL_IsWriteBusy(void)
{
    unsigned char temp = 0;
    CE_SELECT();			//Enable device
    SPI_Write(CMD_RDSR1);   //Send RDSR command
    temp = SPI_Read();
    CE_DESELECT();			//Disable
    if (temp & 0x01) return 1;
    else return 0;
}

/**
 *@brief Read specific Status Register depending on value of statReg. 
 *@param statReg  Address of one of the status registers.
 *@return StatusReg Contents of status register
 */
unsigned char S25FL_SReg_Read(unsigned char statReg)
{
    unsigned char byte = 0;
    CE_SELECT();			// enable device
    SPI_Write(statReg);     // send RDSR command
    byte = SPI_Read();		// receive byte
    CE_DESELECT();			// disable device
    return byte;
}

/**
 *@brief This procedure writes a byte to the Status Register.
 *@param statReg Address of one of the status registers.
 *@return none
 */
void S25FL_SReg_Write(unsigned char statReg)
{
    CE_SELECT();			//Enable device
    SPI_Write(CMD_WREN);	//Send WREN command
    CE_DESELECT();			//Disable device

    CE_SELECT();                        // enable device
    SPI_Write(CMD_WRSR);                // select write to status register
    SPI_Write(statReg);                 // data that will change the status of BPx or BPL (only bits 2,3,4,5,7 can be written)
    CE_DESELECT();                      // disable the device
}

/**
 *@brief Puts the flash chip into deep sleep mode.
 *@return none
 */
void S25FL_DeepSleep(void)
{
    CE_SELECT();                        // enable device
    SPI_Write(CMD_DEEPSLP);             // select write to status register
    CE_DESELECT();                      // disable the device
}

/**
 *@brief  Wakes the flash memory chip from deep sleep mode.
 *@return none
 */
void S25FL_WakeUp(void)
{
    CE_SELECT();                        // enable device
    SPI_Write(CMD_RESDPD);              // select write to status register
    CE_DESELECT();                      // disable the device
}

/**
 *@brief Sets the block protection of the chip.
 *@param prot Protection settings.
 *@return none
 */
void S25FL_SetBlockProtection(unsigned char prot)
{
    CE_SELECT();
    SPI_Write(CMD_EWSR);		//enable write to volatile register
    CE_DESELECT();

    CE_SELECT();
    SPI_Write(CMD_WRSR);		//write status register
    SPI_Write((prot & 0x0f)<<2);
    CE_DESELECT();
}

/// @}

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
 * @file S25FL204K/Example2/main.c
 * @author D.Qendri
 * @date 17 Aug 2014
 * @brief Example of writing and reading pages from NOR flash.
 */
 
#include <stdio.h>
#include <unistd.h>
#include "S25FL204K.h"
#include "SPI.h"
#include "Utilities.h"

unsigned char  pattern1[] = 
{
0x55,0x55,0x55,0x65,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
0x55,0x55,0x55,0x65,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
0x55,0x55,0x55,0x65,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
0x55,0x55,0x55,0x65,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55
};

unsigned char  pattern2[] =
{
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g'
};

unsigned char buffer[256] = {0};
unsigned int len = 256;
unsigned long address = 0x000000;

void readMemory(unsigned long address,unsigned int page_count);

int main(void)
{
	SPI_Initialize();				//Initialize SPI peripheral
	S25FL_Setup();                  //Setup flash memory
	S25FL_SReg_Write(0x00);         //Remove protection from all blocks writes to SR1

	printf("Flash memory example.\r\n");
	printf("This demo read and writes pages from flash memory.\r\n");

	/*****************Read pages *************************/

	printf("Read first page\r\n");
	readMemory(address,1);   
	delay_ms(200);
	printf("\r\n Writing on first page \r\n");
	S25FL_WriteArray(address,pattern1,256);

	printf("\r\nRead new sectors\r\n");
	unsigned long dst2 = 0x000000;
	readMemory(dst2,2);
	
	printf("\r\n Erasing data \r\n");
	S25FL_SectorErase(0x000000);
	printf("\r\n Done erase \r\n");
	delay_ms(500);
  
	dst2 = 0x000000;
	printf("\r\n Veryfing erased data \r\n");
	readMemory(dst2,2); 
	
	delay_ms(500);
	SPI_Close();

return 0;
}


void readMemory(unsigned long address,unsigned int page_count)
{
    unsigned int i = 0;
    unsigned char page = 0;

    for(page =0 ; page < page_count;page++)
    {
        S25FL_ReadArray(address,buffer,len);
        usleep(35000);  
        printf("\r\n\r\nPage %d.", page);
        usleep(35000);  
        
        for(i=0;i<256;i++)
        {            
            if (i%64 == 0)
            {
                printf("\r\n");
            }
            printf("0x%X ",buffer[i]);
        }
        address += 0x100;
    }
}

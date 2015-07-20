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
 * @file Display/Example2/main.c
 * @author D.Qendri
 * @date 30 May 2015
 * @brief Text font examples.   
 */

#include <stdio.h>
#include <unistd.h>
#include "bcm2835.h"
#include "Utilities.h"
#include "SPI.h"
#include "TFT.h"
#include "GFX.h"

extern const unsigned char font[] ;

#define BACKGROUND  WHITE
#define FOREGROUND BLUE
#define DELAY 1200

int main(void)
{

	SPI_Initialize();				//Initialize SPI peripheral
	TFT_Initialize();

	char str1[] = "Ascii text";

    while(1)
    {
		TFT_Background(WHITE);
		TFT_PrintString(5,80,FOREGROUND,BACKGROUND,str1,2);
		delay_ms(DELAY);
		
		TFT_Background(WHITE);
		TFT_PrintString(5,50,FOREGROUND,BACKGROUND,str1,4);
		delay_ms(DELAY);
		
		
		TFT_Background(WHITE);
		TFT_PrintInteger(40, 50, RED, BACKGROUND,12436, 1);
		delay_ms(DELAY);    
    }
	
	SPI_Close();

    return 0;
}

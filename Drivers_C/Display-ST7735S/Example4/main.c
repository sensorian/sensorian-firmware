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
 * @file Display/Example4/main.c
 * @author D.Qendri
 * @date 30 May 2015
 * @brief Image plotting on the TFT screen.   
 */
 
#include <stdio.h>
#include <unistd.h>
#include "bcm2835.h"
#include "Utilities.h"
#include "SPI.h"
#include "TFT.h"
#include "GFX.h"

#include "Barometer2.h"
#include "Altimeter1.h"

#define BACKGROUND  WHITE
#define FOREGROUND BLUE
#define DELAY 2000

int main(void)
{
	SPI_Initialize();				//Initialize SPI peripheral
	TFT_Initialize();
	TFT_Background(WHITE);
	
	Image_t b = {Barometer2,100,100};

    while(1)
    {
		TFT_Background(WHITE);
		TFT_ShowPic(Altimeter1,64,64,15,30);
		delay_ms(DELAY); 
		TFT_Background(WHITE);
		TFT_DisplayImage(&b,10,10);
		delay_ms(DELAY);    
    }
	
	SPI_Close();

    return 0;
}

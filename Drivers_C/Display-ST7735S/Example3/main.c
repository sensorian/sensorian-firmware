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
 * @file Display/Example3/main.c
 * @author D.Qendri
 * @date 30 May 2015
 * @brief Display landscape / portarit modes examples.   
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
#define DELAY 2000

int main(void)
{

	SPI_Initialize();				//Initialize SPI peripheral
	TFT_Initialize();

	char texty[] = "Sensorian-123";

    while(1)
    {
		TFT_Background(WHITE);
		TFT_SetOrientation(PORTRAIT);		//Portrait mode 
		TFT_VerticalLine(5,5,90,BLUE);
		TFT_PrintString(10,80,FOREGROUND,BACKGROUND,texty,1);
		delay_ms(DELAY);   
		
		TFT_Background(WHITE);
		TFT_SetOrientation(PORTRAIT_INV);		//Portrait mode 
		TFT_VerticalLine(5,5,90,GREEN);
		TFT_PrintString(10,80,RED,BACKGROUND,texty,1);
		delay_ms(DELAY);   
		
		TFT_Background(WHITE);
		TFT_SetOrientation(PORTRAIT_REF);			//Portrait mode reflected 
		TFT_VerticalLine(5,5,90,YELLOW);
		TFT_PrintString(10,80,GREEN,BACKGROUND,texty,1);
		delay_ms(DELAY);   
		
		TFT_Background(WHITE);
		TFT_SetOrientation(PORTRAIT_INV_REF);		
		TFT_VerticalLine(5,5,90,RED);			//Portrait mode reflected
		TFT_PrintString(10,80,PURPLE,BACKGROUND,texty,1);
		delay_ms(DELAY);   
		
		TFT_Background(WHITE);
		TFT_SetOrientation(LANDSCAPE);			//Landscape mode 
		TFT_VerticalLine(5,5,80,GRAY1);
		TFT_PrintString(10,80,GREEN,BACKGROUND,texty,1);
		delay_ms(DELAY);   
		
		TFT_Background(WHITE);
		TFT_SetOrientation(LANDSCAPE_REF);				//Landscape mode  reflected
		TFT_VerticalLine(5,5,80,PURPLE);
		TFT_PrintString(10,80,PURPLE,BACKGROUND,texty,1);
		delay_ms(DELAY);   
		
		TFT_Background(WHITE);
		TFT_SetOrientation(LANDSCAPE_INV);			//Landscape mode inverted
		TFT_VerticalLine(5,5,80,NAVY);
		TFT_PrintString(10,80,RED,BACKGROUND,texty,1);
		delay_ms(DELAY);   
		
		TFT_Background(WHITE);
		TFT_SetOrientation(LANDSCAPE_INV_REF);
		TFT_VerticalLine(5,5,80,PURPLE);
		TFT_PrintString(10,80,FOREGROUND,BACKGROUND,texty,1);	//Landscape mode inverted reflected
		delay_ms(DELAY);     
    }
	
	SPI_Close();

    return 0;
}

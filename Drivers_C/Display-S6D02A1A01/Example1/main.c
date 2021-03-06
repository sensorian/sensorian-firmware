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
 * @file Display/Example1/main.c
 * @author D.Qendri
 * @date 30 May 2015
 * @brief Plot graphic primitives on the display.   
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

    while(1)
    {
		/***************Lines ****************************************/
		TFT_Background(WHITE);
		TFT_HorizontalLine(4,10,40,RED);		//Horizontal Line of length 40 pixels
		delay_ms(DELAY);
		
		TFT_Background(WHITE);
		TFT_VerticalLine(20,20,60,MAROON);		//Vertical Line of length 60 pixels
		delay_ms(DELAY);
		
		TFT_Background(WHITE);
		TFT_Line(5,5,100,30,NAVY);		//Line from (5,5) to (100,30)
		delay_ms(DELAY);
		
		TFT_Background(WHITE);
		TFT_SlantyLine(8,8,50,40,BLUE);		//Slanty line of length 40 pixels
		delay_ms(DELAY);
		
		/************************************** Rectangles ************************************/
		
		TFT_Background(WHITE);
		TFT_EmptyRectangle(10,30,50,60,GRAY1);		//
		delay_ms(DELAY);
		
		TFT_Background(WHITE);
		TFT_FullRectangle(10,30,50,60,GRAY1); 	//
		delay_ms(DELAY);
		
		TFT_Background(WHITE);
		TFT_RoundRectangle(10,10,50,60,10,GRAY1);		//
		delay_ms(DELAY);
		
		TFT_Background(WHITE);
		TFT_FullRectangle(80,40,100,120,GRAY0);		//
		delay_ms(DELAY);
		
		/******************************** Circles **************************************************/
		
		TFT_Background(WHITE);
		TFT_Circle(70,60,30,RED);		//
		delay_ms(DELAY);
		
		TFT_Background(WHITE);
		TFT_Disk(50,50,20,PURPLE);		//
		delay_ms(DELAY);
		
		TFT_Background(WHITE);
		TFT_Ellipse(69,40,10,40,BLUE);		//
		delay_ms(DELAY);
		
		/********************************** Triangles **************************************************/
  
		TFT_Background(WHITE);
		TFT_RightTriangle(20,10,40,80,BLACK);		//
		delay_ms(DELAY);	
    
    }
	
	SPI_Close();

    return 0;
}

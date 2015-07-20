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

 
#include <bcm2835.h>
#include <stdio.h>
#include "i2c.h"
#include "SPI.h"
#include "TFT.h"
#include "GFX.h"
#include "MPL3115A2.h"
#include "Barometer.h"
#include "Altimeter.h"
#include "Thermometer.h"
#include "Utilities.h"

extern unsigned int Altimeter[];
extern unsigned int Barometer[];
extern unsigned int Thermometer[];

char buffer[32] = {0};

int main(int argc, char **argv)
{
	SPI_Initialize();				// Initialize SPI peripheral
	TFT_Initialize();				// Configure TFT
	
	I2C_Initialize(MPL3115A2_ADDRESS);			//Initialize I2C peripheral and setup sensor address
	
	MPL3115A2_Initialize();
	MPL3115A2_ActiveMode();	
	TFT_Background(WHITE);
	
	float temp, alt, press;
	
    while(1)
    {    
		//TFT_SetOrientation(LANDSCAPE);
		TFT_ShowPic(Thermometer,64,64,30,30);
		TFT_PlotEmptyRectangle(4,4,122,155,RED);
		temp = MPL3115A2_ReadTemperature();			//Take a temperature reading
		sprintf(buffer,"%0.2f C", temp);
		TFT_PrintString(20,120,BLUE,WHITE,buffer,2);
		delay_ms(4000);
		TFT_Background(WHITE);		
		
		MPL3115A2_StandbyMode();
		MPL3115A2_AltimeterMode();
		TFT_PlotEmptyRectangle(4,4,122,155,BLUE);
		TFT_ShowPic(Altimeter,64,64,30,30);
        alt = MPL3115A2_ReadAltitude();				//Take an altimeter reading
		sprintf(buffer,"%0.2f m", alt/100);
		TFT_PrintString(20,120,BLACK,WHITE,buffer,2);
		delay_ms(4000);		
		TFT_Background(WHITE);		
		
		MPL3115A2_StandbyMode();
		MPL3115A2_BarometerMode();
		TFT_PlotEmptyRectangle(4,4,122,155,NAVY);
		TFT_ShowPic(Barometer,100,100,12,14);
        press = MPL3115A2_ReadBarometricPressure();	//Take a pressure reading
		sprintf(buffer,"%0.2f kPa", press/1000);
		TFT_PrintString(6,120,RED,WHITE,buffer,2);
		delay_ms(4000);  
		TFT_Background(WHITE);			
    }

return 0;
}



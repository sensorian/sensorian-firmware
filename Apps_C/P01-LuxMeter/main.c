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

 
#include <stdio.h>
#include <unistd.h>
#include "bcm2835.h"
#include "SPI.h"
#include "TFT.h"
#include "GFX.h"
#include "APDS9300.h"
#include "i2c.h"
#include "Font.h"
#include "Utilities.h"
#include "LampON.h"

int main(void)
{
	char str[20];
	unsigned int channel0;
	unsigned int channel1;
	float lux;

	SPI_Initialize();					//Initialize SPI peripheral
	TFT_Initialize();
	
	I2C_Initialize(APDS9300ADDR);		//Initialize I2C and setup chip address	
	
	AL_Initialize();					//Setup Ambient light sensor

	Image_t LampOn = {_acLampON,100,100};
	TFT_Background(WHITE);
	
	TFT_DisplayImage(&LampOn,5,5);
	
    while(1)
    { 	 
		channel0 = AL_ReadChannel(CH0);		//Take a reading from channel one
		channel1 = AL_ReadChannel(CH1);		//Take a reading from channel two	
        lux = AL_Lux(channel0,channel1);
		sprintf(str, "%3.2f Lx ", lux);
		TFT_PrintString(10,140,RED,WHITE,str,2);
		delay_ms(1000);
    }

    return 0;
}

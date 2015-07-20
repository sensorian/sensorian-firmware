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
 * @file MPL3115A2/Example2/main.c
 * @author D.Qendri
 * @date 30 April 2015
 * @brief Barometric pressure and altitude reading example.
 */
 
#include <bcm2835.h>
#include <stdio.h>
#include "MPL3115A2.h"
#include "i2c.h"
#include "Utilities.h"

int main(int argc, char **argv)
{

	I2C_Initialize(MPL3115A2_ADDRESS);		//Initialize I2C and setup chip address	
	MPL3115A2_Initialize();
	MPL3115A2_ActiveMode();	
	
	printf("MPL3115A2 Altimeter/Barometer sensor demo.\r\n");
	
	unsigned char id  = MPL3115A2_ID();	//Verify chip id
	printf("Chip ID: 0x%02X . \r\n", id);
	
    while(1)
    {
		MPL3115A2_StandbyMode();
		MPL3115A2_AltimeterMode();
        float alt = MPL3115A2_ReadAltitude();	//Take an altimeter reading
		printf("Altimeter: %0.2f m above sea level.\r\n", alt/100);
		delay_ms(500);
		
		MPL3115A2_StandbyMode();
		MPL3115A2_BarometerMode();
        float press = MPL3115A2_ReadBarometricPressure();	//Take a pressure reading
		printf("Pressure: %0.2f Pascal. \r\n", press);
		delay_ms(500);
        
    }

return 0;
}


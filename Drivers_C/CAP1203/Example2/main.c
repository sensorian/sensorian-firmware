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
 * @file /CAP1203/Example2/main.c
 * @author D.Qendri
 * @date 28 May 2015
 * @brief Demonstrates the multitouch capabilites of the controller.
 *        Pressing buttons B1 and B2 simulateously will generate a multi-touch event.
 */
 
#include <bcm2835.h>
#include "CAP1203.h"
#include "Utilities.h"
#include "i2c.h"

int main(int argc, char **argv)
{
	I2C_Initialize(CAP1203ADDR);
    CAP1203_Initialize();
	
	CAP1203_ConfigureMultiTouch(TWO,CS2_PTRN|CS1_PTRN);
	
	while(1)
	{
			if(CAP1203_MultitouchEvent())
			{
				printf("Multitouch  Event \r\n");
				CAP1203_ClearInterrupt();				
			}
			delay_ms(500);
	}
    I2C_Close();
	
    return 0;
}


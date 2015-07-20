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
 * @file LED/Example1/main.c
 * @author D.Qendri
 * @date 30 May 2015
 * @brief Blink an LED.
 *        Sets the logic level of the LED.
 */
#include <bcm2835.h>
#include <stdio.h>
#include "Utilities.h"

int main(int argc, char **argv)
{
    if (!bcm2835_init())						//Configure I2C pins
	{
		printf("BCM libray error.\n");
	}
	
	printf("LED blinking demo with the Sensorian Shield.\r\n");
	pinModeOutput(LED_PIN);
	
    while (1)
    {
		digitalWrite(LED_PIN, HIGH);		
		
		delay_ms(1000);				

		digitalWrite(LED_PIN, LOW);	

		delay_ms(1000);
    }

    bcm2835_close();
    return 0;
}


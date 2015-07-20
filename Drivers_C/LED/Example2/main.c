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
 * @file LED/Example2/main.c
 * @author D.Qendri
 * @date 30 May 2015
 * @brief Software PWM brightness control for LED.
 *        The PWM duty cycle rised from 0% to 100% and then jumps back to zero.
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
		for(int j=0;j<100;j++)
        {
            for(int k=0;k<128;k++)
            {
                for(int i=0;i<100;i++)
                {
                    if(i<j) 
					{
						digitalWrite(LED_PIN, HIGH);
					}else{
						digitalWrite(LED_PIN, LOW);
					}
                }
            }
        }

        for(int j=0;j<100;j++)
        {
            for(int k=0;k<128;k++)
            {
                for(int i=100;i>0;i--)
                {
                    if(i>j){
						digitalWrite(LED_PIN, HIGH);
					}else {
						digitalWrite(LED_PIN, LOW);
					}

                }
            }
        }
		

    }

    bcm2835_close();
    return 0;
}


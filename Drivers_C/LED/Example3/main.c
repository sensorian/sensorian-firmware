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
 * @file LED/Example3/main.c
 * @author D.Qendri
 * @date 30 May 2015
 * @brief Read Pin status.
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
	unsigned char pval = 0;
	printf("GPIO read pin status.\r\n");
		
    while (1)
    {
		pval = ReadPinStatus(MFP_PIN);
		printf("MFP pin level is %d\r\n",pval);	
		delay_ms(1000);
		pval = ReadPinStatus(MPL_PIN);
		printf("MPL pin level is %d\r\n",pval);	
		delay_ms(1000);
		pval = ReadPinStatus(LUX_PIN);
		printf("LUX pin level is %d\r\n",pval);	
		delay_ms(1000);
		pval = ReadPinStatus(ALERT_PIN);
		printf("ALERT pin level is %d\r\n",pval);	
		delay_ms(1000);
		pval = ReadPinStatus(LED_PIN);
		printf("LED pin level is %d\r\n",pval);	
		delay_ms(1000);
		pval = ReadPinStatus(ACLM_PIN);
		printf("ACLM pin level is %d\r\n",pval);	
		delay_ms(1000);
    }
	
    bcm2835_close();
    return 0;
}

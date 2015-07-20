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

 
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "bcm2835.h"
#include "Utilities.h"
#include "SPI.h"
#include "TFT.h"
#include "GFX.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>

#include <errno.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>


extern const unsigned char font[] ;

#define BACKGROUND  BLACK
#define FOREGROUND WHITE
#define DELAY 2000


int main(void)
{
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;
    char *addr;	
	char intstr[20] = "";
	char ipstr[20];
	
	unsigned char i = 10;
  
	SPI_Initialize();				//Initialize SPI peripheral
	TFT_Initialize();	
	TFT_Background(BLACK);
	

    while(i > 0)
    {
		delay_ms(2000);
		TFT_SetRotation(LANDSCAPE);			//Landscape mode
		TFT_Background(BLACK);
		
		
		getifaddrs (&ifap);
		for (ifa = ifap; ifa; ifa = ifa->ifa_next) 
		{
			if (ifa->ifa_addr->sa_family==AF_INET) 
			{
				sa = (struct sockaddr_in *) ifa->ifa_addr;
				addr = inet_ntoa(sa->sin_addr);
				sprintf(intstr,"%s", ifa->ifa_name);
				sprintf(ipstr,"%s", addr);
			}
		}
		TFT_PrintString(20,40,FOREGROUND,BACKGROUND,intstr,2);
		TFT_PrintString(20,80,FOREGROUND,BACKGROUND,ipstr,1);
		i--;
	}
	
	freeifaddrs(ifap);
	SPI_Close();

    return 0;
}


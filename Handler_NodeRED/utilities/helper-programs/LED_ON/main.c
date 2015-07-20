#include "bcm2835.h"
#include <stdio.h>
#include "Utilities.h"

#define LED_PIN 		RPI_V2_GPIO_P1_12

int main(int argc, char **argv)
{
    if (!bcm2835_init())						//Configure I2C pins
	{
		printf("BCM libray error.\n");
	}

	//printf("LED blinking demo with the Sensorian Shield.\r\n");
	pinModeOutput(LED_PIN);
	digitalWrite(LED_PIN, HIGH);

        bcm2835_close();
        return 0;
}


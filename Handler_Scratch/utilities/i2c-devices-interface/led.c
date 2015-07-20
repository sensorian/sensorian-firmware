#include "led.h"
/**
 * Sets LED pin as output
 */
void LED_init(void)
{
	bcm2835_gpio_fsel(LED_PIN, BCM2835_GPIO_FSEL_OUTP);
}


/**
 * Turns Sensorian LED on.
 */
void LED_on(void)
{
	bcm2835_gpio_write(LED_PIN, 1);
}

/**
 * Turns Sensorian LED off.
 */
void LED_off(void)
{
	bcm2835_gpio_write(LED_PIN, 0);
}

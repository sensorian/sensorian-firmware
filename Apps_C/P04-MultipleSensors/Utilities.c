#include "Utilities.h"

/// \defgroup Utilities
/// These are common helper functions that are used to configure pins and for timing delays.
/// @{

/**
 *@brief Delay in ms.
 *@param ms Delay in milliseconds
 *@return none
 */	
void delay_ms(unsigned int ms)
{
	bcm2835_delay(ms);
}

/**
 *@brief Configures the given pin as output.
 *@param pin PIN_t type 
 *@return none
 */	
void pinModeOutput(PIN_t pin)
{
	bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
}

/**
 *@brief Write a logic level to the pin. The pin should be confgured as output.
 *@param pin Pin to be configured, Availabel pin definitons are defined in the Utitlies.h
 *@para level Logic leve can be HIGH or LOW.
 *@return none
 */	
void digitalWrite(PIN_t pin, unsigned char level)
{
	bcm2835_gpio_write(pin, level);			// set to level	
}

/**
 *@brief Configures the pins as input and returns the pin status. 
 *@param pin Pin to be configured, Availabel pin definitons are defined in the Utitlies.h
 *@return status Pin level can be HIGH or LOW.
 */	
PinLevel_t ReadPinStatus(PIN_t pin)
{
	bcm2835_gpio_fsel(pin,BCM2835_GPIO_FSEL_INPT);		// Set the pin to be an input
	bcm2835_gpio_set_pud(pin, BCM2835_GPIO_PUD_UP);
	PinLevel_t pinLevel = (PinLevel_t)bcm2835_gpio_lev(pin);		//read voltage level on MFP pin	
	return pinLevel;
}

/// @}

// void PreventPagin(void)
// {
	// struct sched_param sp;
	// memset(&sp, 0, sizeof(sp));
	// sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
	// sched_setscheduler(0, SCHED_FIFO, &sp);
	// mlockall(MCL_CURRENT | MCL_FUTURE);
// }


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
 * @file MCP79410_RTCC/Example1/Utilities.c
 * @author D.Qendri
 * @date 30 Dec 2014
 * @brief Utilities driver.
 */
 
 
#include "Utilities.h"

/// \defgroup utilities Utilities
/// These are common helper functions that are used to read and write GPIO pins and for timing delays.
/// @{

/**
 * @brief Delay in ms.
 * @param ms Delay in milliseconds
 * @return none
 */	
void delay_ms(unsigned int ms)
{
	bcm2835_delay(ms);
}

/**
 * @brief Configures the given pin as output.
 * @param pin PIN_t type 
 * @return none
 */	
void pinModeOutput(PIN_t pin)
{
	bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
}

/**
 * @brief Write a logic level to the pin. The pin should be confgured as output.
 * @param pin Pin to be configured, Available pin definitons are defined in the Utitlies.h
 * @param level Logic level can be HIGH or LOW.
 * @return none
 */	
void digitalWrite(PIN_t pin, unsigned char level)
{
	bcm2835_gpio_write(pin, level);			// set to level	
}

/**
 * @brief Configures the pins as input and returns the pin status. 
 * @param pin Pin to be configured, Available pin definitons are defined in the Utitlies.h
 * @return status Pin level can be HIGH or LOW.
 */	
PinLevel_t ReadPinStatus(PIN_t pin)
{
	bcm2835_gpio_fsel(pin,BCM2835_GPIO_FSEL_INPT);		// Set the pin to be an input
	bcm2835_gpio_set_pud(pin, BCM2835_GPIO_PUD_DOWN);		// Enable pull up.
	PinLevel_t pinLevel = (PinLevel_t)bcm2835_gpio_lev(pin);		//read voltage level on MFP pin	
	return pinLevel;
}

/**
 * @brief Enables detection of rising edge.
 * @param pin Pin to be configured, Available pin definitons are defined in the Utitlies.h
 * @return none
 */
void EnableDetectRisingEdge(PIN_t pin)
{
	bcm2835_gpio_ren(pin);		//enable rising edge detect on pin
}

/**
 * @brief Disables detection of rising edge.
 * @param pin Pin to be configured, Available pin definitons are defined in the Utitlies.h
 * @return none
 */
void DisableDetectRisingEdge(PIN_t pin)
{
	bcm2835_gpio_clr_ren(pin);		//Disable rising edge detect on pin
}

/**
 * @brief Enables detection of falling edge.
 * @param pin Pin to be configured, Available pin definitons are defined in the Utitlies.h
 * @return none
 */
void EnableDetectFallingEdge(PIN_t pin)
{
	bcm2835_gpio_ren(pin);		//enable rising edge detect on pin
}

/**
 * @brief Dsiables detection of falling edge.
 * @param pin Pin to be configured, Available pin definitons are defined in the Utitlies.h
 * @return none
 */
void DisableDetectFallinggEdge(PIN_t pin)
{
	bcm2835_gpio_clr_ren(pin);		//Disable rising edge detect on pin
}

/**
 * @brief Enables detection of high level on pin.
 * @param pin Pin to be configured, Available pin definitons are defined in the Utitlies.h
 * @return none
 */
void EnableDetectHighLevel(PIN_t pin)
{
	bcm2835_gpio_ren(pin);		//enable rising edge detect on pin
}

/**
 * @brief Disables detection of high level.
 * @param pin Pin to be configured, Available pin definitons are defined in the Utitlies.h
 * @return none
 */
void DisableDetectHighLevel(PIN_t pin)
{
	bcm2835_gpio_clr_ren(pin);		//Disable rising edge detect on pin
}

/**
 * @brief Enables detection of low level.
 * @param pin Pin to be configured, Available pin definitons are defined in the Utitlies.h
 * @return none
 */
void EnableDetectLowLevel(PIN_t pin)
{
	bcm2835_gpio_ren(pin);		//enable rising edge detect on pin
}

/**
 * @brief Disables detection of low level.
 * @param pin Pin to be configured, Available pin definitons are defined in the Utitlies.h
 * @return none
 */
void DisableDetectLowLevel(PIN_t pin)
{
	bcm2835_gpio_clr_ren(pin);		//Disable rising edge detect on pin
}

/**
 * @brief Enables detection of rising  edge interrupt.
 * @param pin Pin to be configured, Available pin definitons are defined in the Utitlies.h
 * @return none
 */
void EnableDetectRisingInterrupt(PIN_t pin)
{
	bcm2835_gpio_aren(pin);		//enable rising edge detect on pin
}

/**
 * @brief Disables detection of rising  edge interrupt.
 * @param pin Pin to be configured, Available pin definitons are defined in the Utitlies.h
 * @return none
 */
void DisableDetectRisingInterrupt(PIN_t pin)
{
	bcm2835_gpio_clr_aren(pin);		//Disable rising edge detect on pin
}

/**
 * @brief Checks fro even on given pin. 
 * @param pin Pin to be configured, Available pin definitons are defined in the Utitlies.h
 * @return none
 */
unsigned char CheckPinforEvent(PIN_t pin)
{
	unsigned char event = bcm2835_gpio_eds(pin);		//Check for event detection
	bcm2835_gpio_set_eds(pin);		//Clear even detection interrupt flags
	return event;
}

/// @}

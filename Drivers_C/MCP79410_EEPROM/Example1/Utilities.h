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
 * @file MCP79410_EEPROM/Example1/Utilities.h
 * @author D.Qendri
 * @date 30 May 2015
 * @brief Utilities driver header.
 *
 */
 
#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>

#define ALPHA_BOARD

typedef uint8_t PIN_t;

typedef enum PinLev {LOWLEVEL = 0, HIGHLEVEL} PinLevel_t;


#define MPL_PIN 		RPI_V2_GPIO_P1_15
#define LUX_PIN 		RPI_V2_GPIO_P1_11
#define ACLM_PIN 		RPI_V2_GPIO_P1_07
#define ALERT_PIN 		RPI_V2_GPIO_P1_13
#define LED_PIN 		RPI_V2_GPIO_P1_12
#define MFP_PIN 		RPI_V2_GPIO_P1_18

void delay_ms(unsigned int ms);

PinLevel_t 	ReadPinStatus(PIN_t pin);
void 		pinModeOutput(PIN_t pin);
void 		digitalWrite(PIN_t pin, unsigned char level);

void 		EnableDetectRisingEdge(PIN_t pin);
void 		DisableDetectRisingEdge(PIN_t pin);
void 		EnableDetectFallingEdge(PIN_t pin);
void 		DisableDetectFallinggEdge(PIN_t pin);
void 		EnableDetectHighLevel(PIN_t pin);
void 		DisableDetectHighLevel(PIN_t pin);
void 		EnableDetectLowLevel(PIN_t pin);
void 		DisableDetectLowLevel(PIN_t pin);
void 		EnableDetectRisingInterrupt(PIN_t pin);
void 		DisableDetectRisingInterrupt(PIN_t pin);

#endif
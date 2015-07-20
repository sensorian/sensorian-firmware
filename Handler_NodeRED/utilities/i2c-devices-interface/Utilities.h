#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>

//#define ALPHA_BOARD

typedef uint8_t PIN_t;

typedef enum PinLev {LOWLEVEL = 0, HIGHLEVEL} PinLevel_t;

#ifdef ALPHA_BOARD

#define MPL_PIN 		RPI_V2_GPIO_P1_07
#define LUX_PIN 		RPI_V2_GPIO_P1_11
#define ACLM_PIN 		RPI_V2_GPIO_P1_12
#define ALERT_PIN 		RPI_V2_GPIO_P1_13
//#define LED_PIN 		RPI_V2_GPIO_P1_15
#define MFP_PIN 		RPI_V2_GPIO_P1_18

#else

#define MPL_PIN 		RPI_V2_GPIO_P1_15
#define LUX_PIN 		RPI_V2_GPIO_P1_11
#define ACLM_PIN 		RPI_V2_GPIO_P1_07
#define ALERT_PIN 		RPI_V2_GPIO_P1_13
//#define LED_PIN 		RPI_V2_GPIO_P1_12
#define MFP_PIN 		RPI_V2_GPIO_P1_18

#endif

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

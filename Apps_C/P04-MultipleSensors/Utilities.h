#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>

#define TRUE	1
#define FALSE	0

typedef uint8_t PIN_t;

typedef enum PinLev {LOWLEVEL = 0, HIGHLEVEL} PinLevel_t;


#define MPL_PIN 		RPI_V2_GPIO_P1_07
#define LUX_PIN 		RPI_V2_GPIO_P1_11
#define ACLM_PIN 		RPI_V2_GPIO_P1_12
#define ALERT_PIN 		RPI_V2_GPIO_P1_13
#define LED_PIN 		RPI_V2_GPIO_P1_15
#define MFP_PIN 		RPI_V2_GPIO_P1_18


PinLevel_t ReadPinStatus(PIN_t pin);

void pinModeOutput(PIN_t pin);
void digitalWrite(PIN_t pin, unsigned char level);
void delay_ms(unsigned int ms);
void PreventPagin(void);




#endif
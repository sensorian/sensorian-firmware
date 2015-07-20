#include <bcm2835.h>
#include "board_version.h"

#if BOARD_VERSION==1
	#define LED_PIN			(RPI_V2_GPIO_P1_15)
#elif BOARD_VERSION==2
	#define LED_PIN			(RPI_V2_GPIO_P1_12)
#endif

void LED_init(void);
void LED_on(void);
void LED_off(void);

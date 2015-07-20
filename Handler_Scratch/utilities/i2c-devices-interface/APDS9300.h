#ifndef __APDS9300_H__
#define __APDS9300_H__

#include "bcm2835.h"
#include <stdlib.h>

/*! \def APDS9300ADDR
    \brief The I2C address of the APDS9300 ambient light sensor
*/
#define APDS9300ADDR        0x29

/******************Register definitions************************/
#define CONTROL             0x00
#define TIMING              0x01
#define THRESHLOWLOW        0x02
#define THRESHLOWHIGH       0x03
#define THRESHHIGHLOW       0x04
#define THRESHHIGHHIGH      0x05
#define INTERRUPT           0x06
#define CRC                 0x08
#define ID                  0x0A
#define DATA0LOW            0x0C
#define DATA0HIGH           0x0D
#define DATA1LOW            0x0E
#define DATA1HIGH           0x0F

/**************************************************************/

/***********************COMMAND REG*****************************/
#define COMMAND             0x80
#define CMD_CLEAR_INT       0x40
#define CMD_WORD            0x20


/**********************CONTROL REG******************************/
#define POWERON             0x03
#define POWEROFF            0x00

/*********************Timing Reg********************************/
#define GAIN                0x10
#define MANUAL              0x08

#define INTEG13_7MS         0x00
#define	INTEG101MS          0x01
#define INTEG402MS          0x02
#define MANUAL_INTEG		0x03

/*********************INTERRUPT REG*****************************/

#define INTERR_DISMASK      0xCF	//INTR bits zero
#define	INTERR_ENA          0x10	//Level interrupts

#define	EVERYCYCLE			0x00
#define OUTSIDE_THRESH		0x01
#define	PER2_OUT_RANGE		0x02
#define	PER4_OUT_RANGE		0x04
#define	PER10_OUT_RANGE		0x0A
#define	PER15_OUT_RANGE		0x0F

/***************************************************************/

/*! Channel types */
typedef enum{CH0, /*!< Channel one. */
			 CH1  /*!< Channel two. */} channel;

/*! Gain types */
typedef enum{GAIN_1=0, /*!< Gain 1. */
			 GAIN_16=1 /*!< Gain 16. */} gain;
			 
/*! Power state types */ 
typedef enum{POWER_ON=0x03, /*!< Power on. */
			POWER_OFF=0x00 /*!< Power off. */} powerState;
			
typedef enum{
            S1 = INTEG13_7MS,
			S2 = INTEG101MS,
			S3 = INTEG402MS
			}sampTime_t;


unsigned char 	AL_Initialize(void);
void 			AL_PowerState(powerState state);
unsigned char 	AL_ChipID(void);

unsigned int 	AL_ReadChannel(channel chan);
float 			AL_Lux(unsigned int ch0, unsigned int ch1);

unsigned char   AL_SetGain(gain val);
void 			AL_SetSamplingTime(sampTime_t sampling_time);
void 			AL_SetIntLowThreshold(unsigned int lowthreshvalue);
void            AL_SetIntHighThreshold(unsigned int highthreshvalue);
void		 	AL_Clear_Interrupt(void);
void 			AL_ConfigureInterrupt(unsigned char enable, unsigned char persistence);

void 			APDS9300_WriteRegister(unsigned char reg,unsigned char data);
void            APDS9300_WriteByte(unsigned char data);
void            APDS9300_WriteWord(unsigned char reg, unsigned int data);
char 			APDS9300_ReadByte(char reg);
unsigned int 	APDS9300_ReadWordReg(char reg);
unsigned int 	APDS9300_ReadWord(void);

#endif

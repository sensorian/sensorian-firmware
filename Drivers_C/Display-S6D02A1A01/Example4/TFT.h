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
 * @file Display/Example1/TFT.h
 * @author D.Qendri
 * @date 30 May 2015
 * @brief TFT library header.   
 */
 
#ifndef __TFT_H__
#define __TFT_H__

/**
 * @file   /Display/Example3/TFT.h
 * 
 * D.Qendri 
 * @date   June, 2014
 * @brief  TFT Driver.
 *
 * Driver for S6D02A1A01 controller.
 */
 
#include "bcm2835.h"

#define NOP                             0x00
#define SW_RES							0x01
#define RDDIDIF							0x04		//4 arguments
#define RD_STATUS						0x09
#define RDDPM							0x0A
#define READ_MADCTL						0x0B
#define PIXEL_FORMAT                    0x0C
#define IMAGE_FORMAT                    0x0D
#define SIGNAL_MODE                     0x0E
#define SELF_DIAG                       0x0F
#define SLEEP_IN                        0x10
#define SLEEP_OUT                       0x11
#define PARTIAL_MODE_ON                 0x12
#define NORMAL_DISPLAY_ON               0x13
#define INVERSION_OFF                   0x20
#define INVERSION_ON                    0x21
#define GAMMA_SET						0x26
#define DISPLAY_OFF						0x28
#define DISPLAY_ON 						0x29
#define COL_ADDR_SET                    0x2A
#define PAGE_ADDR_SET                   0x2B
#define MEMORY_WR                       0x2C
#define COLOR_SET                       0x2D
#define MEMORY_RD                       0x2E
#define PARTIAL_AREA                    0x30
#define VERTICAL_SCROLL                 0x33
#define TEAR_EFFECT_OFF                 0x34
#define TEAR_EFFECT_ON                  0x35
#define MADCTL                          0x36
#define VERTSCROLLADDRES                0x37
#define IDLE_MODE_OFF                   0x38
#define IDLE_MODE_ON                    0x39
#define PIXEL_INTERFACE_FORMAT          0x0C
#define ID1                             0xDA
#define ID2                             0xDB
#define ID3                             0xDC

#define MTPCTL                          0x0d
#define MTPREG                          0xD3
#define EXCMD1                          0xf0
#define EXCMD2                          0xF1
#define DISCTL                          0xF2
#define MANPWRSEQ                       0xF3
#define PWRCTL                          0xF4
#define VCMCTL                          0xF5
#define SRCCTL                          0xF6
#define IFCTL                           0xF7
#define PANELCTL                        0xF8
#define PGAMMACTL                       0xFA
#define NGAMMACTL						0xfb
#define EXCOMMAND3						0xfc
#define ANALOG_TEST						0xfd
#define DUMMY							0xff

/******************************************************************************/

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_MH  0x04

/*
 * B5 is increment direction when 0 page incement is horizontal when 1 vertical
 */
#define MADCTL_M1   0x00
#define MADCTL_M2   0b10000000
#define MADCTL_M3   0b01000000
#define MADCTL_M4   0b11000000
#define MADCTL_M5   0b00100000
#define MADCTL_M6   0b10100000
#define MADCTL_M7   0b01100000
#define MADCTL_M8   0b11100000

#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08

/******************************************************************************/
#define WIDTH   0x7F        /** Display width 127 pixel. */
#define HEIGHT  0x9F        /** Display height 159 pixel. */

/******************************Colors******************************************/
#define RED  	0xf800		/** Red color. */
#define GREEN	0x07e0		/** Green color. */
#define BLUE 	0x001f		/** Blue color. */
#define WHITE	0xffff		/** White color. */
#define BLACK	0x0000		/** Black color. */
#define YELLOW  0xFFE0		/** Yellow color. */
#define GRAY0   0xEF7D   	/** Gray color. */
#define GRAY1   0x8410      /** Gray color. */
#define GRAY2   0x4208		/** Gray color. */
#define BLACK   0x0000		/** Black color. */
#define MAROON  0x8000		/** Maroon color. */
#define OLIVE   0x8400		/** Olive color. */
#define NAVY    0x0010		/** Navy color. */
#define PURPLE  0x8010		/** Purple color. */
/******************************************************************************/

#define CS_OUTPUT()     bcm2835_gpio_fsel(RPI_V2_GPIO_P1_24,BCM2835_GPIO_FSEL_OUTP);	//Set GPIO24 as output
#define CS_LOW()        bcm2835_gpio_write(RPI_V2_GPIO_P1_24, LOW);
#define CS_HIGH()       bcm2835_gpio_write(RPI_V2_GPIO_P1_24, HIGH);

#define DC_OUTPUT()     bcm2835_gpio_fsel(RPI_V2_GPIO_P1_22,BCM2835_GPIO_FSEL_OUTP);	//Set 
#define DC_LOW()        bcm2835_gpio_write(RPI_V2_GPIO_P1_22, LOW);
#define DC_HIGH()       bcm2835_gpio_write(RPI_V2_GPIO_P1_22, HIGH);

#define RST_OUTPUT()    bcm2835_gpio_fsel(RPI_V2_GPIO_P1_16,BCM2835_GPIO_FSEL_OUTP);	//Set GPIO23 as output
#define RST_LOW()       bcm2835_gpio_write(RPI_V2_GPIO_P1_16, LOW);
#define RST_HIGH()      bcm2835_gpio_write(RPI_V2_GPIO_P1_16, HIGH);


/** 
 * @brief Orientation type. Specific orientation modes of the device.
 */
typedef enum {  PORTRAIT=0,					/**< Portrait mode  */
				PORTRAIT_REF=2,				/**< Portrait mode reflected */	
				PORTRAIT_INV_REF=3,			/**< Portarit mode inverted and reflected */	
				PORTRAIT_INV=5,				/**< Portarit inverted */	
				LANDSCAPE=6,				/**< Landscape mode */				
				LANDSCAPE_INV=4,			/**< Landscape mode inverted */				
				LANDSCAPE_REF=1,			/**< Landscape mode reflected */				
				LANDSCAPE_INV_REF=7
} orientation_t;	/**< Landscape mode reflected and inverted */

/** 
 * @brief Image type. Contains image array with height and width dimensions.
 */
typedef struct _Image
{
	unsigned int *picture;			/**< Pointer to array of int  */
	unsigned char width;			/**< Width of image. */
	unsigned char height;			/**< Height of image. */
}Image_t;

void    		TFT_Initialize(void);
void  			TFT_WriteCommand(unsigned char command);
void  			TFT_WriteData(unsigned char datab);
void    		TFT_WriteDataWord(int wdata);
void  			TFT_RamAdress(void);

void 			TFT_SetPixel(unsigned int x_start,unsigned int y_start,unsigned int color);
void			TFT_Sleep(void);
void 			TFT_WakeUp(void);
void 			TFT_TurnOffDisplay(void);
void 			TFT_TurnOnDisplay(void);
void 			TFT_SetRotation(orientation_t mode);

void 			TFT_InvertDisplay(void);
void 			TFT_InvertDisplayOff(void);
void  			TFT_Background(int color);

void   			TFT_ShowPic(unsigned int picture[],unsigned char width, unsigned char height, unsigned int x, unsigned int y);
void    		TFT_DisplayImage(Image_t *image, unsigned char x, unsigned char y);
unsigned int 	TFT_Color565(unsigned char r, unsigned char g, unsigned char b);

void 			TFT_ASCII(char x, char y, int color, int background, char letter, char size);
void 			TFT_PrintString(char x, char y, int color, int background, char * message, char size);
void 			TFT_PrintInteger(char x, char y, int color, int background,int integer, char size);
#endif

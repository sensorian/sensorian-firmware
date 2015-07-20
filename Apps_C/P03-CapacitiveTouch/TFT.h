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
#define SWRESET	     					0x01
#define RDDID							0x04		//4 arguments
#define RDDST							0x09
#define RDDPM							0x0A
#define RDDMADCTL						0x0B		// Read Display MADCTL
#define RDDCOLMOD                       0x0C		// Read Display Pixel Format
#define RDDIM                           0x0D		// Read Display Image Mode
#define RDDSM                           0x0E		// Read Display Signal Mode
#define RDDSDR                          0x0F		// Read Display Self-Diagnostic Result
#define SLEEP_IN                        0x10
#define SLEEP_OUT                       0x11
#define PTLON                           0x12		// Partial Display Mode On
#define NORON                           0x13		// Normal Display Mode On
#define INVOFF                          0x20
#define INVON                           0x21		// Display Inversion On
#define GAMSET    						0x26		// Gamma Set
#define DISPOFF   						0x28
#define DISPON    						0x29
#define CASET                           0x2A
#define RASET                           0x2B
#define RAMWR                           0x2C
#define RGBSET                          0x2D		//Color set 
#define RAMRD                           0x2E
#define PTLAR                           0x30
#define SCRLAR                          0x33		// Scroll Area Set 
#define TEOFF                           0x34
#define TEON                            0x35
#define MADCTL                          0x36
#define VSCSAD                          0x37		// Vertical Scroll Start Address of RAM
#define IDLE_MODE_OFF                   0x38
#define IDLE_MODE_ON                    0x39
#define COLMOD                          0x3A		// Interface Pixel Format
#define RDID1                           0xDA
#define RDID2                           0xDB
#define RDID3                           0xDC		// Read ID3 Value

#define FRMCTR1                         0xB1		// Frame Rate Control (In normal mode/ Full colors) 
#define FRMCTR2                         0xB2		// Frame Rate Control (In Idle mode/ 8-colors)
#define FRMCTR3                         0xB3		// Frame Rate Control (In Partial mode/ full colors)
#define INVCTR                          0xB4		// Display Inversion Control
#define DISSET5                         0xB6 
#define PWCTR1                          0xC0
#define PWCTR2                          0xC1
#define PWCTR3                          0xC2
#define PWCTR4                          0xC3
#define PWCTR5                          0xC4
#define VMCTR1                          0xC5
#define VMOFCTR                         0xC7
#define WRID2                           0xD1
#define WRID3                           0xD2
#define NVFCTR1                         0xD9		// NVM Control Status
#define NVFCTR2                         0xDE
#define NVFCTR3                         0xDF
#define GMCTRP1                         0xE0		// Gamma (‘+’polarity) Correction Characteristics Setting .
#define GMCTRN1                         0xE1		// Gamma ‘-’polarity Correction Characteristics Setting
#define GCV                             0xFC		// Gate Pump Clock Frequency Variable

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

void 			TFT_SetPixel(unsigned char x_start,unsigned char y_start,unsigned int color);
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

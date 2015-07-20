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
 * @file Display/Example3/TFT.c
 * @author D.Qendri
 * @date 30 May 2015
 * @brief TFT library.   
 */
 
#include <string.h>
#include <unistd.h>
#include "TFT.h"
#include "SPI.h"
#include "Font.h"

/// \defgroup tft TFT Display 
/// TFT driver for S6D02A1A01 controller 1.8 inch display.
/// @{

extern const unsigned char FontASCII8X16[];

/**
 *@brief This function intializes the display controller and prepares the it for any subsequent operations.
 *@return none
 */
void TFT_Initialize(void)
{
    CS_OUTPUT();
    DC_OUTPUT();
    RST_OUTPUT();
    bcm2835_delay(100);
    
    CS_LOW();
    bcm2835_delay(10);
    RST_LOW();
    bcm2835_delay(100);
    RST_HIGH();
    bcm2835_delay(100);

    TFT_WriteCommand(0xf0);
    TFT_WriteData(0x5a);
    TFT_WriteData(0x5a);

    TFT_WriteCommand(0xfc);
    TFT_WriteData(0x5a);
    TFT_WriteData(0x5a);

    TFT_WriteCommand(0x26);
    TFT_WriteData(0x01);

    TFT_WriteCommand(0xfa);
    TFT_WriteData(0x02);
    TFT_WriteData(0x1f);
    TFT_WriteData(0x00);
    TFT_WriteData(0x10);
    TFT_WriteData(0x22);
    TFT_WriteData(0x30);
    TFT_WriteData(0x38);
    TFT_WriteData(0x3A);
    TFT_WriteData(0x3A);
    TFT_WriteData(0x3A);
    TFT_WriteData(0x3A);
    TFT_WriteData(0x3A);
    TFT_WriteData(0x3d);
    TFT_WriteData(0x02);
    TFT_WriteData(0x01);

    TFT_WriteCommand(0xfb);
    TFT_WriteData(0x21);
    TFT_WriteData(0x00);
    TFT_WriteData(0x02);
    TFT_WriteData(0x04);
    TFT_WriteData(0x07);
    TFT_WriteData(0x0a);
    TFT_WriteData(0x0b);
    TFT_WriteData(0x0c);
    TFT_WriteData(0x0c);
    TFT_WriteData(0x16);
    TFT_WriteData(0x1e);
    TFT_WriteData(0x30);
    TFT_WriteData(0x3f);
    TFT_WriteData(0x01);
    TFT_WriteData(0x02);

    //////////////power setting sequence//////////
    TFT_WriteCommand(0xfd);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x17);
    TFT_WriteData(0x10);
    TFT_WriteData(0x00);
    TFT_WriteData(0x01);
    TFT_WriteData(0x01);
    TFT_WriteData(0x00);
    TFT_WriteData(0x1f);
    TFT_WriteData(0x1f);

    TFT_WriteCommand(0xf4);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x3f);
    TFT_WriteData(0x3f);
    TFT_WriteData(0x07);
    TFT_WriteData(0x00);
    TFT_WriteData(0x3C);
    TFT_WriteData(0x36);
    TFT_WriteData(0x00);
    TFT_WriteData(0x3C);
    TFT_WriteData(0x36);
    TFT_WriteData(0x00);
    //delay_ms_ms(80);			   //ÐÂÔö

    TFT_WriteCommand(0xf5);
    TFT_WriteData(0x00);
    TFT_WriteData(0x70);//39
    TFT_WriteData(0x66);//3a
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x6d);//38
    TFT_WriteData(0x66);//38
    TFT_WriteData(0x06);

    TFT_WriteCommand(0xf6);
    TFT_WriteData(0x02);
    TFT_WriteData(0x00);
    TFT_WriteData(0x3f);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x02);
    TFT_WriteData(0x00);
    TFT_WriteData(0x06);
    TFT_WriteData(0x01);
    TFT_WriteData(0x00);

    TFT_WriteCommand(0xf2);
    TFT_WriteData(0x00);
    TFT_WriteData(0x01);//04
    TFT_WriteData(0x03);
    TFT_WriteData(0x08);
    TFT_WriteData(0x08);
    TFT_WriteData(0x04);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x01);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x04);
    TFT_WriteData(0x08);
    TFT_WriteData(0x08);

    TFT_WriteCommand(0xf8);
    TFT_WriteData(0x11);//66

    TFT_WriteCommand(0xf7);
    TFT_WriteData(0xc8);
    TFT_WriteData(0x20);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);

    TFT_WriteCommand(0xf3);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);

    TFT_WriteCommand(0x11);
    bcm2835_delay(50);

    TFT_WriteCommand(0xf3);
    TFT_WriteData(0x00);
    TFT_WriteData(0x01);
    bcm2835_delay(50);
    TFT_WriteCommand(0xf3);
    TFT_WriteData(0x00);
    TFT_WriteData(0x03);
    bcm2835_delay(50);
    TFT_WriteCommand(0xf3);
    TFT_WriteData(0x00);
    TFT_WriteData(0x07);
    bcm2835_delay(50);
    TFT_WriteCommand(0xf3);
    TFT_WriteData(0x00);
    TFT_WriteData(0x0f);
    bcm2835_delay(50);

    TFT_WriteCommand(0xf4);
    TFT_WriteData(0x00);
    TFT_WriteData(0x04);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x3f);
    TFT_WriteData(0x3f);
    TFT_WriteData(0x07);
    TFT_WriteData(0x00);
    TFT_WriteData(0x3C);
    TFT_WriteData(0x36);
    TFT_WriteData(0x00);
    TFT_WriteData(0x3C);
    TFT_WriteData(0x36);
    TFT_WriteData(0x00);
    bcm2835_delay(50);

    TFT_WriteCommand(0xf3);
    TFT_WriteData(0x00);
    TFT_WriteData(0x1f);
    bcm2835_delay(50);
    TFT_WriteCommand(0xf3);
    TFT_WriteData(0x00);
    TFT_WriteData(0x7f);
    bcm2835_delay(50);

    TFT_WriteCommand(0xf3);
    TFT_WriteData(0x00);
    TFT_WriteData(0xff);
    bcm2835_delay(50);

    TFT_WriteCommand(0xfd);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x17);
    TFT_WriteData(0x10);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x01);
    TFT_WriteData(0x00);
    TFT_WriteData(0x16);
    TFT_WriteData(0x16);

    TFT_WriteCommand(0xf4);
    TFT_WriteData(0x00);
    TFT_WriteData(0x09);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x3f);
    TFT_WriteData(0x3f);
    TFT_WriteData(0x07);
    TFT_WriteData(0x00);
    TFT_WriteData(0x3C);
    TFT_WriteData(0x36);
    TFT_WriteData(0x00);
    TFT_WriteData(0x3C);
    TFT_WriteData(0x36);
    TFT_WriteData(0x00);

    TFT_WriteCommand(0x36);		  /////////////initializing sequence/////////////
    TFT_WriteData(0x08);

    TFT_WriteCommand(0x35);
    TFT_WriteData(0x00);
    TFT_WriteCommand(0x3a);
    TFT_WriteData(0x05);

    bcm2835_delay(50);			    /////////////////gamma setting sequence/////////
    TFT_WriteCommand(0x29);
    TFT_WriteCommand(0x2c);
}

/**
 * @brief This function write a command to the display controller
 * @param command byte commnand
 * @return none
 */
void  TFT_WriteCommand(unsigned char command)
{
    CS_LOW();
    DC_LOW();
    SPI_Write(command);
    CS_HIGH();
}

/**
 * @brief This function is used to write data to the TFT controller
 * @param datab Byte data
 * @return none
 */
void  TFT_WriteData(unsigned char datab)
{
    CS_LOW();
    DC_HIGH();
    SPI_Write(datab);
    CS_HIGH();
}

/**
 * @brief This function writes a 16 bit word on the display controller registers.
 * @param wdata 16 bit word data.
 * @return none
 */
void TFT_WriteDataWord(int wdata)
{
    CS_LOW();
    DC_HIGH();
    SPI_Write(wdata>>8);
    SPI_Write(wdata);
    CS_HIGH();
}

/**
 * Set RAM address for controller.
 * @return none
 */
void  TFT_RamAdress(void)
{
   TFT_WriteCommand(COL_ADDR_SET);
   TFT_WriteData(0x00);
   TFT_WriteData(0x00);
   TFT_WriteData(0x00);
   TFT_WriteData(WIDTH);
  
   TFT_WriteCommand(PAGE_ADDR_SET);
   TFT_WriteData(0x00);
   TFT_WriteData(0x00);
   TFT_WriteData(0x00);
   TFT_WriteData(HEIGHT);
   TFT_WriteCommand(MEMORY_WR);
}

/**
 * @brief This functions sets a specific pixel on the TFT display
 * @param x_start Starting X coordinate
 * @param y_start Starting y coordinate
 * @param color Color of the pixel
 * @return none
 */
void TFT_SetPixel(unsigned int x_start,unsigned int y_start,unsigned int color)
{
    TFT_WriteCommand(0x2a);
    TFT_WriteData(0x00);
    TFT_WriteData(x_start);
    TFT_WriteData(0x00);
    TFT_WriteData(0x7f);

    TFT_WriteCommand(0x2b);
    TFT_WriteData(0x00);
    TFT_WriteData(y_start);
    TFT_WriteData(0x00);
    TFT_WriteData(0x9f);

    TFT_WriteCommand(0x2c);
    TFT_WriteDataWord(color);
}

/**
 * @brief Puts the display in a low power mode.
 * @return none
 */
void TFT_Sleep(void)
{
    TFT_WriteCommand(SLEEP_IN);
    bcm2835_delay(5);
}

/**
 * @brief  Wakes the display from sleep mode.
 * @return none
 */
void TFT_WakeUp(void)
{
    TFT_WriteCommand(SLEEP_OUT);
    bcm2835_delay(120);
}

/**
 * @brief Blanks out the display.
 * @return none
 */
void TFT_TurnOffDisplay(void)
{
    TFT_WriteCommand(DISPLAY_OFF);
}

/**
 * @brief This function turns on the display from idle mode.
 * @return none
 */
void TFT_TurnOnDisplay(void)
{
    TFT_WriteCommand(DISPLAY_ON);
}

/**
 * @brief Sets the display text orientation. Mirrored modes are 
 *        also supported on top of portrait and landscape modes.
 * @param mode  orientation data type  
 * @return none
 */
void TFT_SetRotation(orientation_t mode)
{
  TFT_WriteCommand(MADCTL);
  switch (mode)
  {
   case 0x00:
     TFT_WriteData(MADCTL_MY | MADCTL_MX| MADCTL_BGR);
     break;
   case 0x01:
     TFT_WriteData(MADCTL_MV |MADCTL_ML| MADCTL_BGR);
     break;
  case 0x02:
     TFT_WriteData(MADCTL_MY | MADCTL_BGR);
    break;
   case 0x03:
     TFT_WriteData(MADCTL_MX | MADCTL_BGR);
     break;
   case 0x04:
     TFT_WriteData(MADCTL_MV | MADCTL_BGR | MADCTL_MX);
     break;
   case 0x05:
     TFT_WriteData(MADCTL_ML | MADCTL_BGR);
     break;
   case 0x06:
     TFT_WriteData(MADCTL_MV|MADCTL_MY | MADCTL_BGR);
     break;
   case 0x07:
     TFT_WriteData(MADCTL_M8 | MADCTL_BGR);
     break;
  }
}

/**
 * @brief Enables color inversion on the display.
 * @return none 
 */
void TFT_InvertDisplay(void)
{
    TFT_WriteCommand(INVERSION_ON);
}

/**
 * @brief Disables color inversion on the display.
 * @return none
 */
void TFT_InvertDisplayOff(void)
{
    TFT_WriteCommand(INVERSION_OFF);
}

/**
 * @brief This function paints the display background a specific color
 * @param color Color of the background
 * @return none
 */
void  TFT_Background(int color)
{
    unsigned char i,j;
    TFT_RamAdress();
     for (i=0;i<160;i++)
        for (j=0;j<128;j++)
            TFT_WriteDataWord(color);
}

/**
 * @brief This function paints an image with a specific heigh and width on the display at a specific coordinate.
 * @param picture[] Integer array containg picture image in rgb565 format
 * @param width Image width
 * @param height Image height
 * @param x X coordinates of image
 * @param y Y coordinates of image
 * @return none
 */
void    TFT_ShowPic(unsigned int picture[],unsigned char width, unsigned char height, unsigned int x, unsigned int y)
{
    unsigned int i,j,k;
    k=0;
    for(i=0;i<width;i++)
    {
        for(j=0;j<height;j++)
        {
            TFT_SetPixel(i+x,j+y,picture[k]);
			k++;
        }
    }
}

/**
 * @brief This function displays an image of type Image_t on screen.
 * @param image Image type structure
 * @param x Start X coordinate 
 * @param y Start Y coordinate 
 * @return none
 */
void    TFT_DisplayImage(Image_t *image, unsigned char x, unsigned char y)
{
    unsigned int i,j,k;
    k=0;
    for(i=0;i<image->width;i++)
    {
        for(j=0;j<image->height;j++)
        {
            TFT_SetPixel(i+x,j+y,image->picture[k]);
			k++;
        }
    }
}

/**
 * @brief Pass 8-bit (each) R,G,B, get back 16-bit packed color
 * @param r Red color
 * @param g Green color
 * @param b Blue color
 * @return 16 bit packed rgb 
 */
unsigned int TFT_Color565(unsigned char r, unsigned char g, unsigned char b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

/**
 * @brief Plot an ASCII char on the display. A specific font is used.
 * @param x X coordinates 
 * @param y Y coordinates
 * @param color Color of numbers above background.
 * @param background Color of text background.
 * @param letter Ascii character to print
 * @param size Size of font 
 * @return none
 */
void TFT_ASCII(char x, char y, int color, int background, char letter, char size)
{
    unsigned char b,q,d,z;
    char data;
    for(q=0;q<5;q++)
    {
        data = font[(unsigned char)letter][q];
        for(z=0;z<8*size;z++)
        {
            if((data&1)!=0){
                for(d=0; d<size;d++)
                {
                    for(b=0; b<size;b++)
                    {
                        TFT_SetPixel(x+(q*size)+d,y+(z*size)+b,color);
                    }
                }
            }else{
                for(d=0; d<size;d++)
                {
                    for(b=0; b<size;b++)
                    {
                        TFT_SetPixel(x+(q*size)+d,y+(z*size)+b,background);
                    }
                }
            }
            data>>=1;
        }
    }
}

/**
 * @brief Orint a colored string at coordinates x,y with a specific font size.
 * @param x Initial X coordinates 
 * @param y Initial Y coordinates
 * @param color Color of numbers above background.
 * @param background Color of text background.
 * @param message Pointer to string array of chars
 * @param size Size of font
 * @return none
 */
void TFT_PrintString(char x, char y, int color, int background, char * message, char size)
{
    while (*message)
    {
        TFT_ASCII(x,y,color,background,*message++, size);
        x+=6*size;
        if(x>120)
        {
            x=0;
            y+=8*size;
        }
    }
}

/**
 * @brief Prints an integer at coordinates x,y with a specific color on a specific background.
 * 		 The integer font size is deterined bu var size.
 * @param x Initial X coordinates 
 * @param y Initial Y coordinates
 * @param color Color of numbers above background.
 * @param background Color of text background.
 * @param integer Number to print as an int
 * @param size Size of font
 * @return none
 */
void TFT_PrintInteger(char x, char y, int color, int background,int integer, char size)
{
    unsigned char tenthousands,thousands,hundreds,tens,ones;
    tenthousands = integer / 10000;
    TFT_ASCII(x,y,color,background,tenthousands+48, size);
    thousands = ((integer - tenthousands*10000)) / 1000;
    x+=6;
    TFT_ASCII(x,y,color,background,thousands+48, size);
    hundreds = (((integer - tenthousands*10000) - thousands*1000)-1) / 100;
    x+=6;
    TFT_ASCII(x,y,color,background,hundreds+48, size);
    tens=(integer%100)/10;
    x+=6;
    TFT_ASCII(x,y,color,background,tens+48, size);
    ones=integer%10;
    x+=6;
    TFT_ASCII(x,y,color,background,ones+48, size);
}

/// @}

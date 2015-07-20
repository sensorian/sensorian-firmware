/**
 * @file Display/Example1/TFT.c
 * @author D.Qendri
 * @date 18 June 2015
 * @brief TFT library.   
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "TFT.h"
#include "SPI.h"
#include "Font.h"

/// \defgroup tft TFT Display 
/// TFT driver for ST7735 controller 1.8 inch display.
/// @{

extern const unsigned char FontASCII8X16[];

/**
 *@brief This function intializes the display controller and prepares it for any subsequent operations.
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
	
	TFT_WriteCommand(SWRESET);
	bcm2835_delay(150);	
    TFT_WriteCommand(SLEEP_OUT);
    bcm2835_delay(600);

	TFT_WriteCommand(FRMCTR1);			// Set ST7735S Frame Rate
	TFT_WriteData(0x01);
	TFT_WriteData(0x2C);
	TFT_WriteData(0x2D);
	TFT_WriteCommand(FRMCTR2);
	TFT_WriteData(0x01);
	TFT_WriteData(0x2C);
	TFT_WriteData(0x2D);
	TFT_WriteCommand(FRMCTR3);
	TFT_WriteData(0x01);
	TFT_WriteData(0x2C);
	TFT_WriteData(0x2D);
	TFT_WriteData(0x01);
	TFT_WriteData(0x2C);
	TFT_WriteData(0x2D);
	
	TFT_WriteCommand(INVCTR); 		// Display Inversion Control
	TFT_WriteData(0x07);
	TFT_WriteCommand(PWCTR1);
	TFT_WriteData(0xA2);
	TFT_WriteData(0x02);
	TFT_WriteData(0x84);
	TFT_WriteCommand(PWCTR2);		
	TFT_WriteData(0XC5);
	TFT_WriteCommand(PWCTR3);		// Power Control 3 (in Normal mode/ Full colors)
	TFT_WriteData(0x0A);
	TFT_WriteData(0x00);
	TFT_WriteCommand(PWCTR4);		// Power Control 4 (in Idle mode/ 8-colors)
	TFT_WriteData(0x8A);
	TFT_WriteData(0x2A);
	TFT_WriteCommand(PWCTR5);
	TFT_WriteData(0x8A);
	TFT_WriteData(0xEE);
	
	TFT_WriteCommand(VMCTR1); // ST7735S Power Sequence
	TFT_WriteData(0x0E);
	TFT_WriteCommand(INVOFF);
	TFT_WriteCommand(MADCTL); //MX, MY, RGB mode
	TFT_WriteData(0xC0);
	TFT_WriteCommand(COLMOD); //65k mode
	TFT_WriteData(0x05);
	
	TFT_WriteCommand(CASET);
	TFT_WriteData(0x00);
	TFT_WriteData(0x7F);
	TFT_WriteCommand(RASET);
	TFT_WriteData(0x00);
	TFT_WriteData(0x9F);
	
	TFT_WriteCommand(GMCTRP1);			// ST7735S Gamma Sequence
	TFT_WriteData(0x02);
	TFT_WriteData(0x1c);
	TFT_WriteData(0x07);
	TFT_WriteData(0x12);
	
	TFT_WriteData(0x37);
	TFT_WriteData(0x32);
	TFT_WriteData(0x29);
	TFT_WriteData(0x2d);
	
	TFT_WriteData(0x29);
	TFT_WriteData(0x25);
	TFT_WriteData(0x2B);
	TFT_WriteData(0x39);
	
	TFT_WriteData(0x00);
	TFT_WriteData(0x01);
	TFT_WriteData(0x03);
	TFT_WriteData(0x10);
	  
	TFT_WriteCommand(GMCTRN1);
	TFT_WriteData(0x03);
	TFT_WriteData(0x1d);
	TFT_WriteData(0x07);
	TFT_WriteData(0x06);
	
	TFT_WriteData(0x2E);
	TFT_WriteData(0x2C);
	TFT_WriteData(0x29);
	TFT_WriteData(0x2D);
	
	TFT_WriteData(0x2E);
	TFT_WriteData(0x2E);
	TFT_WriteData(0x37);
	TFT_WriteData(0x3F);
	
	TFT_WriteData(0x00);
	TFT_WriteData(0x00);
	TFT_WriteData(0x02);
	TFT_WriteData(0x10);

	TFT_WriteCommand(NORON);        // Normal display on, no args, w/delay 10ms 0x13
    bcm2835_delay(100);				// End ST7735S Gamma Sequence
	TFT_WriteCommand(DISPON); 		// Display on
}

/**
 * @brief This function writes a command byte to the display controller.
 * @param command Commnand byte.
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
 * @brief This function is used to write data to the TFT controller.
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
   TFT_WriteCommand(CASET);
   TFT_WriteData(0x00);
   TFT_WriteData(0x00);
   TFT_WriteData(0x00);
   TFT_WriteData(WIDTH);
  
   TFT_WriteCommand(RASET);
   TFT_WriteData(0x00);
   TFT_WriteData(0x00);
   TFT_WriteData(0x00);
   TFT_WriteData(HEIGHT);
   TFT_WriteCommand(RAMWR);
}

/**
 * @brief This functions sets a specific pixel on the TFT display
 * @param x_start Starting X coordinate
 * @param y_start Starting y coordinate
 * @param color Color of the pixel
 * @return none
 */
void TFT_SetPixel(unsigned char x_start,unsigned char y_start,unsigned int color)
{
    TFT_WriteCommand(CASET);
	TFT_WriteData(0x00);
    TFT_WriteData(x_start);                        // XSTART
    TFT_WriteData(0x00);
    TFT_WriteData(x_start+1);                      // XEND

    TFT_WriteCommand(RASET);              // Row addr set
    TFT_WriteData(0x00);
    TFT_WriteData(y_start);                        // YSTART
    TFT_WriteData(0x00);
    TFT_WriteData(y_start+1);                      // YEND

    TFT_WriteCommand(RAMWR);
    TFT_WriteData(color>>8);
    TFT_WriteData(color);
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
    TFT_WriteCommand(DISPOFF);
}

/**
 * @brief This function turns on the display from idle mode.
 * @return none
 */
void TFT_TurnOnDisplay(void)
{
    TFT_WriteCommand(DISPON);
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
    TFT_WriteCommand(INVON);
}

/**
 * @brief Disables color inversion on the display.
 * @return none
 */
void TFT_InvertDisplayOff(void)
{
    TFT_WriteCommand(INVOFF);
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

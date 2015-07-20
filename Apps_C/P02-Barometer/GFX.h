#ifndef __GFX_H__
#define __GFX_H__

#include "TFT.h"

//Screen dimensions
#define SCREEN_HEIGHT    128
#define SCREEN_WIDTH      64

/***********************************************************************************/
//helper functions 

#define absDiff(x,y) ((x>y) ?  (x-y) : (y-x))
#define swap(a,b) \
do\
{\
unsigned char t;\
    t=a;\
    a=b;\
    b=t;\
} while(0)	

/**************************************************************************************/


void TFT_PlotHorizontalLine(unsigned int Xaxis, unsigned int Yaxis,unsigned int width ,unsigned int color);
void TFT_PlotVerticalLine(unsigned int Xaxis,unsigned int Yaxis, unsigned int height ,unsigned int color);
void TFT_PlotLine(unsigned int x1, unsigned int  y1, unsigned int  x2, unsigned int  y2, unsigned int color);
void TFT_PlotSlantyLine(unsigned int lX1, unsigned int lY1, unsigned int lX2,unsigned int lY2,unsigned int color);
void TFT_PlotDegreeLine(unsigned int x, int y,unsigned int degree,unsigned int inner_radius,unsigned int outer_radius, unsigned int color);
double inline TFT_dfloor( double value );

void TFT_PlotFullRectangle(unsigned int Xaxis1, unsigned int Yaxis1, unsigned int Xaxis2 ,unsigned int Yaxis2,unsigned int color);
void TFT_PlotEmptyRectangle(unsigned int Xaxis1,unsigned int Yaxis1, unsigned int Xaxis2,unsigned int Yaxis2,unsigned int color);
void TFT_PlotRoundRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int radius, unsigned int color);

void TFT_PlotCircle(unsigned int CenterX, unsigned int CenterY, unsigned int Radius,unsigned int color);
void TFT_PlotDisk(unsigned int CenterX, unsigned int CenterY, unsigned int Radius,unsigned int color);
void TFT_PlotEllipse(long CX, long  CY, long XRadius,long YRadius, int color);
void inline Plot4EllipsePoints(long CX,long  CY, long X, long Y, int color);

//void TFT_PlotTriangle ( int topx, int topy, int rightx, int righty, int color);     //debug
void TFT_PlotRightTriangle ( int topx, int topy, int rightx, int righty,int color);


#endif

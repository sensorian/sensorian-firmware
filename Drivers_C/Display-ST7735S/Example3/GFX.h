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
 * @file Display/Example3/GFX.h
 * @author D.Qendri
 * @date 30 May 2015
 * @brief GFX library header.   
 */
 
#ifndef __GFX_H__
#define __GFX_H__

#include "TFT.h"

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

void 			TFT_HorizontalLine(unsigned int Xaxis, unsigned int Yaxis,unsigned int width ,unsigned int color);
void 			TFT_VerticalLine(unsigned int Xaxis,unsigned int Yaxis, unsigned int height ,unsigned int color);
void 			TFT_Line(unsigned int x1, unsigned int  y1, unsigned int  x2, unsigned int  y2, unsigned int color);
void 			TFT_SlantyLine(unsigned int lX1, unsigned int lY1, unsigned int lX2,unsigned int lY2,unsigned int color);
double inline 	TFT_dfloor( double value );

void 			TFT_FullRectangle(unsigned int Xaxis1, unsigned int Yaxis1, unsigned int Xaxis2 ,unsigned int Yaxis2,unsigned int color);
void 			TFT_EmptyRectangle(unsigned int Xaxis1,unsigned int Yaxis1, unsigned int Xaxis2,unsigned int Yaxis2,unsigned int color);
void 			TFT_RoundRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int radius, unsigned int color);
	
void 			TFT_Circle(unsigned int CenterX, unsigned int CenterY, unsigned int Radius,unsigned int color);
void 			TFT_DrawCircle(unsigned int CenterX, unsigned int CenterY, unsigned int Radius,unsigned int color);
void 			TFT_Disk(unsigned int CenterX, unsigned int CenterY, unsigned int Radius,unsigned int color);
void 			TFT_Ellipse(long CX, long  CY, long XRadius,long YRadius, unsigned int color);
void inline 	Plot4EllipsePoints(long CX,long  CY, long X, long Y, unsigned int color);

void 			TFT_RightTriangle ( int topx, int topy, int rightx, int righty,unsigned int color);

#endif

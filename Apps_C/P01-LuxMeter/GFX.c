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

 
#include "GFX.h"
#include <stdio.h>
#include <math.h>

/**
 * @brief Draw a horizontal line on the display.
 * @param Xaxis The x coordinate of the origin
 * @param Yaxis The y coordinate of the origin
 * @param width Width of the segment.
 * @param color Color of the line.
 * @return none
 */
void TFT_HorizontalLine(unsigned int Xaxis, unsigned int Yaxis,unsigned int width ,unsigned int color)
{
    TFT_FullRectangle(Xaxis,Yaxis,Xaxis+width,Yaxis,color);
} 

/**
 * @brief Draw a vertical line on the display.
 * @param Xaxis The x coordinate of the origin.
 * @param Yaxis The y coordinate of the origin.
 * @param height Height of the line.
 * @param color Color of the line.
 * @return none
 */
void TFT_VerticalLine(unsigned int Xaxis,unsigned int Yaxis, unsigned int height ,unsigned int color)
{
    TFT_FullRectangle(Xaxis,Yaxis,Xaxis,Yaxis+height,color);
}

/**
 * @brief This function paints a 
 * @param x1    Starting point x coordinate.
 * @param y1    Starting point y coordinate.
 * @param x2    Finish point x coordinate.
 * @param y2    Finish point y coordinate.
 * @param color Color of the line.
 * @return none
 */
void TFT_Line(unsigned int x1, unsigned int  y1, unsigned int  x2, unsigned int  y2, unsigned int color)
{
   unsigned int  deltax, deltay, x,y;
   unsigned char   steep;
   int lerror, ystep;

    steep = absDiff(y1,y2) > absDiff(x1,x2);   //check slope

    if (steep)
    {
        swap(x1, y1);
        swap(x2, y2);
    }

    if (x1 > x2)
    {
        swap(x1, x2);
        swap(y1, y2);
    }

    deltax = x2 - x1;
    deltay = absDiff(y2,y1);  
    lerror = deltax / 2;
    y = y1;
    if(y1 < y2) ystep = 1;  else ystep = -1;

    for(x = x1; x <= x2; x++)
    {
        if (steep) TFT_SetPixel(y,x, color); else TFT_SetPixel(x,y, color);
           lerror -= deltay;
        if (lerror < 0){
            y = y + ystep;
            lerror += deltax;
        }
    }
}

/**
 * @brief Helper funrtion for truncating the double values 
 * @param value Double value to be truncated
 * @return value Ceiling or floor value of the floating point number
 */
double inline TFT_dfloor( double value )
{  
  if (value < 0.0)
    return ceil( value );
  else
    return floor( value );    
}

/**
 * @brief Plot a slanty line from x1,y1 to x2,y2 on the display.
 * @param lX1 Starting point x coordinate.
 * @param lY1 Starting point y coordinate.
 * @param lX2 Finish point x coordinate. 
 * @param lY2 Finish point y coordinate.
 * @param color
 * @return none
 */
void TFT_SlantyLine(unsigned int lX1, unsigned int lY1, unsigned int lX2,unsigned int lY2,unsigned int color)
{
    long lError, lDeltaX, lDeltaY, lYStep, bSteep;       
    
    // A steep line has a bigger ordinate.
    
    if(((lY2 > lY1) ? (lY2 - lY1) : (lY1 - lY2)) > ((lX2 > lX1) ? (lX2 - lX1) : (lX1 - lX2))){
        bSteep = 1;
    }else {
        bSteep = 0;
    }
        
    // If line is steep, swap the X and Y coordinates.
    if(bSteep){
        lError = lX1;
        lX1 = lY1;
        lY1 = lError;
        lError = lX2;
        lX2 = lY2;
        lY2 = lError;
    }
   
    // If the starting X coordinate is larger than the ending X coordinate,
    // swap the start and end coordinates.
    if(lX1 > lX2){
        lError = lX1;
        lX1 = lX2;
        lX2 = lError;
        lError = lY1;
        lY1 = lY2;
        lY2 = lError;
    }
        
    lDeltaX = lX2 - lX1;	 // Compute the difference between the start and end coordinates. 
    lDeltaY = (lY2 > lY1) ? (lY2 - lY1) : (lY1 - lY2);
                                                                   
    lError = -lDeltaX / 2;          // Initialize the error term to negative half the X delta.
     
    if(lY1 < lY2){           // Determine the direction to step in the Y axis when required.
        lYStep = 1;
    }else{
        lYStep = -1;
    }
        
    for(; lX1 <= lX2; lX1++)
	{    // Loop through all the points along the X axis of the line.
        
        // See if this is a steep line.
        
        if(bSteep){            
            // Plot this point of the line, swapping the X and Y coordinates.            
            TFT_SetPixel(lY1, lX1,color);
        }
        else {           // Plot this point of the line, using the coordinates as is.            
            TFT_SetPixel(lX1, lY1,color);
        }                     
        
        // Increment the error term by the Y delta.
        
        lError += lDeltaY; 
                    
        if(lError > 0){        		// See if the error term is now greater than zero.
                     
            lY1 += lYStep;        	// Take a step in the Y axis.
                                                                       
            lError -= lDeltaX;     	// Decrement the error term by the X delta.
        }
    }
}

/**
 * @brief This function paints a filled rectangle on the screen.
 * @param Xaxis1 x coordinate of upper point 
 * @param Yaxis1 y coordinate of upper point 
 * @param Xaxis2 x coordinate of lower point 
 * @param Yaxis2 y coordinate of lower point 
 * @param color Rectangle plot
 * @return none
 */
void TFT_FullRectangle(unsigned int Xaxis1, unsigned int Yaxis1, unsigned int Xaxis2 ,unsigned int Yaxis2,unsigned int color)
{
  unsigned int i = 0;
  unsigned int j = 0;

    for(i=Xaxis1;i<= Xaxis2;i++)
    {
        for(j=Yaxis1;j<=Yaxis2;j++)
        {
            TFT_SetPixel(i,j,color);
        } 
    }   
}  

/**
 * @brief This function paints an empty rectangle on the screen. The width of the contour is 1 pixel.
 * @param Xaxis1 Starting point x coordinate.
 * @param Yaxis1 Starting point y coordinate.
 * @param Xaxis2 Finish point x coordinate. 
 * @param Yaxis2 Finish point y coordinate.
 * @param color Contour color.
 */
void TFT_EmptyRectangle(unsigned int Xaxis1,unsigned int Yaxis1, unsigned int Xaxis2,unsigned int Yaxis2,unsigned int color)
{
      unsigned int CurrentValue;

    /* Draw the two horizontal lines */
      for (CurrentValue = 0; CurrentValue < Xaxis2 - Xaxis1+ 1; CurrentValue++) 
      {
        TFT_SetPixel(Xaxis1 + CurrentValue, Yaxis1,color);
        TFT_SetPixel(Xaxis1 + CurrentValue, Yaxis2,color);
    }
      
      /* draw the two vertical lines */
      for (CurrentValue = 0; CurrentValue < Yaxis2 - Yaxis1 + 1; CurrentValue++)    
      {
        TFT_SetPixel(Xaxis1, Yaxis1 + CurrentValue,color);
        TFT_SetPixel(Xaxis2, Yaxis1 + CurrentValue,color);
    }
}

/**
 * @brief  This function draws a rectangle with round corners
 * @param x   Top left point X coordinate  
 * @param y   Top left pointy Y coordinate  
 * @param width   Rectangle width
 * @param height   Rectangle height
 * @param radius   Radius of the edges
 * @param color  Recatngle color 
 * @return none
 */
void TFT_RoundRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int radius, unsigned int color)
{
      int tSwitch, x1 = 0, y1 = radius;
      tSwitch = 3 - 2 * radius;
    
    while (x1 <= y1) {
        TFT_SetPixel(x+radius - x1, y+radius - y1, color);
        TFT_SetPixel(x+radius - y1, y+radius - x1, color);

        TFT_SetPixel(x+width-radius + x1, y+radius - y1, color);
        TFT_SetPixel(x+width-radius + y1, y+radius - x1, color);
        
        TFT_SetPixel(x+width-radius + x1, y+height-radius + y1, color);
        TFT_SetPixel(x+width-radius + y1, y+height-radius + x1, color);

        TFT_SetPixel(x+radius - x1, y+height-radius + y1, color);
        TFT_SetPixel(x+radius - y1, y+height-radius + x1, color);

        if (tSwitch < 0) {
            tSwitch += (4 * x1 + 6);
        } else {
            tSwitch += (4 * (x1 - y1) + 10);
            y1--;
        }
        x1++;
    }
          
    TFT_HorizontalLine(x+radius,y, width-(2*radius), color);                // top
    TFT_HorizontalLine(x+radius,y+height, width-(2*radius),  color);        // bottom
    TFT_VerticalLine(x,y+radius,height-(2*radius), color);                // left
    TFT_VerticalLine(x+width, y+radius,height-(2*radius),  color);        // right
}

/**
 * @brief This function draws a circle centered a x,y with radius R and specific color.
 * @param CenterX   Center x coordinate
 * @param CenterY   Center y coordinate
 * @param Radius  Circle radius
 * @param color Circle's color.
 * @return none
 */
void TFT_Circle(unsigned int CenterX, unsigned int CenterY, unsigned int Radius,unsigned int color)
{
  unsigned int y=0, x=0, d = 0;
  int part; 
  d = CenterY - CenterX;
  y = Radius;
  part = 3 - 2 * Radius;

  while (x <= y) { 
    TFT_SetPixel(CenterX + x, CenterY + y,color);
    TFT_SetPixel(CenterX + x, CenterY - y,color);
    TFT_SetPixel(CenterX - x, CenterY + y,color);
    TFT_SetPixel(CenterX - x, CenterY - y,color);
    TFT_SetPixel(CenterY + y - d, CenterY + x,color);
    TFT_SetPixel(CenterY + y - d, CenterY - x,color);
    TFT_SetPixel(CenterY - y - d, CenterY + x,color);
    TFT_SetPixel(CenterY - y - d, CenterY - x,color);

    if (part < 0) part += (4 * x + 6);
    else {
      part += (4 * (x - y) + 10);
      y--;
    }
    x++;
  }
}  

/**
 * @brief This function plots a full disk on the display.
 * @param CenterX   Center x coordinate
 * @param CenterY   Center y coordinate
 * @param Radius   Disk radius
 * @param color   Disk Color 
 * @return none
 */
 void TFT_Disk(unsigned int CenterX, unsigned int CenterY, unsigned int Radius,unsigned int color)
 {
	unsigned int i, x, yL, yU;

	//start at Xmiddle-Radius and end at Xmiddle+Radius
	for (x = CenterX - Radius; x <= CenterX + Radius; x++) 
	{
		yL = TFT_dfloor(CenterY - sqrt(Radius * Radius - (x - CenterX) * (x - CenterX)));
		yU = TFT_dfloor(CenterY + sqrt(Radius * Radius - (x - CenterX) * (x - CenterX)));
		//draw vertical lines between X|Yl and X|Yu
		for (i = 0; i < (yU - yL); i++) 
		{
			if (i <= yU && yL >= 0) 
			{
				TFT_SetPixel(x, yL + i, color);
			}
		}
		//draw the upper halve's pixel
		if (yU < 80 && yU > 0) 
		{
			TFT_SetPixel(x, yU, color);
		}
		//the lower pixel
		if (yL < 80 && yL > 0) 
		{
			TFT_SetPixel(x, yL, color);
		}
	}
}

/**
 * @brief This function draws a circle centered a x,y with radius R and specific color.
 * @param CenterX   Center x coordinate
 * @param CenterY   Center y coordinate
 * @param Radius  Circle radius
 * @param color Circle's color.
 * @return none
 */
void TFT_DrawCircle(unsigned int CenterX, unsigned int CenterY, unsigned int Radius,unsigned int color)
{
	unsigned int x, yL, yU;

	for (x = (CenterX - Radius); x <= CenterX + Radius; x++) 
	{
		yL = CenterY - sqrt(Radius * Radius - (x - CenterX) * (x - CenterX));
		yU = CenterY + sqrt(Radius * Radius - (x - CenterX) * (x - CenterX));

		if (yL < 80 && yL > 0) 
		{
			TFT_SetPixel(x, yL, color);
		}
		if (yU < 80 && yU > 0) 
		{
			TFT_SetPixel(x, yU, color);
		}
	}
}

/**
 * @brief This function paints a 
 * @param CX    x coordinate of one side
 * @param CY    y coordinate of one side
 * @param XRadius   x coordinate of other side
 * @param YRadius   y coordinate of other side
 * @param color     Ellipse color 
 * @return none
 */
void TFT_Ellipse(long CX, long  CY, long XRadius,long YRadius, unsigned int color)
{

  long X, Y;
  long XChange, YChange;
  long EllipseError;
  long TwoASquare,TwoBSquare;
  long StoppingX, StoppingY;
  TwoASquare = 2*XRadius*XRadius;
  TwoBSquare = 2*YRadius*YRadius;
  X = XRadius;
  Y = 0;
  XChange = YRadius*YRadius*(1-2*XRadius);
  YChange = XRadius*XRadius;
  EllipseError = 0;
  StoppingX = TwoBSquare*XRadius;
  StoppingY = 0;

  while ( StoppingX >=StoppingY ) 				//first set of points,y'>-1
  {
    Plot4EllipsePoints(CX,CY,X,Y,color);
    Y++;
    StoppingY=StoppingY+ TwoASquare;
    EllipseError = EllipseError+ YChange;
    YChange=YChange+TwoASquare;
    if ((2*EllipseError + XChange) > 0 ) {
      X--;
      StoppingX=StoppingX- TwoBSquare;
      EllipseError=EllipseError+ XChange;
      XChange=XChange+TwoBSquare;
    }
  }

  Y = YRadius;
  X = 0;
  YChange = XRadius*XRadius*(1-2*YRadius);
  XChange = YRadius*YRadius;
  EllipseError = 0;
  StoppingY = TwoASquare*YRadius;
  StoppingX = 0;

  while ( StoppingY >=StoppingX ) 				//{2nd set of points, y'< -1}
  {
    Plot4EllipsePoints(CX,CY,X,Y,color);
    X++;
    StoppingX=StoppingX + TwoBSquare;
    EllipseError=EllipseError+ XChange;
    XChange=XChange+TwoBSquare;
    if ((2*EllipseError + YChange) > 0 ) {
      Y--;
      StoppingY=StoppingY- TwoASquare;
      EllipseError=EllipseError+ YChange;
      YChange=YChange+TwoASquare;
    }
  }
} 

/**
 * @brief Helper function for plotting an ellipse.
 * @param CX    x coordinate of one side
 * @param CY    y coordinate of one side
 * @param X   x coordinate of other side
 * @param Y   y coordinate of other side
 * @param color     Ellipse color 
 * @return none
 */
void inline Plot4EllipsePoints(long CX,long  CY, long X, long Y, unsigned int color)
{
  TFT_SetPixel(CX+X, CY+Y, color); //{point in quadrant 1}
  TFT_SetPixel(CX-X, CY+Y, color); //{point in quadrant 2}
  TFT_SetPixel(CX-X, CY-Y, color); //{point in quadrant 3}
  TFT_SetPixel(CX+X, CY-Y, color); //{point in quadrant 4}
}

/**
 * @brief This function paints a right angled triangle 
 * @param topx x coordinate 
 * @param topy y coordinate 
 * @param rightx x coordinate 
 * @param righty y coordinate 
 * @param color Triangle color
 * @return none
 */
void TFT_RightTriangle ( int topx, int topy, int rightx, int righty,unsigned int color)
{
	//draw rectangle one line at a time
	TFT_Line( topx,topy, rightx,righty,color );    	//draw hypotenuse
	TFT_Line ( topx,righty,topx,topy,color); 		//draw perpendicular
	TFT_Line (topx,righty, rightx,righty,color);  	// draw base
}


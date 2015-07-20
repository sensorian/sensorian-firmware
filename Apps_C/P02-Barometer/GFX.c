#include "GFX.h"
#include <stdio.h>
#include <math.h>

/**
 *
 * @param Xaxis1
 * @param Yaxis1
 * @param Xaxis2
 * @param Yaxis2
 * @param color
 */
void TFT_PlotFullRectangle(unsigned int Xaxis1, unsigned int Yaxis1, unsigned int Xaxis2 ,unsigned int Yaxis2,unsigned int color)
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
 *
 * @param Xaxis1
 * @param Yaxis1
 * @param Xaxis2
 * @param Yaxis2
 * @param color
 */
void TFT_PlotEmptyRectangle(unsigned int Xaxis1,unsigned int Yaxis1, unsigned int Xaxis2,unsigned int Yaxis2,unsigned int color)
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
 * @brief       Draw a rectangle with round corners
 * @param x     x-coordinate of the top left point
 * @param y     y-coordinate of the top left point
 * @param width     rectangle width
 * @param height    rectangle height
 * @param radius    radius of the edges
 * @param color
 */
void TFT_PlotRoundRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int radius, unsigned int color)
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
          
    TFT_PlotHorizontalLine(x+radius,y, width-(2*radius), color);                // top
    TFT_PlotHorizontalLine(x+radius,y+height, width-(2*radius),  color);        // bottom
    TFT_PlotVerticalLine(x,y+radius,height-(2*radius), color);                // left
    TFT_PlotVerticalLine(x+width, y+radius,height-(2*radius),  color);        // right
}

/**
 * @brief Draw a horizontal line
 * @param Xaxis
 * @param Yaxis
 * @param width
 * @param color
 */
void TFT_PlotHorizontalLine(unsigned int Xaxis, unsigned int Yaxis,unsigned int width ,unsigned int color)
{
    TFT_PlotFullRectangle(Xaxis,Yaxis,Xaxis+width,Yaxis,color);
} 


/**
 * @brief Draw a vertical line of a given width starting from X, Y
 * @param Xaxis
 * @param Yaxis
 * @param height Height of line
 * @param color
 */
void TFT_PlotVerticalLine(unsigned int Xaxis,unsigned int Yaxis, unsigned int height ,unsigned int color)
{
    TFT_PlotFullRectangle(Xaxis,Yaxis,Xaxis,Yaxis+height,color);
}

/**
 * @brief       Draws a line from x,y at given degree from inner_radius to outer_radius.
 * @param x
 * @param y
 * @param degree
 * @param inner_radius
 * @param outer_radius
 * @param color
 */
void TFT_PlotDegreeLine(unsigned int x, int y,unsigned int degree,unsigned int inner_radius,unsigned int outer_radius, unsigned int color)
{
  int fx,fy,tx,ty;
  fx = x + TFT_dfloor(inner_radius * sin(degree * 3.14 / 180));
  fy = y - TFT_dfloor(inner_radius * cos(degree * 3.14 / 180));
  tx = x + TFT_dfloor(outer_radius * sin(degree * 3.14 / 180));
  ty = y - TFT_dfloor(outer_radius * cos(degree * 3.14 / 180));
  TFT_PlotSlantyLine(fx,fy,tx,ty,color);
}

/**
 *
 * @param value
 * @return
 */
double inline TFT_dfloor( double value )
{  
  if (value < 0.0)
    return ceil( value );
  else
    return floor( value );    
}

/**
 * @brief Draws a slanty line from x1,y1  to x2,y2
 * @param lX1
 * @param lY1
 * @param lX2
 * @param lY2
 * @param color
 */
void TFT_PlotSlantyLine(unsigned int lX1, unsigned int lY1, unsigned int lX2,unsigned int lY2,unsigned int color){
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
        
    // Compute the difference between the start and end coordinates.      
    lDeltaX = lX2 - lX1;
    lDeltaY = (lY2 > lY1) ? (lY2 - lY1) : (lY1 - lY2);
                                                                   
    lError = -lDeltaX / 2;          // Initialize the error term to negative half the X delta.
     
    if(lY1 < lY2){           // Determine the direction to step in the Y axis when required.
        lYStep = 1;
    }else{
        lYStep = -1;
    }
        
    for(; lX1 <= lX2; lX1++){    // Loop through all the points along the X axis of the line.
        
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
 * 
 * @param x1    x coordinate of inital point
 * @param y1    y coordinate of inital point
 * @param x2    x coordinate of finish point
 * @param y2    y coordinate of finish point
 * @param color
 */
void TFT_PlotLine(unsigned int x1, unsigned int  y1, unsigned int  x2, unsigned int  y2, unsigned int color)
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
 * Draws a circle centered a x,y with radius R and specific color.
 * @param CenterX   center x coordinate
 * @param CenterY   center y coordinate
 * @param Radius    circle radius
 * @param color
 */
void TFT_PlotCircle(unsigned int CenterX, unsigned int CenterY, unsigned int Radius,unsigned int color)
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
 *
 * @param CenterX   center x coordinate
 * @param CenterY   center y coordinate
 * @param Radius    circle radius
 * @param color     Color can be BLACK or WHITE
 */
void TFT_PlotDisk(unsigned int CenterX, unsigned int CenterY, unsigned int Radius,unsigned int color)
{

}                 


/**
 *
 * @param CX    x coordinate of one side
 * @param CY    y coordinate of one side
 * @param XRadius   x coordinate of other side
 * @param YRadius   y coordinate of other side
 * @param color     color can be BLACK or WHITE
 */
void TFT_PlotEllipse(long CX, long  CY, long XRadius,long YRadius, int color)
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
 *
 * @param CX
 * @param CY
 * @param X
 * @param Y
 * @param color
 */
void inline Plot4EllipsePoints(long CX,long  CY, long X, long Y, int color)
{
  TFT_SetPixel(CX+X, CY+Y, color); //{point in quadrant 1}
  TFT_SetPixel(CX-X, CY+Y, color); //{point in quadrant 2}
  TFT_SetPixel(CX-X, CY-Y, color); //{point in quadrant 3}
  TFT_SetPixel(CX+X, CY-Y, color); //{point in quadrant 4}
}														

/**
 *
 * @param topx
 * @param topy
 * @param rightx
 * @param righty
 */
void TFT_PlotRightTriangle ( int topx, int topy, int rightx, int righty,int color)
{
	//draw rectangle one line at a time
	TFT_PlotLine( topx,topy, rightx,righty,color );    	//draw hypotenuse
	TFT_PlotLine ( topx,righty,topx,topy,color); 		//draw perpendicular
	TFT_PlotLine (topx,righty, rightx,righty,color);  	// draw base
	
}

/**
 *
 * @param topx
 * @param topy
 * @param rightx
 * @param righty
 */
void TFT_PlotTriangle ( int topx, int topy, int rightx, int righty, int color)
{
	int base =0;
	base = 2* rightx-topx;
	//draw rectangle one line at a time
	TFT_PlotLine( topx,topy, rightx,righty,color );    			//draw hypotenuse
	TFT_PlotLine ( topx,righty,topx,topy,color); 					//draw perpendicular
	TFT_PlotLine(topx-base/2,righty, rightx,righty,color); 		// draw base
	TFT_PlotLine(topx-base/2, righty, topx,topy,color);			// draw hypotenuse
}

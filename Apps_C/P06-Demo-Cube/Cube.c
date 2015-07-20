#include "Cube.h"

unsigned int X_scale_flag = 0;
unsigned int Y_scale_flag = 0;
unsigned int X_offset_flag = 0;
unsigned int Y_offset_flag = 0;

float a1x, a1y, a1z;
float a2x, a2y, a2z;
float a3x, a3y, a3z;
float a4x, a4y, a4z;
float a5x, a5y, a5z;
float a6x, a6y, a6z;
float a7x, a7y, a7z;
float a8x, a8y, a8z;

float b1x, b1y;
float b2x, b2y;
float b3x, b3y;
float b4x, b4y;
float b5x, b5y;
float b6x, b6y;
float b7x, b7y;
float b8x, b8y;

float b1x_old, b1y_old;
float b2x_old, b2y_old;
float b3x_old, b3y_old;
float b4x_old, b4y_old;
float b5x_old, b5y_old;
float b6x_old, b6y_old;
float b7x_old, b7y_old;
float b8x_old, b8y_old;

void InitializeCube(void)
{
	// define the points of the cube
	a1x = 1;
	a1y = 1;
	a1z = 1;
	
	a2x = 1;
	a2y = 1;
	a2z = -1;
	
	a3x = -1;
	a3y = 1;
	a3z = -1;
	
	a4x = -1;
	a4y = 1;
	a4z = 1;
	
	a5x = 1;
	a5y = -1;
	a5z = 1;
	
	a6x = 1;
	a6y = -1;
	a6z = -1;
	
	a7x = -1;
	a7y = -1;
	a7z = -1;
	
	a8x = -1;
	a8y = -1;
	a8z = 1;
	
	return;
}

void RotateCubeX(float angle)
{
	// rotate the points of the the cube around the X axis
	rotatex(&a1x, &a1y, &a1z, angle); // in Transforms.c
	rotatex(&a2x, &a2y, &a2z, angle); // in Transforms.c
	rotatex(&a3x, &a3y, &a3z, angle); // in Transforms.c
	rotatex(&a4x, &a4y, &a4z, angle); // in Transforms.c
	rotatex(&a5x, &a5y, &a5z, angle); // in Transforms.c
	rotatex(&a6x, &a6y, &a6z, angle); // in Transforms.c
	rotatex(&a7x, &a7y, &a7z, angle); // in Transforms.c
	rotatex(&a8x, &a8y, &a8z, angle); // in Transforms.c
	
	return;
}

void RotateCubeY(float angle)
{
	// rotate the points of the the cube around the Y axis
	rotatey(&a1x, &a1y, &a1z, angle); // in Transforms.c
	rotatey(&a2x, &a2y, &a2z, angle); // in Transforms.c
	rotatey(&a3x, &a3y, &a3z, angle); // in Transforms.c
	rotatey(&a4x, &a4y, &a4z, angle); // in Transforms.c
	rotatey(&a5x, &a5y, &a5z, angle); // in Transforms.c
	rotatey(&a6x, &a6y, &a6z, angle); // in Transforms.c
	rotatey(&a7x, &a7y, &a7z, angle); // in Transforms.c
	rotatey(&a8x, &a8y, &a8z, angle); // in Transforms.c
	
	return;
}

void RotateCubeZ(float angle)
{
	// rotate the points of the cube around the Z axis
	rotatez(&a1x, &a1y, &a1z, angle); // in Transforms.c
	rotatez(&a2x, &a2y, &a2z, angle); // in Transforms.c
	rotatez(&a3x, &a3y, &a3z, angle); // in Transforms.c
	rotatez(&a4x, &a4y, &a4z, angle); // in Transforms.c
	rotatez(&a5x, &a5y, &a5z, angle); // in Transforms.c
	rotatez(&a6x, &a6y, &a6z, angle); // in Transforms.c
	rotatez(&a7x, &a7y, &a7z, angle); // in Transforms.c
	rotatez(&a8x, &a8y, &a8z, angle); // in Transforms.c
	
	return;
}

void SaveOldLinesCube(void)
{
	// save the current points so that 
	// the old cube lines can be erased
	b1x_old = b1x;
	b2x_old = b2x;
	b3x_old = b3x;
	b4x_old = b4x;
	b5x_old = b5x;
	b6x_old = b6x;
	b7x_old = b7x;
	b8x_old = b8x;
	
	b1y_old = b1y;
	b2y_old = b2y;
	b3y_old = b3y;
	b4y_old = b4y;
	b5y_old = b5y;
	b6y_old = b6y;
	b7y_old = b7y;
	b8y_old = b8y;
	
	return;
}

void ProjectCube(void)
{
	// project the 3 points of the cube into the X,Y plane
	project(a1x, a1y, a1z, &b1x, &b1y); // in Transforms.c
	project(a2x, a2y, a2z, &b2x, &b2y); // in Transforms.c
	project(a3x, a3y, a3z, &b3x, &b3y); // in Transforms.c
	project(a4x, a4y, a4z, &b4x, &b4y); // in Transforms.c
	project(a5x, a5y, a5z, &b5x, &b5y); // in Transforms.c
	project(a6x, a6y, a6z, &b6x, &b6y); // in Transforms.c
	project(a7x, a7y, a7z, &b7x, &b7y); // in Transforms.c
	project(a8x, a8y, a8z, &b8x, &b8y); // in Transforms.c
}

void DrawCube(void)
{
	// draw over the old cube line with the background color
	// and draw the new cube lines in the forground color
	TFT_Line((unsigned char) b1x_old,(unsigned char) b1y_old,(unsigned char) b2x_old,(unsigned char) b2y_old, BACKGROUND); // in LCD.c
	TFT_Line((unsigned char) b1x,(unsigned char) b1y,(unsigned char) b2x,(unsigned char) b2y, FOREGROUND); // in LCD.c
	
	TFT_Line((unsigned char) b1x_old,(unsigned char) b1y_old,(unsigned char) b4x_old,(unsigned char) b4y_old, BACKGROUND); // in LCD.c
	TFT_Line((unsigned char) b1x,(unsigned char) b1y,(unsigned char) b4x,(unsigned char) b4y, FOREGROUND); // in LCD.c
	
	TFT_Line((unsigned char) b1x_old,(unsigned char) b1y_old,(unsigned char) b5x_old,(unsigned char) b5y_old, BACKGROUND); // in LCD.c
	TFT_Line((unsigned char) b1x,(unsigned char) b1y,(unsigned char) b5x,(unsigned char) b5y, FOREGROUND); // in LCD.c
	
	TFT_Line((unsigned char) b2x_old,(unsigned char) b2y_old,(unsigned char) b3x_old,(unsigned char) b3y_old, BACKGROUND); // in LCD.c
	TFT_Line((unsigned char) b2x,(unsigned char) b2y,(unsigned char) b3x,(unsigned char) b3y, FOREGROUND); // in LCD.c
	
	TFT_Line((unsigned char) b2x_old,(unsigned char) b2y_old,(unsigned char) b6x_old,(unsigned char) b6y_old, BACKGROUND); // in LCD.c
	TFT_Line((unsigned char) b2x,(unsigned char) b2y,(unsigned char) b6x,(unsigned char) b6y, FOREGROUND); // in LCD.c
	
	TFT_Line((unsigned char) b5x_old,(unsigned char) b5y_old,(unsigned char) b6x_old,(unsigned char) b6y_old, BACKGROUND); // in LCD.c
	TFT_Line((unsigned char) b5x,(unsigned char) b5y,(unsigned char) b6x,(unsigned char) b6y, FOREGROUND); // in LCD.c
	
	TFT_Line((unsigned char) b5x_old,(unsigned char) b5y_old,(unsigned char) b8x_old,(unsigned char) b8y_old, BACKGROUND); // in LCD.c
	TFT_Line((unsigned char) b5x,(unsigned char) b5y,(unsigned char) b8x,(unsigned char) b8y, FOREGROUND); // in LCD.c
	
	TFT_Line((unsigned char) b8x_old,(unsigned char) b8y_old,(unsigned char) b7x_old,(unsigned char) b7y_old, BACKGROUND); // in LCD.c
	TFT_Line((unsigned char) b8x,(unsigned char) b8y,(unsigned char) b7x,(unsigned char) b7y, FOREGROUND); // in LCD.c
	
	TFT_Line((unsigned char) b7x_old,(unsigned char) b7y_old,(unsigned char) b6x_old,(unsigned char) b6y_old, BACKGROUND); // in LCD.c
	TFT_Line((unsigned char) b7x,(unsigned char) b7y,(unsigned char) b6x,(unsigned char) b6y, FOREGROUND); // in LCD.c
	
	TFT_Line((unsigned char) b7x_old,(unsigned char) b7y_old,(unsigned char) b3x_old,(unsigned char) b3y_old, BACKGROUND); // in LCD.c
	TFT_Line((unsigned char) b7x,(unsigned char) b7y,(unsigned char) b3x,(unsigned char) b3y, FOREGROUND); // in LCD.c
	
	TFT_Line((unsigned char) b4x_old,(unsigned char) b4y_old,(unsigned char) b3x_old,(unsigned char) b3y_old, BACKGROUND); // in LCD.c
	TFT_Line((unsigned char) b4x,(unsigned char) b4y,(unsigned char) b3x,(unsigned char) b3y, FOREGROUND); // in LCD.c
	
	TFT_Line((unsigned char) b4x_old,(unsigned char) b4y_old,(unsigned char) b8x_old,(unsigned char) b8y_old, BACKGROUND); // in LCD.c
	TFT_Line((unsigned char) b4x,(unsigned char) b4y,(unsigned char) b8x,(unsigned char) b8y, FOREGROUND); // in LCD.c
	
	// draw a red X on one face
	TFT_Line((unsigned char) b2x_old,(unsigned char) b2y_old,(unsigned char) b4x_old,(unsigned char) b4y_old, BACKGROUND); // in LCD.c
	TFT_Line((unsigned char) b2x,(unsigned char) b2y,(unsigned char) b4x,(unsigned char) b4y, RED);
	
	TFT_Line((unsigned char) b1x_old,(unsigned char) b1y_old,(unsigned char) b3x_old,(unsigned char) b3y_old, BACKGROUND); // in LCD.c
	TFT_Line((unsigned char) b1x,(unsigned char) b1y,(unsigned char) b3x,(unsigned char) b3y, RED);
	
	return;
}

void ScaleDistort(void)  // unused
{
		if(X_scale_flag == 0)
		{
			X_SCALE--;
		}
		else
		{
			X_SCALE++;
		}
		
		if(X_SCALE == 0)
		{
			X_scale_flag = 1;
		}
		if(X_SCALE == 40)
		{
			X_scale_flag = 0;
		}
		
		if(Y_scale_flag == 0)
		{
			Y_SCALE--;
		}
		else
		{
			Y_SCALE++;
		}
		
		if(Y_SCALE == 0)
		{
			Y_scale_flag = 1;
		}
		if(Y_SCALE == 37)
		{
			Y_scale_flag = 0;
		}
		
		return;
}

void OffsetBounce(void) // unused
{
		if(X_offset_flag == 0)
		{
			X_OFFSET--;
		}
		else
		{
			X_OFFSET++;
		}
		
		if(X_OFFSET == 0)
		{
			X_offset_flag = 1;
		}
		if(X_OFFSET == 132)
		{
			X_offset_flag = 0;
		}
		
		if(Y_offset_flag == 0)
		{
			Y_OFFSET--;
		}
		else
		{
			Y_OFFSET++;
		}
		
		if(Y_OFFSET == 0)
		{
			Y_offset_flag = 1;
		}
		if(Y_OFFSET == 132)
		{
			Y_offset_flag = 0;
		}
		
		return;
}

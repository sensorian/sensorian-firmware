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
 * @file P00-Demo-Cube/Utilities.h
 * @author D.Qendri
 * @date 30 May 2015
 * @brief Utilities library header.   
 */

#include "Transforms.h"

float r = 0.2; // z
float p = 0; // x
float q = 0; // y

float X_OFFSET;
float Y_OFFSET;
float X_SCALE; 
float Y_SCALE;

void topolar(float a, float b, float * r, float * theta)
{
	*theta = atan2(b, a);
	*r = sqrt(pow(a,2) + pow(b,2));
	
	return;
}

void torect(float r, float theta, float * a, float * b)
{
	*a = r*cos(theta);
	*b = r*sin(theta);
	
	return;
}

void rotate(float * a, float * b, float theta)
{
	float r, theta_new;
	
	topolar(*a, *b, &r, &theta_new);
	theta_new = theta_new + theta;
	torect(r, theta_new, a, b);
	
	return;
}

void rotatex(float * ax, float * ay, float * az, float theta)
{
	rotate(ay, az, theta);
	
	return;
}

void rotatey(float * ax, float * ay, float * az, float theta)
{
	rotate(az, ax, theta);
	
	return;
}

void rotatez(float * ax, float * ay, float * az, float theta)
{
	rotate(ax, ay, theta);
	
	return;
}

void project(float ax, float ay, float az, float * bx, float * by)
{	
	
	// three point persepctive
	*bx = ax/(p*ax+q*ay+r*az+1)*X_SCALE + X_OFFSET;
	*by = ay/(p*ax+q*ay+r*az+1)*Y_SCALE + Y_OFFSET;
	
	// two point persepctive
//	*bx = ax/(p*ax+q*ay+1)*X_SCALE + X_OFFSET;
//	*by = ay/(p*ax+q*ay+1)*Y_SCALE + Y_OFFSET;
	
	// single point persepctive
//	*bx = ax/(r*az+1)*X_SCALE + X_OFFSET;
//	*by = ay/(r*az+1)*Y_SCALE + Y_OFFSET;
	
	// no perspective
//	*bx = ax*X_SCALE + X_OFFSET;
//	*by = ay*Y_SCALE + Y_OFFSET;
	
	// simple protection to prevent display freakout
	
	if(*bx < 0)
	{
		*bx = 0;
	}
	
	if(*by < 0)
	{
		*by = 0;
	}
	
	return;
}

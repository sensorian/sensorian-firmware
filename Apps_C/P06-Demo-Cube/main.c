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
 * @file P06-Demo-Cube/main.c
 * @author D.Qendri
 * @date 29 Aug 2014
 * @brief 3D cube deom.   
 */
 
#include <stdio.h>
#include <unistd.h>

#include "bcm2835.h"
#include "SPI.h"
#include "TFT.h"
#include "GFX.h"

#include "Cube.h"


float x_rot, y_rot, z_rot;

int main(void)
{
	SPI_Initialize();				//Initialize SPI peripheral
	TFT_Initialize();
	TFT_Background(BLACK);
	
	X_OFFSET = 65.0;
	Y_OFFSET = 65.0;
	X_SCALE = 30.0;
	Y_SCALE = 30.0;
	
	x_rot = 0.02;
	y_rot = 0.035;
	z_rot = 0.055;
	
	InitializeCube();

    while(1)
    {
		RotateCubeX(x_rot); 		// rotate the cube points around the defined axis
		RotateCubeY(y_rot); 
		RotateCubeZ(z_rot); 		
		SaveOldLinesCube(); 		// save the current points		
		ProjectCube();				// perform a 2D project of the 3D object onto the X,Y plane with Z perspective		
		DrawCube(); 				// Draw the cube on the screen
    }
}

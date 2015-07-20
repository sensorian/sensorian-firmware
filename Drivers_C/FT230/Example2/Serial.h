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
 * @file FT230/Example2/Serial.h
 * @author D.Qendri
 * @date 15 Aug 2014
 * @brief Serial port driver header.
 *
 */
 
#ifndef __SERIAL_H__
#define __SERIAL_H__

int   SerialOpen      (int baud) ;
void  SerialClose     (int fd) ;
void  SerialFlush     (int fd) ;
void  SerialPutchar   (int fd,unsigned char c) ;
void  SerialPuts      (int fd,char *s) ;
void  SerialPrintf    (int fd,char const *message, ...) ;
int   SerialDataAvail (int fd) ;
int   SerialGetchar   (int fd) ;


#endif
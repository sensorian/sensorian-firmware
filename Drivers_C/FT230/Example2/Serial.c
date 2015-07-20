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
 * @file FT230/Example2/Serial.c
 * @author D.Qendri
 * @date 15 Aug 2014
 * @brief Serial port driver.
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "Serial.h"

/// \defgroup serial Serial port
/// These functions let you communicate with the CAP1203 capacitive touch controller
/// @{

 /**
 *@brief Open and initialise the serial port with a specific baud rate.
 *@param baud Baud rate for port ttyAMA0
 *@return fd  device file number
 */
int SerialOpen (int baud)
{
  struct termios options ;
  speed_t myBaud ;
  int     status, fd ;

  switch (baud)
  {
    case     50:	myBaud =     B50 ; break ;
    case     75:	myBaud =     B75 ; break ;
    case    110:	myBaud =    B110 ; break ;
    case    134:	myBaud =    B134 ; break ;
    case    150:	myBaud =    B150 ; break ;
    case    200:	myBaud =    B200 ; break ;
    case    300:	myBaud =    B300 ; break ;
    case    600:	myBaud =    B600 ; break ;
    case   1200:	myBaud =   B1200 ; break ;
    case   1800:	myBaud =   B1800 ; break ;
    case   2400:	myBaud =   B2400 ; break ;
    case   9600:	myBaud =   B9600 ; break ;
    case  19200:	myBaud =  B19200 ; break ;
    case  38400:	myBaud =  B38400 ; break ;
    case  57600:	myBaud =  B57600 ; break ;
    case 115200:	myBaud = B115200 ; break ;
    case 230400:	myBaud = B230400 ; break ;

    default:
      return -2 ;
  }

  if ((fd = open ("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) == -1)
  {
	fprintf (stderr, "Cannot open port serial port.\n") ;    
  }

  fcntl (fd, F_SETFL, O_RDWR) ;

// Get and modify current options:

  tcgetattr (fd, &options) ;

  cfmakeraw   (&options) ;
  cfsetispeed (&options, myBaud) ;
  cfsetospeed (&options, myBaud) ;

  options.c_cflag |= (CLOCAL | CREAD) ;
  options.c_cflag &= ~PARENB ;
  options.c_cflag &= ~CSTOPB ;
  options.c_cflag &= ~CSIZE ;
  options.c_cflag |= CS8 ;
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG) ;
  options.c_oflag &= ~OPOST ;

  options.c_cc [VMIN]  =   0 ;
  options.c_cc [VTIME] = 100 ;	// Ten seconds (100 deciseconds)
  tcsetattr (fd, TCSANOW | TCSAFLUSH, &options) ;

  ioctl (fd, TIOCMGET, &status);

  status |= TIOCM_DTR ;
  status |= TIOCM_RTS ;

  ioctl (fd, TIOCMSET, &status);

  usleep (10000) ;	// 10mS

  return fd ;
}

 /**
 *@brief Flush the serial buffers (both tx & rx)
 *@param fd  Device file number
 *@return none
 */
void SerialFlush (int fd)
{
  tcflush (fd, TCIOFLUSH) ;
}

/**
 *@brief Closes the serial port characterized by device number fd.
 *@param fd  device file number
 *@return none
 */
void SerialClose (int fd)
{
  close (fd) ;
}

 /**
 *@brief Send a single character to the serial port
 *@param fd  device file number
 *@param c Single chracter to be sent over the serial port.
 *@return none
 */
void SerialPutchar (int fd, unsigned char c)
{
  write (fd, &c, 1) ;
}

/**
 *@brief Send a string to the serial port
 *@param fd  Device file number.
 *@param s Array of characters to be sent over the serial port.
 *@return none
 */
void SerialPuts (int fd, char *s)
{
  write (fd, s, strlen (s)) ;
}

 /**
 *@brief Printf over Serial Port
 *@param fd  device file number
 *@param *message  String array
 *@return none
 */
void SerialPrintf (int fd, char const *message, ...)
{
  va_list argp ;
  char buffer [1024] ;

  va_start (argp, message) ;
    vsnprintf (buffer, 1023, message, argp) ;
  va_end (argp) ;

  SerialPuts (fd, buffer) ;
}

 /**
 *@brief Return the number of bytes of data avalable to be read from the serial port
 *@param fd  Device file number
 *@return result Number of bytes available from the serial port.
 */
int SerialDataAvail (int fd)
{
  int result ;

  if (ioctl (fd, FIONREAD, &result) == -1)
    return -1 ;

  return result ;
}

 /**
 *@brief Get a single character from the serial device. Note: Zero is a valid character and this function will time-out after
 *	10 seconds.
 *@param fd  device file number
 *@return ch Serial Char received
 */ 
int SerialGetchar (int fd)
{
  uint8_t x ;

  if (read (fd, &x, 1) != 1)
    return -1 ;

  return ((int)x) & 0xFF ;
}

/// @}
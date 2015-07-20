#!/usr/bin/python

import Image
import ImageDraw
import ImageFont
import time
import TFT as GLCD
import os
import socket
import fcntl
import struct

def GetIP_Address(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    return socket.inet_ntoa(fcntl.ioctl(
        s.fileno(),
        0x8915,  # SIOCGIFADDR
        struct.pack('256s', ifname[:15])
    )[20:24])

def GetTempCPU():
	"Returns CPU temp in degrees F"
	tPath = '/sys/class/thermal/thermal_zone0/temp'
	tFile = open(tPath)
	temp = tFile.read()
	tFile.close()
	return (float(temp)/1000)
	
def Run(cmd):
	"Runs a system (bash) command"
	return os.popen(cmd).read()
	
def GetIPAddr():
	"Returns IP address as a string"
	cmd = "ifconfig | awk '/192/ {print $2}'"
	res = Run(cmd).replace('\n','') #remove end of line char
	return res.replace('addr:','')  #remove 'addr:' prefix

def main():
	height = GLCD.TFT_HEIGHT
	width = GLCD.TFT_WIDTH

	disp = GLCD.TFT()		# Create TFT LCD display class.
	disp.initialize()		# Initialize display.
	disp.clear()			# Alternatively can clear to a black screen by calling:
	draw = disp.draw()		# Get a PIL Draw object to start drawing on the display buffer
	font = ImageFont.truetype('/usr/share/fonts/truetype/freefont/FreeSansBold.ttf', 14)		# use a truetype font
	
	while True:
		temp = GetTempCPU()
		draw.text((30, 10), "CPU Temp", font=font)
		draw.text((20, 30), str(temp) + " deg C", font=font)
		ipaddr = GetIP_Address('eth0')
		draw.text((30, 80), "IP Viewer", font=font)
		draw.text((15, 100), str(ipaddr), font=font)	
		disp.display()
		time.sleep(3)
		disp.clear()

if __name__=="__main__":
	try:
		main()
	except KeyboardInterrupt:
		print("...Quitting ...")
		
# /****************************************************************************
 # * Copyright (C) 2015 Sensorian
 # *                                                                          *
 # * This file is part of Sensorian.                                          *
 # *                                                                          *
 # *   Sensorian is free software: you can redistribute it and/or modify it   *
 # *   under the terms of the GNU Lesser General Public License as published  *
 # *   by the Free Software Foundation, either version 3 of the License, or   *
 # *   (at your option) any later version.                                    *
 # *                                                                          *
 # *   Sensorian is distributed in the hope that it will be useful,           *
 # *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 # *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 # *   GNU Lesser General Public License for more details.                    *
 # *                                                                          *
 # *   You should have received a copy of the GNU Lesser General Public       *
 # *   License along with Sensorian.                                          *
 # *   If not, see <http://www.gnu.org/licenses/>.                            *
 # ****************************************************************************/
 
 # /**
 # * @file P04-IP_viwer/IP_Viewer.py
 # * @author D.Qendri
 # * @date 15 July 2014
 # * @brief Print IP on TFT on boot up.   
 # */
#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import mmap

mmap_altitude_file = open('/home/pi/.sensorian/mmap_altitude','a+b')
mmap_altitude = mmap.mmap(mmap_altitude_file.fileno(),0,access=mmap.ACCESS_WRITE)

#Check if multithreaded_socket_server.py is busy sending data
if mmap_altitude[1] == '1':
	sys.exit()

#Request multithreaded_socket_server.py to send data to Node-RED
mmap_altitude[0] = '1'

__author__ = "Michael Lescisin"
__copyright__ = "Copyright Sensorian 2015"

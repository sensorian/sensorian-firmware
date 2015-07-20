#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import mmap

mmap_temperature_file = open('/home/pi/.sensorian/mmap_temperature','a+b')
mmap_temperature = mmap.mmap(mmap_temperature_file.fileno(),0,access=mmap.ACCESS_WRITE)

#Check if multithreaded_socket_server.py is busy sending data
if mmap_temperature[1] == '1':
	sys.exit()

#Request multithreaded_socket_server.py to send data to Node-RED
mmap_temperature[0] = '1'

__author__ = "Michael Lescisin"
__copyright__ = "Copyright Sensorian 2015"

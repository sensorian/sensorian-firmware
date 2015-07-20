#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import mmap

mmap_ambientlight_file = open('/home/pi/.sensorian/mmap_ambientlight','a+b')
mmap_ambientlight = mmap.mmap(mmap_ambientlight_file.fileno(),0,access=mmap.ACCESS_WRITE)

#Check if multithreaded_socket_server.py is busy sending data
if mmap_ambientlight[1] == '1':
	print "DEBUG: server was too busy"
	sys.exit()

#Request multithreaded_socket_server.py to send data to Node-RED
mmap_ambientlight[0] = '1'
print "DEBUG: sucessfully made request to Node-RED"

__author__ = "Michael Lescisin"
__copyright__ = "Copyright Sensorian 2015"

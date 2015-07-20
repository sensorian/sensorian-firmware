#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket
import tft_printer

HOST = "127.0.0.1"
PORT = 7778

server_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
server_socket.bind((HOST,PORT))
server_socket.listen(1)


while True:
	client_socket, client_addr = server_socket.accept()
	data = client_socket.recv(4096)
	data = str(data)
	try:
		mode = int(data[0])
		data = data[1:]
	except:
		print "Mode not specified. Assuming zero."
		mode = 0
	
	if mode == 0:
		tft_printer.screen_print(str(data))
	elif mode == 1:
		tft_printer.screen_print_rotated(str(data),1)
	
	client_socket.close()

server_socket.close()

__author__ = "Michael Lescisin"
__copyright__ = "Copyright Sensorian 2015"

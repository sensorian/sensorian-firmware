#!/usr/bin/env python

""" This is an experimental proxy which has 2 inbound connections which
can bridge a Scratch outbound connection to a Sensorian Scratch Server
outbound connection. This can be useful for bypassing certain firewall
configurations when connecting the Sensorian + RPi to Scratch running
on a Windows/Mac/Linux computer. """

import socket
import threading

LOCAL_ADDRESS = "127.0.0.1"
LOCAL_PORT = 43001
REMOTE_ADDRESS = "0.0.0.0"
REMOTE_PORT = 42001

class SocketAdapter(threading.Thread):
	def __init__(self,c_a,c_b):
		threading.Thread.__init__(self)
		self.connection_a = c_a
		self.connection_b = c_b
	
	def run(self):
		while True:
			data_a = self.connection_a.recv(1024)
			self.connection_b.send(data_a)


remote_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
remote_socket.bind((REMOTE_ADDRESS,REMOTE_PORT))
remote_socket.listen(1)
#Wait for Scratch to connect
remote_connection, remote_addr = remote_socket.accept()

#...now that Scratch has connected we can now get a connection from the sensorian server

local_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
local_socket.bind((LOCAL_ADDRESS,LOCAL_PORT))
local_socket.listen(1)

#Wait for sensorian server to connect
local_connection, local_addr = local_socket.accept()

#... at this point we should have both connections and can begin proxying
print "Beginning proxying..."


connect_a_to_b = SocketAdapter(remote_connection,local_connection)
connect_b_to_a = SocketAdapter(local_connection,remote_connection)

connect_a_to_b.start()
connect_b_to_a.start()

__author__ = "Michael Lescisin"
__copyright__ = "Copyright Sensorian 2015"

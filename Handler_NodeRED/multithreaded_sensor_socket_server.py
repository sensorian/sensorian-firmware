#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket
import threading
import json
import mmap
import SensorsInterface

HOST = "127.0.0.1"
PORT = 7777

sensors = {'touchpad' : 0}
sensors['magnetometer'] = (0,0,0)
sensors['accelerometer'] = (0,0,0)
sensors_lock = threading.Lock()
data_ready_event = threading.Event()

""" Convert a tuple of integers or floats to a tuple of binary
numbers. Any element less than or equal to zero will become 0 while any
element greater than zero will become 1. For example,
mk_bin_tuple((32,12,7)) --> (1,1,1).
And mk_bin_tuple((-2,0,6)) --> (0,0,1) """
def mk_bin_tuple(tup):
	list_tup = []
	for num in tup:
		list_tup.append(int(num > 0))
	return tuple(list_tup)

""" Retrieves a time tuple from a memory-mapped file object by pulling
out characters starting from index. """
def get_time_from_mmap(mmap_obj,index):
	year = int(mmap_obj[index:index+4])
	month = int(mmap_obj[index+4:index+6])
	date = int(mmap_obj[index+6:index+8])
	hour = int(mmap_obj[index+8:index+10])
	minute = int(mmap_obj[index+10:index+12])
	second = int(mmap_obj[index+12:index+14])
	return (year,month,date,hour,minute,second)

""" Retrieves an alarm tuple (time + alarm_mode) from a memory-mapped
file object by pulling out characters starting from index. """
def get_alarm_from_mmap(mmap_obj,index):
	year = int(mmap_obj[index:index+4])
	month = int(mmap_obj[index+4:index+6])
	date = int(mmap_obj[index+6:index+8])
	hour = int(mmap_obj[index+8:index+10])
	minute = int(mmap_obj[index+10:index+12])
	second = int(mmap_obj[index+12:index+14])
	mode = int(mmap_obj[index+14:index+15])
	return (year,month,date,hour,minute,second,mode)


class ServerThread(threading.Thread):
	def __init__(self,cli_soc,cli_addr):
		threading.Thread.__init__(self)
		self.alive = True
		self.client_socket = cli_soc
		self.client_address = cli_addr
	
	def stop(self):
		self.alive = False
		
	def run(self):
		while self.alive:
			try:
				#self.client_socket.sendall("Hello World")
				sensors_lock.acquire()
				current_sensors = sensors
				sensors_lock.release()
				if current_sensors != {}:
					self.client_socket.sendall(str(json.dumps(current_sensors)) + '\n')
					data_ready_event.set()
				
			except:
				self.client_socket.close()
				self.alive = False
		self.client_socket.close()

class SensorMonitor(threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self)
		self.alive = True
		self.mods_count = 0
		self.attempted_mods_count = 0
	
	def stop(self):
		self.alive = False
	
	def run(self):
		global sensors
		
		#self.flask_input = open('read_pipe','w')
		#self.flask_output = open('write_pipe','r')
		self.mmap_file = open('mmap_ipc','a+b')
		self.mmap_buf = mmap.mmap(self.mmap_file.fileno(),0,access=mmap.ACCESS_WRITE)
		
		self.mmap_altitude_file = open('/home/pi/.sensorian/mmap_altitude','a+b')
		self.mmap_altitude = mmap.mmap(self.mmap_altitude_file.fileno(),0,access=mmap.ACCESS_WRITE)
		
		self.mmap_pressure_file = open('/home/pi/.sensorian/mmap_pressure','a+b')
		self.mmap_pressure = mmap.mmap(self.mmap_pressure_file.fileno(),0,access=mmap.ACCESS_WRITE)
		
		self.mmap_temperature_file = open('/home/pi/.sensorian/mmap_temperature','a+b')
		self.mmap_temperature = mmap.mmap(self.mmap_temperature_file.fileno(),0,access=mmap.ACCESS_WRITE)
		
		self.mmap_ambientlight_file = open('/home/pi/.sensorian/mmap_ambientlight','a+b')
		self.mmap_ambientlight = mmap.mmap(self.mmap_ambientlight_file.fileno(),0,access=mmap.ACCESS_WRITE)
		
		SensorsInterface.setupSensorian()
		
		#Setup a dummy alarm for now
		SensorsInterface.setRTCCtime(2016,3,4,5,33,0)
		SensorsInterface.setRTCCalarm(2018,1,1,1,40,20,1) #MINUTES_MATCH mode
		
		prev_touchpad = SensorsInterface.getTouchpad()
		#prev_magnetometer = SensorsInterface.getMagnetometer()
		prev_magnetometer = mk_bin_tuple(SensorsInterface.getMagnetometer())
		prev_accelerometer = mk_bin_tuple(SensorsInterface.getAccelerometer())
		prev_alarmtrigger = SensorsInterface.pollRTCCalarm()
		while self.alive:
			sensors_lock.acquire()
			sensors = {}
			touchpad = SensorsInterface.getTouchpad()
			if touchpad != prev_touchpad:
				print "Touchpad: " + str(touchpad)
				#sensors_lock.acquire()
				sensors['touchpad'] = touchpad
				#sensors_lock.release()
			prev_touchpad = touchpad
			
			#magnetometer = SensorsInterface.getMagnetometer()
			magnetometer = mk_bin_tuple(SensorsInterface.getMagnetometer())
			if magnetometer != prev_magnetometer:
				print "Magnetometer: " + str(magnetometer)
				#sensors_lock.acquire()
				sensors['magnetometer'] = magnetometer
				#sensors_lock.release()
			prev_magnetometer = magnetometer
			
			accelerometer = mk_bin_tuple(SensorsInterface.getAccelerometer())
			if accelerometer != prev_accelerometer:
				print "Accelerometer: " + str(accelerometer)
				sensors['accelerometer'] = accelerometer
			prev_accelerometer = accelerometer
			
			alarmtrigger = SensorsInterface.pollRTCCalarm()
			if alarmtrigger != prev_alarmtrigger:
				print "Alarm Trigger: " + str(alarmtrigger)
				sensors['alarmtrigger'] = alarmtrigger
			prev_alarmtrigger = alarmtrigger
			
			#Poll flask_rtcc_interface.py for new updates to RTCC
			#print "polling flask_rtcc_interface.py"
			#self.flask_input.write("reqUpdate\n")
			#self.flask_input.flush()
			if self.mmap_buf[0] == '1':
				#Update the time
				new_time_tuple = get_time_from_mmap(self.mmap_buf,2)
				SensorsInterface.setRTCCtime(new_time_tuple[0],new_time_tuple[1],new_time_tuple[2],new_time_tuple[3],new_time_tuple[4],new_time_tuple[5])
				print "done setting the time"
				self.mmap_buf[0] = '0' #Clear NEW_TIME
			
			if self.mmap_buf[1] == '1':
				#Update the alarm
				new_alarm_tuple = get_alarm_from_mmap(self.mmap_buf,16)
				SensorsInterface.setRTCCalarm(new_alarm_tuple[0],new_alarm_tuple[1],new_alarm_tuple[2],new_alarm_tuple[3],new_alarm_tuple[4],new_alarm_tuple[5],new_alarm_tuple[6])
				print "done setting the alarm"
				self.mmap_buf[1] = '0' #Clear NEW_ALARM
			
			#Altitude
			if self.mmap_altitude[0] == '1':
				#Send altitude data... but first set the BUSY byte
				self.mmap_altitude[1] = '1'
				#Get the Altitude data
				print "Getting altitude data"
				sensors['altitude'] = str(SensorsInterface.getAltitude())
				self.mmap_altitude[0] = '0' #The request has been completed
				self.mmap_altitude[1] = '0' #...and we are no longer busy
			
			#Pressure
			if self.mmap_pressure[0] == '1':
				#Send pressure data... but first set the BUSY byte
				self.mmap_pressure[1] = '1'
				#Get the Pressure data
				print "Getting pressure data"
				sensors['pressure'] = str(SensorsInterface.getBarometricPressure())
				self.mmap_pressure[0] = '0' #The request has been completed
				self.mmap_pressure[1] = '0' #...and we are no longer busy
			
			#Temperature
			if self.mmap_temperature[0] == '1':
				#Send temperature data... but first set the BUSY byte
				self.mmap_temperature[1] = '1'
				#Get the Temperature data
				print "Getting temperature data"
				sensors['temperature'] = str(SensorsInterface.getTemperature())
				self.mmap_temperature[0] = '0' #The request has been completed
				self.mmap_temperature[1] = '0' #...and we are no longer busy
			
			#AmbientLight
			if self.mmap_ambientlight[0] == '1':
				#Send temperature data... but first set the BUSY byte
				self.mmap_ambientlight[1] = '1'
				#Get the Ambient Light data
				print "Getting ambient light data"
				sensors['ambientlight'] = str(SensorsInterface.getAmbientLight())
				self.mmap_ambientlight[0] = '0' #The request has been completed
				self.mmap_ambientlight[1] = '0' #...and we are no longer busy
			
			sensors_lock.release()
			
			"""If there is new data stored in sensors --> sensors != {},
			then we should make this thread wait until it is sent to the
			socket. """
			if sensors != {}:
				data_ready_event.clear()
				data_ready_event.wait()


server_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
server_socket.bind((HOST,PORT))
server_socket.listen(1)

#Start a thread for listening to Sensorian
sensors_thread = SensorMonitor()
sensors_thread.start()

while True:
	print "Listening for an incomming connection"
	client_socket, client_addr = server_socket.accept()
	new_thread = ServerThread(client_socket, client_addr)
	new_thread.start()

server_socket.close()

__author__ = "Michael Lescisin"
__copyright__ = "Copyright Sensorian 2015"

#!/usr/bin/env python

import asyncore
import socket
import threading
import argparse
import sys
import SensorsInterface
import tft_printer
import mesh_parser

HOST = "127.0.0.1"
PORT = 42001
#PORT = 43001
BUFFER_SIZE = 175
N_SENSORS = 18

SCRATCH_SENSOR_NAME_INPUT = (
	'sensorian-touchpad',
	'sensorian-ambientlight',
	'sensorian-pressure',
	'sensorian-altitude',
	'sensorian-temperature',
	'sensorian-acceleration.x',
	'sensorian-acceleration.y',
	'sensorian-acceleration.z',
	'sensorian-magnetometer.x',
	'sensorian-magnetometer.y',
	'sensorian-magnetometer.z',
	'sensorian-rtcc.year',
	'sensorian-rtcc.month',
	'sensorian-rtcc.date',
	'sensorian-rtcc.hour',
	'sensorian-rtcc.minute',
	'sensorian-rtcc.second',
	'sensorian-rtcc.alarmtrigger')

SCRATCH_SENSOR_NAME_OUTPUT = (
	'sensorian-led',
	'sensorian-tft',
	'sensorian-rtcc.time',
	'sensorian-rtcc.alarmtime',
	'sensorian-rtcc.resetalarm')

#For compatability with Python 2.7 - http://stackoverflow.com/questions/16022556/has-python-3-2-to-bytes-back-ported-to-python-2-7
def to_bytes(n, length, endianess='big'):
	h = '%x' % n
	s = ('0'*(len(h) % 2) + h).zfill(length*2).decode('hex')
	return s if endianess == 'big' else s[::-1]

def make_scratch_command(cmd):
	global scratch_socket
	#length = len(cmd).to_bytes(4, byteorder='big') # Python 2.7 compatability
	length = to_bytes(len(cmd),4,endianess='big')
	#scratch_socket.send(length + cmd.encode("utf-8"))
	return length + cmd.encode("utf-8")

def make_pin_update(pin_index, value):
	sensor_name = SCRATCH_SENSOR_NAME_INPUT[pin_index]
	bcast_str = 'sensor-update "%s" %d' % (sensor_name, value)
	print "Preparing to send :" + str(bcast_str)
	#return bcast_str
	return make_scratch_command(bcast_str)


def sensor_update(data):
	index_is_data = False  # ignore the loop contents if not sensor
	zero_bit_mask = 0  # bit mask showing where zeros should be written
	one_bit_mask = 0  # bit mask showing where ones should be written
	we_should_update_piface = False

	# go through all of the sensors that have been updated
	for i in range(len(data)):
		if index_is_data:
			index_is_data = False
			continue

		sensor_name = data[i].strip('"')

		# if this sensor is a piface output then reflect
		# that update on the board
		if sensor_name in SCRATCH_SENSOR_NAME_OUTPUT:
			we_should_update_piface = True
			pin_index = SCRATCH_SENSOR_NAME_OUTPUT.index(sensor_name)
			#sensor_value = int(data[i + 1])
			sensor_value = str(data[i + 1])
			print str(sensor_name) + " : " + str(sensor_value)
			index_is_data = True
			
			output_handler(pin_index, sensor_value)

def broadcast_update(data):
	global TFT_orientation
	for msg in data:
		if msg == 'sensorian-tft.landscape':
			TFT_orientation_lock.acquire()
			TFT_orientation = 1
			TFT_orientation_lock.release()
		elif msg == 'sensorian-tft.portrait':
			TFT_orientation_lock.acquire()
			TFT_orientation = 0
			TFT_orientation_lock.release()

def output_handler(pin,value):
	#Sensorian-led
	if pin == 0:
		if value == "1":
			SensorsInterface.ledOn()
		else:
			SensorsInterface.ledOff()
	
	#Sensorian-tft
	elif pin == 1:
		print "update sensorian-tft was called"
		#Check TFT availablity
		print "waiting for TFT_available_lock"
		TFT_available_lock.acquire()
		print "acquired TFT_available_lock"
		availability = TFT_available
		TFT_available_lock.release()
		print "released TFT_available_lock"
		if availability == False:
			print "dropping TFT write."
			return
		print "waiting for TFT_orientation_lock"
		TFT_orientation_lock.acquire()
		print "acquired TFT_orientation_lock"
		orientation = TFT_orientation
		#orientation = 1
		TFT_orientation_lock.release()
		print "released TFT_orientation_lock"
		print "Writing to tft"
		#tft_printer.screen_print(str(value))
		print "Trying to acquire TFT_Lock"
		TFT_Lock.acquire()
		print "Acquired TFT_Lock"
		my_tft_writer = TFT_Thread(value,orientation)
		my_tft_writer.start()
		print "End of output handler"
	
	#Sensorian-rtcc.time
	elif pin == 2:
		try:
			#Extract the time
			time = str(value).split(",")
			year = int(time[0])
			month = int(time[1])
			date = int(time[2])
			hour = int(time[3])
			minute = int(time[4])
			second = int(time[5])
			#Set the time
			SensorsInterface.setRTCCtime(year,month,date,hour,minute,second)
			print "Time has been set"
		except:
			print "Error setting the time"
	
	
	#Sensorian-rtcc.alarmtime
	elif pin == 3:
		try:
			#Extract the time and alarm mode
			time = str(value).split(",")
			year = int(time[0])
			month = int(time[1])
			date = int(time[2])
			hour = int(time[3])
			minute = int(time[4])
			second = int(time[5])
			mode = int(time[6])
			#Set the alarm
			SensorsInterface.setRTCCalarm(year,month,date,hour,minute,second,mode)
			print "Alarm time has been set"
		except:
			print "Error setting the alarm"
		
	#Sensorian-rtcc.resetalarm
	elif pin == 4:
		SensorsInterface.resetRTCCalarm()

TFT_Lock = threading.Lock()
TFT_available_lock = threading.Lock()
TFT_available = True #If the TFT is being written to, drop any other attempts to write to it to keep things moving.
TFT_orientation = 0
TFT_orientation_lock = threading.Lock()

""" Call TFT_Lock.acquire() before spawning a TFT_Thread. When the thread is finished writing to the TFT-LCD it will
release TFT_Lock by calling TFT_Lock.release() """
class TFT_Thread(threading.Thread):
	def __init__(self,msg,ori):
		threading.Thread.__init__(self)
		self.message = msg
		self.orientation = ori

	def run(self):
		global TFT_available
		TFT_available_lock.acquire()
		TFT_available = False
		TFT_available_lock.release()
		#tft_printer.screen_print(str(self.message))
		tft_printer.screen_print_rotated(str(self.message),self.orientation)
		TFT_Lock.release()
		TFT_available_lock.acquire()
		TFT_available = True
		TFT_available_lock.release()
		#TFT_Lock.release()

class SensorMonitor(threading.Thread):
	def __init__(self,exp_sens):
		threading.Thread.__init__(self)
		self.sensors = [{}]*N_SENSORS
		self.sensors_lock = threading.Lock()
		self.exported_sensors = exp_sens
		self.has_data_event = threading.Event()
		self.has_data_event.set()
	
	def stop(self):
		self.alive = False
	
	def run(self):
		self.alive = True
		while self.alive:
			self.sensors_lock.acquire()
			
			######################### Touchpad ####################
			cap_button = SensorsInterface.getTouchpad()
			try:
				if cap_button != self.sensors[0]["da"]:
					#self.sensors[0]["da"] = cap_button
					#self.sensors[0]["dr"] = True
					self.sensors[0] = {"da" : cap_button, "dr" : True}
					print "Updating touchpad"
			except:
				#self.sensors[0]["da"] = cap_button
				#self.sensors[0]["dr"] = True
				self.sensors[0] = {"da" : cap_button, "dr" : True}
				print "Updating touchpad"
				
			
			#################### Ambient Light #####################
			ambient_light = SensorsInterface.getAmbientLight()
			try:
				if ambient_light != self.sensors[1]["da"]:
					self.sensors[1] = {"da" : ambient_light, "dr" : True}
			except:
				self.sensors[1] = {"da" : ambient_light, "dr" : True}
			
			##################### Barometric Pressure ################
			pressure = SensorsInterface.getBarometricPressure()
			try:
				if pressure != self.sensors[2]["da"]:
					self.sensors[2] = {"da" : pressure, "dr" : True}
			except:
				self.sensors[2] = {"da" : pressure, "dr" : True}
			
			##################### Altitude ############################
			altitude = SensorsInterface.getAltitude()
			try:
				if altitude != self.sensors[3]["da"]:
					self.sensors[3] = {"da" : altitude, "dr" : True}
			except:
				self.sensors[3] = {"da" : altitude, "dr" : True}
			
			##################### Temperature #########################
			temperature = SensorsInterface.getTemperature()
			try:
				if temperature != self.sensors[4]["da"]:
					self.sensors[4] = {"da" : temperature, "dr" : True}
			except:
				self.sensors[4] = {"da" : temperature, "dr" : True}
			
			
			##################### Accelerometer #######################
			accel = SensorsInterface.getAccelerometer()
			#-------- X component ---------
			accel_x = accel[0]
			try:
				if accel_x != self.sensors[5]["da"]:
					self.sensors[5] = {"da" : accel_x, "dr" : True}
			except:
				self.sensors[5] = {"da" : accel_x, "dr" : True}
			
			
			#---------- Y component --------
			accel_y = accel[1]
			try:
				if accel_y != self.sensors[6]["da"]:
					self.sensors[6] = {"da" : accel_y, "dr" : True}
			except:
				self.sensors[6] = {"da" : accel_y, "dr" : True}
			
			#---------- Z component ----------
			accel_z = accel[2]
			try:
				if accel_z != self.sensors[7]["da"]:
					self.sensors[7] = {"da" : accel_z, "dr" : True}
			except:
				self.sensors[7] = {"da" : accel_z, "dr" : True}
			
			
			##################### Magnetometer #######################
			magneto = SensorsInterface.getMagnetometer()
			#-------- X component ---------
			magneto_x = magneto[0]
			try:
				if magneto_x != self.sensors[8]["da"]:
					self.sensors[8] = {"da" : magneto_x, "dr" : True}
			except:
				self.sensors[8] = {"da" : magneto_x, "dr" : True}
			
			
			#---------- Y component --------
			magneto_y = magneto[1]
			try:
				if magneto_y != self.sensors[9]["da"]:
					self.sensors[9] = {"da" : magneto_y, "dr" : True}
			except:
				self.sensors[9] = {"da" : magneto_y, "dr" : True}
			
			#---------- Z component ----------
			magneto_z = magneto[2]
			try:
				if magneto_z != self.sensors[10]["da"]:
					self.sensors[10] = {"da" : magneto_z, "dr" : True}
			except:
				self.sensors[10] = {"da" : magneto_z, "dr" : True}
			
			###################### RTCC ##############################
			rtcc = SensorsInterface.getRTCCtime()
			
			#----------- year
			rtcc_year = rtcc[0]
			try:
				if rtcc_year != self.sensors[11]["da"]:
					self.sensors[11] = {"da" : rtcc_year, "dr" : True}
			except:
				self.sensors[11] = {"da" : rtcc_year, "dr" : True}
			
			#---------- month
			rtcc_month = rtcc[1]
			try:
				if rtcc_month != self.sensors[12]["da"]:
					self.sensors[12] = {"da" : rtcc_month, "dr" : True}
			except:
				self.sensors[12] = {"da" : rtcc_month, "dr" : True}			
			
			#---------- date
			rtcc_date = rtcc[2]
			try:
				if rtcc_date != self.sensors[13]["da"]:
					self.sensors[13] = {"da" : rtcc_date, "dr" : True}
			except:
				self.sensors[13] = {"da" : rtcc_date, "dr" : True}
			
			#---------- hour
			rtcc_hour = rtcc[3]
			try:
				if rtcc_hour != self.sensors[14]["da"]:
					self.sensors[14] = {"da" : rtcc_hour, "dr" : True}
			except:
				self.sensors[14] = {"da" : rtcc_hour, "dr" : True}
			
			#---------- minute
			rtcc_minute = rtcc[4]
			try:
				if rtcc_minute != self.sensors[15]["da"]:
					self.sensors[15] = {"da" : rtcc_minute, "dr" : True}
			except:
				self.sensors[15] = {"da" : rtcc_minute, "dr" : True}
			
			#---------- second
			rtcc_second = rtcc[5]
			try:
				if rtcc_second != self.sensors[16]["da"]:
					self.sensors[16] = {"da" : rtcc_second, "dr" : True}
			except:
				self.sensors[16] = {"da" : rtcc_second, "dr" : True}
			
			#------------- alarm trigger
			alarm_trigger = SensorsInterface.pollRTCCalarm()
			try:
				if alarm_trigger != self.sensors[17]["da"]:
					self.sensors[17] = {"da" : alarm_trigger, "dr" : True}
			except:
				self.sensors[17] = {"da" : alarm_trigger, "dr" : True}
			
			self.sensors_lock.release()
			
			for s in self.exported_sensors:
				if self.sensors[s]["dr"] == True:
					#Wait here until data has been sent to socket
					self.has_data_event.clear()
					self.has_data_event.wait()
					break
	
	def data_ready(self,index):
		#return True
		status = False
		try:
			status = self.sensors[index]["dr"]
		except:
			pass
		return status
	
	def get_data(self,index):
		self.sensors[index]["dr"] = False
		return self.sensors[index]["da"]


class ScratchHandler(asyncore.dispatcher):
	def __init__(self,s_lst,host=HOST):
		asyncore.dispatcher.__init__(self)
		self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
		self.connect((host,PORT))
		#self.buffer = make_pin_update(0,1)
		self.sensors_list = s_lst
	
	def handle_connect(self):
		print "Sucessfully connected to Scratch!"
	
	def handle_close(self):
		self.close()
	
	def handle_read(self):
		data = self.recv(BUFFER_SIZE)
		data = data[4:]  # get rid of the length info
		print "DATA STR: " + str(data)
		#data = data.split(" ")
		data = mesh_parser.scratch_message_parse(data)
		print "DATA: " + str(data)
		if data[0] == 'sensor-update':
			data = data[1:]
			try:
				print "sensor_update(" + str(data) + ")"
				sensor_update(data)
			except:
				print "Failed to update sensor data"
		
		elif data[0] == 'broadcast':
			data = data[1:]
			try:
				broadcast_update(data)
			except:
				print "Failed to process broadcast data"
	
	def writable(self):
		return True
	
	
	def handle_write(self):
		sensormonitor.sensors_lock.acquire()
		send_str = ""
		
		for i in self.sensors_list:
			if sensormonitor.data_ready(i):
				send_str += make_pin_update(i,sensormonitor.get_data(i))
		
		
		if send_str != "":
			self.send(send_str)
		
		sensormonitor.has_data_event.set()
		sensormonitor.sensors_lock.release()
		

parser = argparse.ArgumentParser(description='Allows the Sensorian to communicate with Scratch.')

parser.add_argument('--all', dest='all_sensors', const=True, default=False, action='store_const', help="Send data from all sensors to Scratch. Slow when Scratch is running on Raspberry Pi.")
parser.add_argument('--touchpad', dest='touchpad', const=True, default=False, action='store_const', help="Send data from touchpad to Scratch.")
parser.add_argument('--light', dest='light', const=True, default=False, action='store_const', help="Send data from ambient light sensor to Scratch.")
parser.add_argument('--pressure', dest='pressure', const=True, default=False, action='store_const', help="Send data from barometric pressure sensor to Scratch.")
parser.add_argument('--altitude', dest='altitude', const=True, default=False, action='store_const', help="Send data from altitude sensor to Scratch.")
parser.add_argument('--temperature', dest='temperature', const=True, default=False, action='store_const', help="Send data from temperature sensor to Scratch.")
parser.add_argument('--accelerometer', dest='accelerometer', const=True, default=False, action='store_const', help="Send data from accelerometer to Scratch.")
parser.add_argument('--magnetometer', dest='magnetometer', const=True, default=False, action='store_const', help="Send data from magnetometer to Scratch.")
parser.add_argument('--rtcc', dest='rtcc', const=True, default=False, action='store_const', help="Send data from the Real-Time clock to Scratch.")

parser.add_argument('--connect', dest='connect', nargs=1, help="Connect to a Scratch session other than localhost.")

args = parser.parse_args()

#print vars(args)['connect'][0]

var_args = vars(args)
display_help = True
for arg in ['all_sensors','touchpad','light','pressure','altitude','temperature','accelerometer','magnetometer','rtcc']:
	if var_args[arg]:
		display_help = False
		break

if display_help:
	parser.print_help()
	sys.exit()

selected_sensor_numbers = [] #List of sensors to send data to Scratch
if var_args['all_sensors']:
	selected_sensor_numbers = range(0,N_SENSORS)
else:
	if var_args['touchpad']:
		selected_sensor_numbers.append(0)
	if var_args['light']:
		selected_sensor_numbers.append(1)
	if var_args['pressure']:
		selected_sensor_numbers.append(2)
	if var_args['altitude']:
		selected_sensor_numbers.append(3)
	if var_args['temperature']:
		selected_sensor_numbers.append(4)
	if var_args['accelerometer']:
		selected_sensor_numbers.append(5)
		selected_sensor_numbers.append(6)
		selected_sensor_numbers.append(7)
	if var_args['magnetometer']:
		selected_sensor_numbers.append(8)
		selected_sensor_numbers.append(9)
		selected_sensor_numbers.append(10)
	if var_args['rtcc']:
		selected_sensor_numbers.append(11)
		selected_sensor_numbers.append(12)
		selected_sensor_numbers.append(13)
		selected_sensor_numbers.append(14)
		selected_sensor_numbers.append(15)
		selected_sensor_numbers.append(16)
		selected_sensor_numbers.append(17)

SensorsInterface.setupSensorian()

#Ensure all alarms are off
SensorsInterface.resetRTCCalarm()

sensormonitor = SensorMonitor(selected_sensor_numbers)
sensormonitor.start()

if var_args['connect']:
	remote_host = var_args['connect'][0]
	client = ScratchHandler(selected_sensor_numbers, host=remote_host)
else:
	client = ScratchHandler(selected_sensor_numbers)

asyncore.loop()

__author__ = "Michael Lescisin"
__copyright__ = "Copyright Sensorian 2015"

#Based on PiFace Scratch interface code from: https://github.com/piface/pifacedigital-scratch-handler

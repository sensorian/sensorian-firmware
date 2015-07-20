#!/usr/bin/python
import time 
import math
import datetime
from flask import jsonify
from flask import Flask, render_template
import APDS9300 as LuxSens
import RPi.GPIO as GPIO
import MPL3115A2 as altibar
from ctypes import *

app = Flask(__name__)

sensor = CDLL("./libMPL.so")						#load the shared library

@app.route("/")
def hello():
	now = datetime.datetime.now()
	timeString = now.strftime("%Y-%m-%d %H:%M")
	templateData = {
		'title' : 'HELLO!',
		'time': timeString
		}
	return render_template('index.html', **templateData)

@app.route('/light')
def readAmbientLight():
	AmbientLight = LuxSens.APDS9300()			#Setup Ambient light sensor 
	try:
		channel1 = AmbientLight.readChannel(1)				#Take a reading from channel one
		channel2 = AmbientLight.readChannel(0)				#Take a reading from channel two
		Lux = AmbientLight.getLuxLevel(channel1,channel2)		 
	except:
		response = "There was an error reading the sensor ."

	templateData = {
		'title' : 'Ambient Light Level ',
		'Channel1' : channel1,
		'Channel2' : channel2,
		'Luxlevel' : Lux  
		}
	return render_template('light.html', **templateData)
	
@app.route('/Sensorian')
def readMPL():
	sensor.I2C_Initialize(altibar.MPL3115A2_ADDRESS)	#initialize I2C and BCM library
	AltiBar = altibar.MPL3115A2()				#initialize sensor
	time.sleep(0.5)
	AltiBar.ActiveMode()						#puts sensor in active mode
	time.sleep(0.5)
	temperature = AltiBar.ReadTemperature()
	time.sleep(0.5)
	AltiBar.AltimeterMode()
	time.sleep(0.5)
	altitude =  AltiBar.ReadAltitude()		#Take a pressure reading
	time.sleep(0.5)
	AltiBar.BarometerMode()	#puts sensor in active mode
	time.sleep(0.5)
	baroPressure =  AltiBar.ReadBarometricPressure()		#Take a pressure reading
	
	templateData = {
		'Title' : 'Sensorian Dashboard ',
		'temperature' : temperature,
		'altitude' : altitude/1000,
		'pressure' : baroPressure  
		}
	return render_template('Sensorian.html', **templateData)
   
@app.route('/GetLight')
def ReadSensor():
	AmbientLight = LuxSens.APDS9300()			#Setup Ambient light sensor 
	channel1 = AmbientLight.readChannel(1)				#Take a reading from channel one
	channel2 = AmbientLight.readChannel(0)				#Take a reading from channel two
	Lux = AmbientLight.getLuxLevel(channel1,channel2)
	
	return jsonify(CH1 = channel1,CH2 = channel2, LuxVAL = Lux)

@app.route('/GetTemperature')
def ReadTemperature():
	sensor.I2C_Initialize(altibar.MPL3115A2_ADDRESS)	#initialize I2C and BCM library
	AltiBar = altibar.MPL3115A2()				#initialize sensor
	time.sleep(1)
	AltiBar.ActiveMode()						#puts sensor in active mode
	time.sleep(1)
	temperature = AltiBar.ReadTemperature()
	print "temp %f" % temperature
	
	return jsonify(Temp = temperature)

@app.route('/GetAltitude')
def ReadAltimeter():
	sensor.I2C_Initialize(altibar.MPL3115A2_ADDRESS)	#initialize I2C and BCM library
	AltiBar = altibar.MPL3115A2()				#initialize sensor
	AltiBar.ActiveMode()						#puts sensor in active mode
	time.sleep(1)
	AltiBar.AltimeterMode()
	time.sleep(1)
	altitude =  AltiBar.ReadAltitude()		#Take a pressure reading
	
	return jsonify(Alt = altitude)
	
@app.route('/GetPressure')
def ReadBarometer():
	sensor.I2C_Initialize(altibar.MPL3115A2_ADDRESS)	#initialize I2C and BCM library
	AltiBar = altibar.MPL3115A2()				#initialize sensor
	
	AltiBar.ActiveMode()						#puts sensor in active mode
	time.sleep(1)
	AltiBar.BarometerMode()	#puts sensor in active mode
	time.sleep(1)
	baroPressure =  AltiBar.ReadBarometricPressure()		#Take a pressure reading
	
	return jsonify(Barometer = baroPressure)

if __name__ == "__main__":
   app.run(host='0.0.0.0', port=5050, debug=True)
  
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
 # * @file P05-FlaskSensors/App.py
 # * @author D.Qendri
 # * @date 15 July 2014
 # * @brief Flask server with Ajax Sensor dashboard.   
 # */
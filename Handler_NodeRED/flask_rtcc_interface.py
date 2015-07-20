#!/usr/bin/env python
# -*- coding: utf-8 -*-

import random
import string
import mmap
from flask import Flask, render_template, request, send_from_directory, session
from flask_wtf.csrf import CsrfProtect


app = Flask(__name__,static_url_path='')
app.secret_key = ''.join(random.SystemRandom().choice(string.lowercase + string.digits) for _ in xrange(32))
CsrfProtect(app)

""" Writes information from time_tuple into a mmap'ed file mmap_obj so
that it may be understood by multithreaded_socket_server.py  """
def write_time_mmap(time_tuple, mmap_obj):
	year = time_tuple[0]
	month = time_tuple[1]
	date = time_tuple[2]
	hour = time_tuple[3]
	minute = time_tuple[4]
	second = time_tuple[5]
	memory_image = "" #The string of chars to be written into the mmap
	memory_image += str(year).zfill(4)
	memory_image += str(month).zfill(2)
	memory_image += str(date).zfill(2)
	memory_image += str(hour).zfill(2)
	memory_image += str(minute).zfill(2)
	memory_image += str(second).zfill(2)
	#Write memory_image into the mmap
	for c_index in range(0,len(memory_image)):
		mmap_obj[2 + c_index] = memory_image[c_index]


""" Writes information from alarm_tuple into a mmap'ed file mmap_obj so
that it may be understood by multithreaded_socket_server.py  """
def write_alarm_mmap(alarm_tuple, mmap_obj):
	year = alarm_tuple[0]
	month = alarm_tuple[1]
	date = alarm_tuple[2]
	hour = alarm_tuple[3]
	minute = alarm_tuple[4]
	second = alarm_tuple[5]
	mode = alarm_tuple[6]
	memory_image = "" #The string of chars to be written into the mmap
	memory_image += str(year).zfill(4)
	memory_image += str(month).zfill(2)
	memory_image += str(date).zfill(2)
	memory_image += str(hour).zfill(2)
	memory_image += str(minute).zfill(2)
	memory_image += str(second).zfill(2)
	memory_image += str(mode).zfill(1)
	#Write memory_image into the mmap
	for c_index in range(0,len(memory_image)):
		mmap_obj[16 + c_index] = memory_image[c_index]

@app.route("/")
def main_page():
	#Generate an anti-CSRF token for this session
	session["anti_csrf"] = ''.join(random.SystemRandom().choice(string.lowercase + string.digits) for _ in xrange(32))
	print "SESSION"
	print session
	return render_template("index.html", anti_csrf=session["anti_csrf"])


@app.route("/time_upload",methods=["POST"])
def time_upload():
	global mmap_buf
	print request.form
	try:
		#print request.form["eeprom_upload"]
		#normem_data = str(request.form["eeprom_upload"])
		#print "Pages..."
		#print pretty_print_pages(create_normem_pages(normem_data))
		year = int(request.form["time_year"])
		month = int(request.form["time_month"])
		date = int(request.form["time_date"])
		hour = int(request.form["time_hour"])
		minute = int(request.form["time_minute"])
		second = int(request.form["time_second"])
		#req = "time," + str(year) + "," + str(month) + "," + str(date) + "," + str(hour) + "," + str(minute) + "," + str(second)
		req = (year,month,date,hour,minute,second)
		write_time_mmap(req,mmap_buf)
		mmap_buf[0] = '1' #Set NEW_TIME
		
	except:
		print "Bad upload POST request."
		return "Bad upload POST request."
		
	return render_template("index.html", anti_csrf=session["anti_csrf"])

@app.route("/alarm_upload",methods=["POST"])
def alarm_upload():
	global mmap_buf
	print request.form
	try:
		#print request.form["eeprom_upload"]
		#normem_data = str(request.form["eeprom_upload"])
		#print "Pages..."
		#print pretty_print_pages(create_normem_pages(normem_data))
		year = int(request.form["alarm_year"])
		month = int(request.form["alarm_month"])
		date = int(request.form["alarm_date"])
		hour = int(request.form["alarm_hour"])
		minute = int(request.form["alarm_minute"])
		second = int(request.form["alarm_second"])
		mode = int(request.form["alarm_mode"])
		#req = "alarm," + str(year) + "," + str(month) + "," + str(date) + "," + str(hour) + "," + str(minute) + "," + str(second) + "," + str(mode)
		req = (year,month,date,hour,minute,second,mode)
		write_alarm_mmap(req,mmap_buf)
		mmap_buf[1] = '1' #Set NEW_ALARM
		
	except:
		print "Bad upload POST request."
		return "Bad upload POST request."
		
	return render_template("index.html", anti_csrf=session["anti_csrf"])


if __name__ == "__main__":
	mmap_file = open('mmap_ipc','a+b')
	mmap_buf = mmap.mmap(mmap_file.fileno(),0,access=mmap.ACCESS_WRITE)
	app.run(host='0.0.0.0', debug=False) # REMOVE 'debug=True' THIS IS A HUGE SECURITY ISSUE!!!!!!!!!!

__author__ = "Michael Lescisin"
__copyright__ = "Copyright Sensorian 2015"

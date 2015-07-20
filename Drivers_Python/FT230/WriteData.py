#!/usr/bin/python

import serial

def main():
	ser = serial.Serial('/dev/ttyAMA0',115200,timeout=3) 	# Open the serial port
	ser.write("X-shield UART Demo\n")					# Write a serial string to the serial port
	print "Serial closed"
	ser.close()											# Close the serial port


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print("")
		

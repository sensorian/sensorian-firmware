#!/usr/bin/python

import serial

def main():
	ser = serial.Serial('/dev/ttyAMA0',9600)
	ser.write('Read data from terminal')

	while True:
		c = ser.read()
		print(c)

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print("")

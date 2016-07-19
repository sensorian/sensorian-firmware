#!/usr/bin/env python

"""SensorsInterface.py: Controls the various sensors and modules on the Sensorian Shield"""

from ctypes import *
import time
import calendar

## @var lib_sensorian
# Points to the C Shared Object DLL which is used to call the Sensorian C functions
lib_sensorian = CDLL("./libsensorian.so")


def setupSensorian():
    """Sets up the Sensorian Shield.

    Call the C version of the function using the DLL to set up all the Sensorian sensors and LCD.
    """
    lib_sensorian.setupSensorian()


def ledOn():
    """Turns on the orange LED on the Sensorian.

    Call the C version of the function using the DLL to turn on the Sensorian's orange LED.
    """
    lib_sensorian.LED_on()


def ledOff():
    """Turns of the orange LED on the Sensorian.

    Call the C version of the function using the DLL to turn off the Sensorian's orange LED.
    """
    lib_sensorian.LED_off()


def getAmbientLight():
    """Gets a float of the ambient light level.

    Call the C version of the function using the DLL to get the ambient light level.
    """
    lightCall = lib_sensorian.getAmbientLight  # Creates a C function to be called later
    lightCall.restype = c_float  # Tells Python to expect a return type of float rather than the default of int
    lightString = str(lightCall())  # Calls the C function and stores the result in a string
    return float(lightString)  # Returns a float when called

## @var mpl_last_polled
# Used to ensure the temperature/altitude/pressure sensor is not polled too often
mpl_last_polled = -1


def getTemperature():
    """Gets an int of the ambient temperature.

    Call the C version of the function using the DLL to get the ambient temperature.
    """
    global mpl_last_polled  # Declares mpl_last_polled to be global since all three sensors share the timer
    if mpl_last_polled == -1:  # If this is the first time the sensor is polled, it is okay to poll it
        print "Polling MPL3115A2"
        lib_sensorian.pollMPL()  # Polls the sensor since it is confirmed okay to do so
        mpl_last_polled = time.time()  # Sets the last time the sensor was polled to the current time

    elif time.time() - mpl_last_polled > 30:  # If it has been at least 30 seconds since the last poll, it is safe
        print "Polling MPL3115A2"
        lib_sensorian.pollMPL()  # Polls the sensor since it is confirmed okay to do so
        mpl_last_polled = time.time()  # Sets the last time the sensor was polled to the current time

    return lib_sensorian.getTemperature()  # Calls the C function to get the current temperature and return an int


def getAltitude():
    """Gets an int of the altitude.

    Call the C version of the function using the DLL to get the altitude.
    """
    global mpl_last_polled  # Declares mpl_last_polled to be global since all three sensors share the timer
    if mpl_last_polled == -1:  # If this is the first time the sensor is polled, it is okay to poll it
        print "Polling MPL3115A2"
        lib_sensorian.pollMPL()  # Polls the sensor since it is confirmed okay to do so
        mpl_last_polled = time.time()  # Sets the last time the sensor was polled to the current time

    elif time.time() - mpl_last_polled > 30:  # If it has been at least 30 seconds since the last poll, it is safe
        print "Polling MPL3115A2"
        lib_sensorian.pollMPL()  # Polls the sensor since it is confirmed okay to do so
        mpl_last_polled = time.time()  # Sets the last time the sensor was polled to the current time

    return lib_sensorian.getAltitude()  # Calls the C function to get the current altitude and return an int


def getBarometricPressure():
    """Gets an int of the barometric pressure.

    Call the C version of the function using the DLL to get the barometric pressure.
    """
    global mpl_last_polled  # Declares mpl_last_polled to be global since all three sensors share the timer
    if mpl_last_polled == -1:  # If this is the first time the sensor is polled, it is okay to poll it
        print "Polling MPL3115A2"
        lib_sensorian.pollMPL()  # Polls the sensor since it is confirmed okay to do so
        mpl_last_polled = time.time()  # Sets the last time the sensor was polled to the current time

    elif time.time() - mpl_last_polled > 30:  # If it has been at least 30 seconds since the last poll, it is safe
        print "Polling MPL3115A2"
        lib_sensorian.pollMPL()  # Polls the sensor since it is confirmed okay to do so
        mpl_last_polled = time.time()  # Sets the last time the sensor was polled to the current time

    return lib_sensorian.getBarometricPressure()  # Calls the C function to get the current pressure and return an int


def getTouchpad():
    """Gets an int of the capacitive button state.

    Call the C version of the function using the DLL to get the capacitive button state.
    """
    return lib_sensorian.CAP1203_ReadPressedButton()

## @var fxos_last_polled
# Used to ensure the Accelerometer/Magnetometer sensor is not polled too often
fxos_last_polled = -1


def getAccelerometer():
    """Gets ints for the x, y and z forces of acceleration.

    Call the C version of the function using the DLL to get the forces of acceleration.
    """
    global fxos_last_polled  # Declares fxos_last_polled to be global since the two sensors share the timer
    if fxos_last_polled == -1:  # If this is the first time the sensor is polled, it is okay to poll it
        lib_sensorian.pollFXOS()  # Polls the sensor since it is confirmed okay to do so
        fxos_last_polled = time.time()  # Sets the last time the sensor was polled to the current time
    elif time.time() - fxos_last_polled > 1:  # If it has been at least 1 second since the last poll, it is safe
        lib_sensorian.pollFXOS()  # Polls the sensor since it is confirmed okay to do so
        fxos_last_polled = time.time()  # Sets the last time the sensor was polled to the current time

    ac_x = lib_sensorian.getAccelX()  # Calls the C function to get the current x forces which returns an int
    ac_y = lib_sensorian.getAccelY()  # Calls the C function to get the current y forces which returns an int
    ac_z = lib_sensorian.getAccelZ()  # Calls the C function to get the current z forces which returns an int
    return ac_x, ac_y, ac_z  # Return all three of the forces from the accelerometer as integers


def getMagnetometer():
    """Gets ints for the x, y and z magnetic forces.

    Call the C version of the function using the DLL to get the magnetic forces.
    """
    global fxos_last_polled  # Declares fxos_last_polled to be global since the two sensors share the timer
    if fxos_last_polled == -1:  # If this is the first time the sensor is polled, it is okay to poll it
        lib_sensorian.pollFXOS()  # Polls the sensor since it is confirmed okay to do so
        fxos_last_polled = time.time()  # Sets the last time the sensor was polled to the current time
    elif time.time() - fxos_last_polled > 1:  # If it has been at least 1 second since the last poll, it is safe
        lib_sensorian.pollFXOS()  # Polls the sensor since it is confirmed okay to do so
        fxos_last_polled = time.time()  # Sets the last time the sensor was polled to the current time

    mag_x = lib_sensorian.getMagX()  # Calls the C function to get the current x magnetic forces which returns an int
    mag_y = lib_sensorian.getMagY()  # Calls the C function to get the current y magnetic forces which returns an int
    mag_z = lib_sensorian.getMagZ()  # Calls the C function to get the current z magnetic forces which returns an int
    return mag_x, mag_y, mag_z  # Return all three of the magnetic forces from the magnetometer as integers


def getRTCCtime():
    """Gets ints for the date and time values.

    Call the C version of the function using the DLL to get the date and time.
    """
    lib_sensorian.poll_rtcc()  # Call the C function to poll the real time clock, which has no limit on frequency
    # Call the respective C functions to get the year,month,date,hour,minute,second as integers
    year = lib_sensorian.get_rtcc_year()
    month = lib_sensorian.get_rtcc_month()
    date = lib_sensorian.get_rtcc_date()
    hour = lib_sensorian.get_rtcc_hour()
    minute = lib_sensorian.get_rtcc_minute()
    second = lib_sensorian.get_rtcc_second()
    return year, month, date, hour, minute, second  # Return all the real time clock values as integers


def setRTCCtime(year, month, date, hour, minute, second):
    """Sets the date and time using given values.

    Call the C version of the function using the DLL to set the date and time.
    """
    # Check that the arguments are reasonable
    if year < 1900:  # Cannot set the year to before the year 1900
        print "Invalid year"
        return

    if month not in range(1, 13):  # There are only 12 months in a year
        print "Invalid month"
        return

    if date not in range(1, 32):  # There are at most 31 days in a month
        print "Invalid date"
        return

    if hour not in range(0, 24):  # There are hours 0 to 23 in a 24 hour clock
        print "Invalid hour"
        return

    if minute not in range(0, 60):  # There are 0 to 59 minutes in an hour
        print "Invalid minute"
        return

    if second not in range(0, 60):  # There are 0 to 59 seconds in a minute
        print "Invalid second"
        return

    w_day = calendar.weekday(year, month, date)  # Get the weekday for the given year, month, date
    # Set the values if valid to the Sensorian Real Time Clock and Calendar
    lib_sensorian.set_rtcc_datetime(((year + 1900) - 2000) % 100, month, date, w_day, hour, minute, 0)


def setRTCCalarm(year, month, date, hour, minute, second, mode):
    """Sets an alarm for a date and time using given values.

    Call the C version of the function using the DLL to set the alarm at the given date and time.
    """
    # Check that the arguments are reasonable
    if year < 1900:  # Cannot set the year to before the year 1900
        print "Invalid year"
        return

    if month not in range(1, 13):  # There are only 12 months in a year
        print "Invalid month"
        return

    if date not in range(1, 32):  # There are at most 31 days in a month
        print "Invalid date"
        return

    if hour not in range(0, 24):  # There are hours 0 to 23 in a 24 hour clock
        print "Invalid hour"
        return

    if minute not in range(0, 60):  # There are 0 to 59 minutes in an hour
        print "Invalid minute"
        return

    if second not in range(0, 60):  # There are 0 to 59 seconds in a minute
        print "Invalid second"
        return

    w_day = calendar.weekday(year, month, date)  # Get the weekday for the given year, month, date
    # Set the values if valid to an alarm on the Sensorian Real Time Clock and Calendar
    lib_sensorian.set_rtcc_alarm(((year + 1900) - 2000) % 100, month, date, w_day, hour, minute, second, mode)


def pollRTCCalarm():
    """Check for alarms on the real time clock.

    Call the C version of the function using the DLL to check for alarms on the real time clock.
    """
    return lib_sensorian.poll_rtcc_alarm()


def resetRTCCalarm():
    """Clear an alarm state on the real time clock.

    Call the C version of the function using the DLL to clear an alarm state on the real time clock.
    """
    lib_sensorian.reset_alarm()


__author__ = "Michael Lescisin"
__maintainer__ = "Dylan Kauling"
__copyright__ = "Copyright Sensorian 2015"
__status__ = "Development"

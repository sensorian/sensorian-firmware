/**
 * @file SensorsInterface.c
 * @author Michael Lescisin
 * @author Dylan Kauling
 * @date 06 July 2016
 * @brief Controls the various sensors and modules on the Sensorian Shield
 */

#include <bcm2835.h>
#include <stdio.h>
#include <unistd.h>
#include "MPL3115A2.h"
#include "APDS9300.h"
#include "CAP1203.h"
#include "FXOS8700CQ.h"
#include "MCP79410.h"
#include "led.h"
#include "i2c.h"
#include "Utilities.h"
#include "SensorsInterface.h"

RTCC_Struct *current_time; /*!< Stores last polled date and time */


/**
 * @brief Sets up all the Sensorian sensors, buttons, clock and LED for use by the program
 * @return 0 upon successful setup
 */
int setupSensorian(void)
{
	printf("Initializing sensors.\r\n");
	I2C_Initialize(); //Initialize I2C BCM library
	
	MPL3115A2_Initialize();		//Initialize Temp/Baro/Alti sensor
	MPL3115A2_ActiveMode();		//Configure the sensor for active mode
	
	AL_Initialize(); //Setup Ambient Light sensor
	
	CAP1203_Initialize(); //Setup Capacitive Touch sensor
	
	FXOS8700CQ_Initialize(); //Setup Accelerometer/Magnetometer sensor
	
	unsigned char id  = MPL3115A2_ID();	//Verify chip id for MPL3115A2
	printf("MPL3115A2 Chip ID: 0x%02X . \r\n", id);
	id = AL_ChipID(); //Verify chip id for APDS9300
	printf("APDS9300 Chip ID: 0x%02X . \r\n", id);
	id = CAP1203_ReadID(); //Verify chip id for CAP1203
	printf("CAP1203 Chip ID: 0x%04X. \r\n",id);
	id = FXOS8700CQ_ID(); //Verify chip id for FXOS8700CQ
	printf("FXOS8700CQ Chip ID: 0x%02X. \r\n",id);
	
	//Setup APDS9300 - Ambient Light Sensor
	AL_SetGain(GAIN_16);
	AL_SetSamplingTime(S3);
	AL_Clear_Interrupt();
	
	//Setup FXOS8700CQ - Magnetometer/Accelerometer
	FXOS8700CQ_StandbyMode();
	FXOS8700CQ_WriteByte(SYSMOD,0x01);		//Set to wake up 
	FXOS8700CQ_ActiveMode();
	
	//Set LED pin as output
	LED_init();
	
	//Setup RTCC
	if(MCP79410_IsRunning())	//If clock is already running stop it 
	{
		MCP79410_DisableOscillator();
	}
	MCP79410_Initialize();	//Initialize RTCC with system time and date
	current_time = (RTCC_Struct *) malloc(sizeof(RTCC_Struct));

	sleep(2); // Wait 2 seconds or some sensors won't be ready
	printf("Sensors initialized.\n");
	return 0;
}

/**
 * @brief Polls the ambient light sensor for lux level and returns it as a c_float, needs to be converted in the Python version
 * @return float of the calculated current lux level
 */
float getAmbientLight(void)
{
	unsigned int channel1 = AL_ReadChannel(CH0);  //Get a light reading from the first channel
	unsigned int channel2 = AL_ReadChannel(CH1);  //Get a light reading from the second channel
	return AL_Lux(channel1,channel2);  //Return a c_float of the calculated lux level
}

float mpl_temperature = 0.0; /*!< Stores last Temperature polled from MPL3115A2 */
float mpl_altitude = 0.0; /*!< Stores last Altitude polled from MPL3115A2 */
float mpl_pressure = 0.0; /*!< Stores last Barometric Pressure polled from MPL3115A2 */


/**
 * @brief Polls the sensor for temperature, altitude and pressure sequentially and stores their values in their global buffers
 */
void pollMPL(void)
{
	mpl_temperature = MPL3115A2_ReadTemperature();
	MPL3115A2_StandbyMode();
	MPL3115A2_AltimeterMode();
	mpl_altitude = MPL3115A2_ReadAltitude();
	MPL3115A2_StandbyMode();
	MPL3115A2_BarometerMode();
	mpl_pressure = MPL3115A2_ReadBarometricPressure();
}

/**
 * @brief Gets the ambient temperature from when the last time the MPL sensor was polled
 * @return int of the ambient temperature from the last poll
 */
int getTemperature(void)
{
	return (int) mpl_temperature;
}

/**
 * @brief Gets the altitude from when the last time the MPL sensor was polled
 * @return int of the altitude from the last poll
 */
int getAltitude(void)
{
	return (int) mpl_altitude;
}

/**
 * @brief Gets the barometric pressure from when the last time the MPL sensor was polled
 * @return int of the barometric pressure from the last poll
 */
int getBarometricPressure(void)
{
	return (int) mpl_pressure;
}


rawdata_t magnetometerBuffer = {.x = 0, .y = 0, .z = 0}; /*!< Stores last polled Magnetometer data */
rawdata_t accelerometerBuffer = {.x = 0, .y = 0, .z = 0}; /*!< Stores last polled Accelerometer data */

/**
 * @brief Polls the accelerometer and magnetometer simultaneously and stores their values in the global buffer
 */
void pollFXOS(void)
{
	if(FXOS8700CQ_ReadStatusReg() & 0x80)
	{
		FXOS8700CQ_GetData(&accelerometerBuffer,&magnetometerBuffer);
	}
	else
	{
		magnetometerBuffer.x = 0;
		magnetometerBuffer.y = 0;
		magnetometerBuffer.z = 0;
		accelerometerBuffer.x = 0;
		accelerometerBuffer.y = 0;
		accelerometerBuffer.z = 0;
	}
}

/**
 * @brief Gets the X component of the pointing direction
 * @return int of the magnetic force in the X direction from the last poll
 */
int getMagX(void)
{
	return (int) magnetometerBuffer.x;
}

/**
 * @brief Gets the Y component of the pointing direction
 * @return int of the magnetic force in the Y direction from the last poll
 */
int getMagY(void)
{
	return (int) magnetometerBuffer.y;
}

/**
 * @brief Gets the Z component of the pointing direction
 * @return int of the magnetic force in the Z direction from the last poll
 */
int getMagZ(void)
{
	return (int) magnetometerBuffer.z;
}

/**
 * @brief Gets the X component of the acceleration
 * @return int of the force of acceleration in the X direction from the last poll
 */
int getAccelX(void)
{
	return (int) accelerometerBuffer.x;
}

/**
 * @brief Gets the Y component of the acceleration
 * @return int of the force of acceleration in the Y direction from the last poll
 */
int getAccelY(void)
{
	return (int) accelerometerBuffer.y;
}

/**
 * @brief Gets the Z component of the acceleration
 * @return int of the force of acceleration in the Z direction from the last poll
 */
int getAccelZ(void)
{
	return (int) accelerometerBuffer.z;
}

/**
 * @brief Polls data from the real time clock
 */
void poll_rtcc(void)
{
	current_time = MCP79410_GetTime();
}

/**
 * @brief Gets the year from the last time the RTCC was polled
 * @return int of the current year from the last poll
 */
int get_rtcc_year(void)
{
	return (int) current_time->year;
}

/**
 * @brief Gets the month from the last time the RTCC was polled
 * @return int of the current month from the last poll
 */
int get_rtcc_month(void)
{
	return (int) current_time->month;
}

/**
 * @brief Gets the date from the last time the RTCC was polled
 * @return int of the current day from the last poll
 */
int get_rtcc_date(void)
{
	return (int) current_time->date;
}

/**
 * @brief Gets the hour from the last time the RTCC was polled
 * @return int of the current hour from the last poll
 */
int get_rtcc_hour(void)
{
	return (int) current_time->hour;
}

/**
 * @brief Gets the minute from the last time the RTCC was polled
 * @return int of the current minute from the last poll
 */
int get_rtcc_minute(void)
{
	return (int) current_time->min;
}

/**
 * @brief Gets the second from the last time the RTCC was polled
 * @return int of the current second from the last poll
 */
int get_rtcc_second(void)
{
	return (int) current_time->sec;
}

/**
 * @brief Set the date/time on the RTCC.
 */
void set_rtcc_datetime(int year, int month, int date, int w_day, int hour, int minute, int second)
{
	RTCC_Struct input_time;
	input_time.year = (unsigned char) year;
	input_time.month = (unsigned char) month;
	input_time.date = (unsigned char) date;
	input_time.weekday = (unsigned char) w_day;
	input_time.hour = (unsigned char) hour;
	input_time.min = (unsigned char) minute;
	input_time.sec = (unsigned char) second;
	
	//If the clock is already running temporarily stop it
	if (MCP79410_IsRunning())
	{
		MCP79410_DisableOscillator();
	}
	//Load the new time
	MCP79410_SetTime(&input_time);
	
	//Restart the clock
	MCP79410_EnableOscillator();
}

/**
 * @brief Set an alarm on the RTCC. As of now, only 1 is supported.
 */
void set_rtcc_alarm(int year, int month, int date, int w_day, int hour, int minute, int second, int match_mode)
{
	RTCC_Struct input_time;
	input_time.year = (unsigned char) year;
	input_time.month = (unsigned char) month;
	input_time.date = (unsigned char) date;
	input_time.weekday = (unsigned char) w_day;
	input_time.hour = (unsigned char) hour;
	input_time.min = (unsigned char) minute;
	input_time.sec = (unsigned char) second;
	
	MCP79410_ClearInterruptFlag(RTCC_ZERO);
	MCP79410_SetAlarmTime(&input_time,RTCC_ZERO);
	//Allow different match modes
	switch (match_mode)
	{
		case 0:
			MCP79410_SetAlarmMatch(SECONDS_MATCH,RTCC_ZERO);
			break;
		case 1:
			MCP79410_SetAlarmMatch(MINUTES_MATCH,RTCC_ZERO);
			break;
		case 2:
			MCP79410_SetAlarmMatch(HOURS_MATCH,RTCC_ZERO);
			break;
		case 3:
			MCP79410_SetAlarmMatch(WEEKDAY_MATCH,RTCC_ZERO);
			break;
		case 4:
			MCP79410_SetAlarmMatch(DATE_MATCH,RTCC_ZERO);
			break;
		case 5:
			MCP79410_SetAlarmMatch(FULL_DATE_MATCH,RTCC_ZERO);
			break;
		default:
			MCP79410_SetAlarmMatch(MINUTES_MATCH,RTCC_ZERO);
			break;
	}
	MCP79410_SetAlarmMFPPolarity(LOWPOL,RTCC_ZERO);
	MCP79410_SetMFP_Functionality(ALARM_INTERRUPT);	 //Set alarm interrupt
}

/**
 * @brief Check whether or not the alarm is triggered.
 * @return an int boolean, 0 if not triggered
 */
int poll_rtcc_alarm(void)
{
	AlarmStatus_t s = MCP79410_GetAlarmStatus(RTCC_ZERO); //Check alarm status
	return (int) s;  //Returns 0 if not triggered
}

/**
 * @brief Turn off the alarm if it is triggered.
 */
void reset_alarm(void)
{
	AlarmStatus_t s = MCP79410_GetAlarmStatus(RTCC_ZERO); //Check alarm status
	
	if ((int) s != 0)  //If the alarm is triggered
	{
		MCP79410_ClearInterruptFlag(RTCC_ZERO);  //Alarms trigger an interrupt so clear that
		MCP79410_DisableAlarm(RTCC_ZERO);  //Turn off the alarm since it is confirmed to have triggered
	}
}

/**
 * @brief Turn on the orange LED on the Sensorian
 */
void orange_led_on(void)
{
	LED_on();  //Call the function to do so from TFT.c so it isn't called implicitly from the program
}

/**
 * @brief Turn off the orange LED on the Sensorian
 */
void orange_led_off(void)
{
	LED_off();  //Call the function to do so from TFT.c so it isn't called implicitly from the program
}

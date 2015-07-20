#include <bcm2835.h>
#include <stdio.h>
#include "MPL3115A2.h"
#include "APDS9300.h"
#include "CAP1203.h"
#include "FXOS8700CQ.h"
#include "MCP79410.h"
#include "led.h"
#include "i2c.h"
#include "Utilities.h"

RTCC_Struct *current_time;

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
	
	return 0;
}

int getAmbientLight(void)
{
	unsigned int channel1 = AL_ReadChannel(CH0);
	unsigned int channel2 = AL_ReadChannel(CH1);
	float lux = AL_Lux(channel1,channel2);
	//printf("Lux: %f.\r\n" ,lux);
	return (int) lux*100;
}

float mpl_temperature = 0.0; //Temperature recorded from MPL3115A2.
float mpl_altitude = 0.0; //Altitude recorded from MPL3115A2.
float mpl_pressure = 0.0; //Barometric pressure from MPL3115A2.

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

int getTemperature(void)
{
	//float temp = MPL3115A2_ReadTemperature();
	return (int) mpl_temperature;
}

int getAltitude(void)
{
	//MPL3115A2_StandbyMode();
	//MPL3115A2_AltimeterMode();
	//float alt = MPL3115A2_ReadAltitude();
	return (int) mpl_altitude;
}

int getBarometricPressure(void)
{
	//MPL3115A2_StandbyMode();
	//MPL3115A2_BarometerMode();
	//float press = MPL3115A2_ReadBarometricPressure();	//Take a pressure reading
	return (int) mpl_pressure;
}


rawdata_t magnetometerBuffer = {.x = 0, .y = 0, .z = 0}; //Magnetometer data
rawdata_t accelerometerBuffer = {.x = 0, .y = 0, .z = 0}; //Accelerometer data


void pollFXOS(void)
{
	if(FXOS8700CQ_ReadStatusReg() & 0x80)
	{
		// FXOS8700CQ_PollAccelerometer(&accelerometerBuffer);	
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
 * Gets the X component of the pointing direction
 */
int getMagX(void)
{
	return (int) magnetometerBuffer.x;
}

/**
 * Gets the Y component of the pointing direction
 */
int getMagY(void)
{
	return (int) magnetometerBuffer.y;
}

/**
 * Gets the Z component of the pointing direction
 */
int getMagZ(void)
{
	return (int) magnetometerBuffer.z;
}

/**
 * Gets the X component of the acceleration
 */
int getAccelX(void)
{
	return (int) accelerometerBuffer.x;
}

/**
 * Gets the Y component of the acceleration
 */
int getAccelY(void)
{
	return (int) accelerometerBuffer.y;
}

/**
 * Gets the Z component of the acceleration
 */
int getAccelZ(void)
{
	return (int) accelerometerBuffer.z;
}

//polls data from the realtime clock
void poll_rtcc(void)
{
	current_time = MCP79410_GetTime();
}

/**
 * Gets the year from the last time the RTCC was polled
 */
int get_rtcc_year(void)
{
	return (int) current_time->year;
}

/**
 * Gets the month from the last time the RTCC was polled
 */
int get_rtcc_month(void)
{
	return (int) current_time->month;
}

/**
 * Gets the date from the last time the RTCC was polled
 */
int get_rtcc_date(void)
{
	return (int) current_time->date;
}

/**
 * Gets the hour from the last time the RTCC was polled
 */
int get_rtcc_hour(void)
{
	return (int) current_time->hour;
}

/**
 * Gets the minute from the last time the RTCC was polled
 */
int get_rtcc_minute(void)
{
	return (int) current_time->min;
}

/**
 * Gets the second from the last time the RTCC was polled
 */
int get_rtcc_second(void)
{
	return (int) current_time->sec;
}

/**
 * Set the date/time on the RTCC.
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
 * Set an alarm on the RTCC. As of now, only 1 is supported.
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
	MCP79410_SetMFP_Functionality(ALARM_INTERRUPT);	 //Set alaram interrupt
}

/**
 * Returns whether or not the alarm is triggered.
 */
int poll_rtcc_alarm(void)
{
	//PinLevel_t pval;
	AlarmStatus_t s = MCP79410_GetAlarmStatus(RTCC_ZERO); //Check alarm status
	return (int) s;
}

/**
 * Turn off the alarm if it is triggered.
 */
void reset_alarm(void)
{
	AlarmStatus_t s = MCP79410_GetAlarmStatus(RTCC_ZERO); //Check alarm status
	
	if ((int) s != 0)
	{
		MCP79410_ClearInterruptFlag(RTCC_ZERO);
		MCP79410_DisableAlarm(RTCC_ZERO);
	}
}

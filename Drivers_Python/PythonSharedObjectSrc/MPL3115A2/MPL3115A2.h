/****************************************************************************
 * Copyright (C) 2015 Sensorian
 *                                                                          *
 * This file is part of Sensorian.                                          *
 *                                                                          *
 *   Sensorian is free software: you can redistribute it and/or modify it   *
 *   under the terms of the GNU Lesser General Public License as published  *
 *   by the Free Software Foundation, either version 3 of the License, or   *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   Sensorian is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU Lesser General Public License for more details.                    *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with Sensorian.                                          *
 *   If not, see <http://www.gnu.org/licenses/>.                            *
 ****************************************************************************/
 
/**
 * @file MPL3115A2/Example1/MPL3115A2.h
 * @author D.Qendri
 * @date 30 April 2015
 * @brief MPL3115A2 driver header.
 *
 */
 
#ifndef __MPL3115A2_H__
#define __MPL3115A2_H__

#include <stdint.h>

#define TRUE	1
#define FALSE	0

/*! \def MPL3115A2_ADDRESS
    \brief The I2C address of the MPL3115A2 barometer altimeter sensor
*/
#define MPL3115A2_ADDRESS   0x60 // 7-bit I2C address for sensor

#define STATUS              0x00
#define OUT_P_MSB           0x01        //Altitude data
#define OUT_P_CSB           0x02
#define OUT_P_LSB           0x03
#define OUT_T_MSB           0x04
#define OUT_T_LSB           0x05
#define DR_STATUS           0x06
#define OUT_P_DELTA_MSB     0x07
#define OUT_P_DELTA_CSB     0x08
#define OUT_P_DELTA_LSB     0x09
#define OUT_T_DELTA_MSB     0x0A
#define OUT_T_DELTA_LSB     0x0B
#define WHO_AM_I            0x0C
#define F_STATUS            0x0D
#define F_DATA              0x0E
#define F_SETUP             0x0F
#define TIME_DLY            0x10
#define SYSMOD              0x11
#define INT_SOURCE          0x12
#define PT_DATA_CFG         0x13
#define BAR_IN_MSB          0x14
#define BAR_IN_LSB          0x15
#define P_TGT_MSB           0x16
#define P_TGT_LSB           0x17
#define T_TGT               0x18
#define P_WND_MSB           0x19
#define P_WND_LSB           0x1A
#define T_WND               0x1B
#define P_MIN_MSB           0x1C
#define P_MIN_CSB           0x1D
#define P_MIN_LSB           0x1E
#define T_MIN_MSB           0x1F
#define T_MIN_LSB           0x20
#define P_MAX_MSB           0x21
#define P_MAX_CSB           0x22
#define P_MAX_LSB           0x23
#define T_MAX_MSB           0x24
#define T_MAX_LSB           0x25
#define CTRL_REG1           0x26
#define CTRL_REG2           0x27
#define CTRL_REG3           0x28
#define CTRL_REG4           0x29
#define CTRL_REG5           0x2A
#define OFF_P               0x2B
#define OFF_T               0x2C
#define OFF_H               0x2D

/**********************DR Status **************************************/
#define PTOW            0x80
#define POW             0x40       //Pressure overwrite
#define TOW             0x20
#define PTDR            0x08      //Pressure/Altitude OR Temperature data ready
#define PDR             0x04       //Pressure/Altitude new data available
#define TDR             0x02       //Temperature new Data Available.


/**********************Control Register 1****************************/
#define ALT     0x80
#define RAW     0x40
#define OS2     0x20
#define OS1     0x10
#define OS0     0x08
#define RST     0x04
#define OST     0x02
#define SBYB    0x01        //Active mode

// Oversample Ratio 1
#define OS_1    0x00                                     //  6 ms min between samples
#define OS_2    OS0                                      //  10 ms
#define OS_4    OS1                                      //  18 ms
#define OS_8    OS1 | OS0                                //  34 ms
#define OS_16   OS2                                      //  66 ms
#define OS_32   OS2 | OS0                                // 130 ms
#define OS_64   OS2 | OS1                                // 258 ms
#define OS_128  OS2 | OS1 | OS0                          // 512 ms

#define BAR_MASK       0x80
#define ALT_MASK       0xEF
#define ACTIVE_MASK    0xF1
#define STANDBY_MASK   0xFE

/**********************Control Register 2****************************/
#define ALARM_SEL     0x20  
#define LOAD_OUTPUT   0x10
#define ST3           0x08
#define ST2           0x04
#define ST1           0x02
#define ST0           0x01
#define CLEAR_ST      0xF0

/**********************Control Register 3****************************/
#define IPOL1   0x10           //Set to 1 for Active HIGH on INT1
#define PP_OD1  0x08           //Set to 0 for internal pull up
#define IPOL2   0x02           //1 Active high, 0 Active Low Interrupt on Pad2
#define PP_OD2  0x01           //Set to 0 for internal pull up

/**********************Control Register 4****************************/
#define INT_EN_DRDY 0x80       //Set to 1: Data Ready interrupt enabled
#define INT_EN_FIFO 0x40       //Set to 1: FIFO interrupt enabled
#define INT_EN_PW   0x20       //Set to 1: Pressure window interrupt enabled
#define INT_EN_TW   0x10       //Set to 1: Temperature window interrupt enabled
#define INT_EN_PTH  0x08       //Set to 1: Pressure Threshold interrupt enabled
#define INT_EN_TTH  0x04       //Set to 1: Temperature Threshold interrupt enabled
#define INT_EN_PCHG 0x02       //Set to 1: Pressure Change interrupt enabled.
#define INT_EN_TCHG 0x01       //Set to 1: Temperature Change interrupt enabled
#define INT_EN_CLEAR  0x00

/**********************Control Register 5****************************/

#define INT_CFG_DRDY    0x80       //1: Interrupt is routed to INT1
#define INT_CFG_FIFO    0x40
#define INT_CFG_PW      0x20
#define INT_CFG_TW      0x10
#define INT_CFG_PTH     0x08
#define INT_CFG_TTH     0x04
#define INT_CFG_PCHG    0x02
#define INT_CFG_TCHG    0x01

#define INT_CFG_CLEAR   0x00

#define INT2            0
#define INT1            1
/**************************************************************************/

#define DISABLED    0x00
#define CIRCULAR    0x40
#define FULL_STOP   0x80
#define F_MODE      DISABLED
/******************PT_DATA_CFG - Sensor data event flag register***********/
#define DREM     	0x04                  // Data Ready Event Mode
#define PDEFE    	0x02                  // Pressure Data Event Flag Enabled
#define TDEFE    	0x01                  // Temperature Data Event Flag Enabled

/*************************** INT_SOURCE Register*****************************/

#define SRC_DRDY     0x80
#define SRC_FIFO     0x40
#define SRC_PW       0x20
#define SRC_TW       0x10
#define SRC_PTH      0x08
#define SRC_TTH      0x04
#define SRC_PCHG     0x02
#define SRC_TCHG     0x01

/*******************************************************************************/

typedef enum { PSI,
               INHG,
			   MMHG} unitsType;
			   
typedef enum { SR1 = OS_1, 
               SR2 = OS_2, 
			   SR3 = OS_4, 
			   SR4 = OS_8, 
			   SR5 = OS_16,
			   SR6 = OS_32, 
			   SR7 = OS_64, 
			   SR8 = OS_128} OverSample_t;

void            MPL3115A2_Initialize(void);
unsigned char   MPL3115A2_ID(void);
unsigned char   MPL3115A2_GetMode(void);
void            MPL3115A2_StandbyMode(void);                        // Puts the sensor into Standby mode. Required when changing CTRL1 register.
void            MPL3115A2_ActiveMode(void);                         // Start taking measurements!

void            MPL3115A2_AltimeterMode(void);                      // Puts the sensor into altimetery mode.
float           MPL3115A2_ReadAltitude(void);                       // Returns float with meters above sealevel. Ex: 1638.94
void            MPL3115A2_SetAltimeterOffset(unsigned char H_Offset);

void            MPL3115A2_BarometerMode(void);                      // Puts the sensor into Pascal measurement mode.
void            MPL3115A2_SetPressureOffset(unsigned char P_Offset);
float           MPL3115A2_GetMinimumPressure(void);
float           MPL3115A2_GetMaximumPressure(void);
unsigned int    MPL3115A2_ReadBarometicPressureInput(void);
float           MPL3115A2_ReadBarometricPressure(void);             // Returns float with barometric pressure in Pa
float           MPL3115A2_ReadPressure(unitsType units);
void            MPL3115A2_SetPressureAlarmThreshold(unsigned int thresh);
void            MPL3115A2_SetPressureTargetWindow(unsigned int target,unsigned int window);

float           MPL3115A2_ReadTemperature(void);                    // Returns float with current temperature in Celsius
float           MPL3115A2_GetMinimumTemperature(void);
float           MPL3115A2_GetMaximumTemperature(void);
void            MPL3115A2_SetTempTargetWindow(unsigned int target,unsigned int window);
void            MPL3115A2_SetTemperatureThreshold(unsigned char thresh);
void            MPL3115A2_SetTempOffset(char T_Offset);

void            MPL3115A2_OutputSampleRate(unsigned char);        // Sets the # of samples from 1 to 128. See datasheet.
void            MPL3115A2_SetAcquisitionTimeStep(unsigned char); // Sets the # of time steps
void            MPL3115A2_EnableEventFlags(void);                   // Sets the fundamental event flags. Required during setup.
void            MPL3115A2_ToggleOneShot(void);

void            MPL3115A2_ClearInterrupts(void);
void            MPL3115A2_ConfigureInterruptPin(unsigned char intrrpt,unsigned char pin);
void            MPL3115A2_ConfigurePressureInterrupt(void);
void            MPL3115A2_ConfigureAltitudeInterrupt(void);

unsigned char   MPL3115A2_ReadByte(unsigned char address,char reg);
void 			MPL3115A2_ReadByteArray(unsigned char address,char reg,char *buffer, unsigned int length);
void            MPL3115A2_WriteByte(unsigned char address,char reg, char value);
void 			MPL3115A2_WriteByteArray(unsigned char address,char reg, char* buffer, unsigned int length);

#endif

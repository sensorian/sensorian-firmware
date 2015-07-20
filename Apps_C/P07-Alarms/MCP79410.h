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
 * @file MCP79410_RTCC/Example1/MCP79410.h
 * @author D.Qendri
 * @date 30 Dec 2014
 * @brief MCP79410 driver header.
 */
 
#ifndef __MCP79410_H__
#define __MCP79410_H__

/**
 * @file   MCP79410.h
 * 
 * D.Qendri 
 * @date   Dec, 2014
 * @brief  RTCC API.
 */
 
#include "bcm2835.h"

/*! \def MCP79410_ADDRESS
    \brief The I2C address of the MCP79410 real time clock and calendar
*/

#define  MCP79410_ADDRESS	0x6f

/************************* RTCC Memory map ****************************/

#define  EEPROM_WRITE 0xae       //  DEVICE ADDR for EEPROM (writes)   
#define  EEPROM_READ  0xaf       //  DEVICE ADDR for EEPROM (reads)  
#define  RTCC_WRITE   0xde       //  DEVICE ADDR for RTCC MCHP  (writes) 
#define  RTCC_READ    0xdf       //  DEVICE ADDR for RTCC MCHP  (reads)

#define  SRAM_PTR     0x20       //  pointer of the SRAM area (RTCC) 
#define  EEPROM_SR    0xff       //  STATUS REGISTER in the  EEPROM

#define  SEC          0x00       //  address of SECONDS      register 
#define  MIN          0x01       //  address of MINUTES      register 
#define  HOUR         0x02       //  address of HOURS        register 
#define  DAY          0x03       //  address of DAY OF WK    register 
#define  STAT         0x03       //  address of STATUS       register 
#define  DATE         0x04       //  address of DATE         register  
#define  MNTH         0x05       //  address of MONTH        register 
#define  YEAR         0x06       //  address of YEAR         register 
#define  CTRL         0x07       //  address of CONTROL      register 
#define  CAL          0x08       //  address of CALIB        register 
#define  ULID         0x09       //  address of UNLOCK ID    register

#define  ALM0SEC      0x0a       //  address of ALARM0 SEC   register 
#define  ALM0MIN      0x0b       //  address of ALARM0 MIN   register 
#define  ALM0HR       0x0c       //  address of ALARM0 HOUR  register 
#define  ALM0WDAY     0x0d       //  address of ALARM0 CONTR register
#define  ALM0DATE     0x0e       //  address of ALARM0 DATE  register 
#define  ALM0MTH      0x0f       //  address of ALARM0 MONTH register

#define  ALM1SEC      0x11       //  address of ALARM1 SEC   register 
#define  ALM1MIN      0x12       //  address of ALARM1 MIN   register 
#define  ALM1HR       0x13       //  address of ALARM1 HOUR  register 
#define  ALM1WDAY     0x14       //  address of ALARM1 CONTR register
#define  ALM1DATE     0x15       //  address of ALARM1 DATE  register 
#define  ALM1MTH      0x16       //  address of ALARM1 MONTH register

#define  PWRDNMIN     0x18       //  address of T_SAVER MIN(VDD->BAT)
#define  PWRDNHOUR    0x19       //  address of T_SAVER HR (VDD->BAT) 
#define  PWRDNDATE    0x1a       //  address of T_SAVER DAT(VDD->BAT) 
#define  PWRDNMTH     0x1b       //  address of T_SAVER MTH(VDD->BAT)

#define  PWRUPMIN     0x1c       //  address of T_SAVER MIN(BAT->VDD)
#define  PWRUPHOUR    0x1d       //  address of T_SAVER HR (BAT->VDD) 
#define  PWRUPDATE    0x1e       //  address of T_SAVER DAT(BAT->VDD) 
#define  PWRUPMTH     0x1f       //  address of T_SAVER MTH(BAT->VDD)

/************************GLOBAL CONSTANTS RTCC - INITIALIZATION****************/

#define  PM                0x20       //  post-meridian bit (HOUR)
#define  HOUR_FORMAT       0x40       //  Hour format
#define  OUT_PIN           0x80       //  = b7 (CTRL)
#define  SQWEN             0x40       //  SQWE = b6 (CTRL)
#define  ALM_NO            0x00       //  no alarm activated        (CTRL)
#define  ALM_0             0x10       //  ALARM0 is       activated (CTRL)
#define  ALM_1             0x20       //  ALARM1 is       activated (CTRL)
#define  ALM_01            0x30       //  both alarms are activated (CTRL)
#define  MFP_01H           0x00       //  MFP = SQVAW(01 HERZ)      (CTRL)
#define  MFP_04K           0x01       //  MFP = SQVAW(04 KHZ)       (CTRL)
#define  MFP_08K           0x02       //  MFP = SQVAW(08 KHZ)       (CTRL)
#define  MFP_32K           0x03       //  MFP = SQVAW(32 KHZ)       (CTRL)
#define  MFP_64H           0x04       //  MFP = SQVAW(64 HERZ)      (CTRL)
#define  ALMx_POL          0x80       //  polarity of MFP on alarm  (ALMxCTL)
#define  ALMxC_SEC         0x00       //  ALARM compare on SEC      (ALMxCTL)
#define  ALMxC_MIN         0x10       //  ALARM compare on MIN      (ALMxCTL)
#define  ALMxC_HR          0x20       //  ALARM compare on HOUR     (ALMxCTL)
#define  ALMxC_DAY         0x30       //  ALARM compare on DAY      (ALMxCTL)
#define  ALMxC_DAT         0x40       //  ALARM compare on DATE     (ALMxCTL)
#define  ALMxC_ALL         0x70       //  ALARM compare on all param(ALMxCTL)
#define  ALMx_IF           0x08       //  MASK of the ALARM_IF      (ALMxCTL)

#define  OSCRUN            0x20       //  state of the oscillator(running or not)
#define  PWRFAIL           0x10
#define  VBATEN            0x08       //  enable battery for back-up
#define  VBAT_DIS          0x37       //  disable battery back-up

#define  START_32KHZ       0x80       //  start crystal: ST = b7 (SEC)
#define  LP                0x20       //  mask for the leap year bit(MONTH REG)
#define  HOUR_12           0x40       //  12 hours format   (HOUR)

#define  LPYR              0x20


/********************************************************************************/
#define ALM1MSK2           0x40
#define ALM1MSK1           0x20
#define ALM1MSK0           0x10

#define ALM0MSK2           0x40
#define ALM0MSK1           0x20
#define ALM0MSK0           0x10

/*********************************************************************************/

/**
 *  MCP79410 RTCC time structure type 
 */ 
typedef struct _RTCC_Struct
{
	unsigned char sec;		/// Seconds
	unsigned char min;		/// Minutes 
	unsigned char hour;		/// Hours, (Format used : Standard / Military) 
	unsigned char weekday;	/// Weekday 
	unsigned char date;		/// Date of the month 
	unsigned char month;	/// Month 
	unsigned char year;		///Year 
} RTCC_Struct;

/** Alarm
 *  Alarm Type. 
 */
typedef enum Alarm {ZERO = 0, ONE} Alarm_t;

/** Alarm Status
 *  Alarm Status Type . 
 */
typedef enum AlarmStatus {NOT_SET = 0, SET} AlarmStatus_t;

/** Alarm Status
 *  Alarm Status Type . 
 */
typedef enum PMAM {AMT = 0, PMT} PMAM_t;

/** PMAM 
 *  PMAM Type . 
 */
typedef enum Format {H24 = 0, H12} Format_t;

/** Hour Format 
 *  Format Type . 
 */
typedef enum Match {SECONDS_MATCH = 0, MINUTES_MATCH, HOURS_MATCH, WEEKDAY_MATCH, DATE_MATCH, FULL_DATE_MATCH } Match_t;

/** Alarm Match 
 *  Alarm Match Type . 
 */
typedef enum MFP_MODE {GPO = 0, ALARM_INTERRUPT, SQUARE_WAVE} MFP_t;

/** MFP_MODE 
 *  MFP_MODE Type . 
 */
typedef enum MFP_POL {LOWPOL = 0, HIGHPOL} Polarity_t;


#define TRUE	1
#define FALSE	0

/***************************Function definitions********************************************/

void            MCP79410_Initialize(void);
void 			MCP79410_EnableOscillator(void);
void 			MCP79410_DisableOscillator(void);
unsigned char 	MCP79410_IsRunning(void);

RTCC_Struct* 	MCP79410_GetTime(void);
void 			MCP79410_SetTime(RTCC_Struct *time);
void 			MCP79410_SetHourFormat(Format_t format);
void 			MCP79410_SetPMAM(PMAM_t meridian);

void			MCP79410_EnableAlarm(Alarm_t alarm);
void			MCP79410_DisableAlarm(Alarm_t alarm);
AlarmStatus_t	MCP79410_GetAlarmStatus(Alarm_t alarm);
void 			MCP79410_ClearInterruptFlag(Alarm_t alarm);
void 			MCP79410_SetAlarmTime(RTCC_Struct *time, Alarm_t alarm);
void 			MCP79410_SetAlarmMFPPolarity(Polarity_t MFP_pol,Alarm_t alarm);
void 			MCP79410_SetAlarmMatch(Match_t match,Alarm_t alarm);
void 			MCP79410_SetMFP_Functionality(MFP_t mode);
void 			MCP79410_SetMFP_GPOStatus(Polarity_t status);

unsigned char	MCP79410_CheckPowerFailure(void);
unsigned char 	MCP79410_IsVbatEnabled(void);
void 			MCP79410_EnableVbat(void);
void 			MCP79410_DisableVbat(void);
RTCC_Struct* 	MCP79410_GetPowerUpTime(void);
RTCC_Struct* 	MCP79410_GetPowerDownTime(void);

unsigned char 	MCP79410_dec2bcd(unsigned char num);
unsigned char 	MCP79410_bcd2dec(unsigned char num);

void            MCP79410_Write(unsigned char rtcc_reg, unsigned char data); 
unsigned char   MCP79410_Read(unsigned char rtcc_reg); 

#endif


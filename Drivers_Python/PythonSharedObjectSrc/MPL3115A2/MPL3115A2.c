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
 * @file MPL3115A2/Example1/MPL3115A2.c
 * @author D.Qendri
 * @date 30 April 2015
 * @brief MPL3115A2 driver.
 *
 */
 
#include "MPL3115A2.h"
#include "i2c.h"

/// \defgroup barometer Barometer, Altimeter, Temperature sensor
/// These functions let you communicate with the MPL3115A2 Barometer/Altimeter/Temperature sensor
/// @{

/**
 * @brief Initialize the sensor and enable interrupts
 * @return none
 */
void MPL3115A2_Initialize(void)
{	
	MPL3115A2_StandbyMode();
	MPL3115A2_WriteByte(MPL3115A2_ADDRESS,PT_DATA_CFG, DREM | PDEFE | TDEFE);		//Enable data ready flags for pressure and temperature )	
	MPL3115A2_WriteByte(MPL3115A2_ADDRESS,CTRL_REG1, OS_128 | SBYB);				//Set sensor to active state with oversampling ratio 128 (512 ms between samples)	
	
}

/**
 * @brief Returns the Factory Chip ID
 * @return ID Chip ID as byte
 */
unsigned char MPL3115A2_ID(void)
{
	unsigned char id = MPL3115A2_ReadByte(MPL3115A2_ADDRESS,WHO_AM_I);
	return id;
}

/**
 * @brief Return a bool value indicating wheather the sensor is in Active or Standby mode
 * @return status True if in Active mode, False otherwise
 */
unsigned char MPL3115A2_GetMode(void)
{
    unsigned char status = MPL3115A2_ReadByte(MPL3115A2_ADDRESS,SYSMOD);
    return (status & 0x01) ? 1 : 0;
}

/**
 * @brief Puts the sensor in standby mode, the user must do this in order to modify the major control registers
 * @return none
 */
void MPL3115A2_StandbyMode(void)
{
  unsigned char ctrl_reg = MPL3115A2_ReadByte(MPL3115A2_ADDRESS,CTRL_REG1); //Read current settings
  ctrl_reg &= ~SBYB;                                      //Clear SBYB bit for Standby mode
  MPL3115A2_WriteByte(MPL3115A2_ADDRESS,CTRL_REG1, ctrl_reg);               //Put device in Standby mode
}

/**
 * @brief Puts the sensor in active mode, needed is the sensor is in standby mode.
 * @return none
 */
void  MPL3115A2_ActiveMode(void)
{
  unsigned char tempSetting = MPL3115A2_ReadByte(MPL3115A2_ADDRESS,CTRL_REG1);    //Read current settings
  tempSetting |= SBYB;                                    //Set SBYB bit for Active mode
  MPL3115A2_WriteByte(MPL3115A2_ADDRESS,CTRL_REG1, tempSetting);
}

/**
 * @brief Sets the mode to Altimeter
 * @return none
 */
void MPL3115A2_AltimeterMode(void)
{
  unsigned char ctrl_reg = MPL3115A2_ReadByte(MPL3115A2_ADDRESS,CTRL_REG1); //Read current settings
  ctrl_reg &= ~(SBYB);                               		//Go to Standby mode
  MPL3115A2_WriteByte(MPL3115A2_ADDRESS,CTRL_REG1, ctrl_reg);

  ctrl_reg = ALT|OS_128;                    //Set ALT bit to one and enable back Active mode
  MPL3115A2_WriteByte(MPL3115A2_ADDRESS,CTRL_REG1, ctrl_reg);
}

/**
 * @brief Returns the number of meters above sea level,Returns -1 if no new data is available
 * @return altitude Altitude from sea-level in meters
 */
float MPL3115A2_ReadAltitude(void)
{
    char altpbyte[3] = {0x00};
    MPL3115A2_ReadByteArray(MPL3115A2_ADDRESS,OUT_P_MSB,altpbyte,3);       //Read altitude data  
    int m_altitude = altpbyte[0];
    int c_altitude = altpbyte[1];
	int l_altitude = altpbyte[2];
	uint32_t al = (m_altitude << 24)|(c_altitude < 16)| (l_altitude);
	return (float)(al/65536);
}

/**
 * @brief Set the altimeter offset correction.
 * @param H_Offset Altitude in meters ,range varies from -128 to +127 meters
 * @return none
 */
void MPL3115A2_SetAltimeterOffset(unsigned char H_Offset)
{	
	if((H_Offset > -128) || (H_Offset < 128))
	{
		MPL3115A2_WriteByte(MPL3115A2_ADDRESS,OFF_H,H_Offset);
	}
}

/**
 * @brief Configure the sensor in Barometer mode
 * @return none
 */
void MPL3115A2_BarometerMode(void)
{
  unsigned char ctrl_reg = MPL3115A2_ReadByte(MPL3115A2_ADDRESS,CTRL_REG1); //Read current settings
  ctrl_reg &= ~(SBYB);                               //Set SBYB to 0 and go to Standby mode
  MPL3115A2_WriteByte(MPL3115A2_ADDRESS,CTRL_REG1, ctrl_reg);

  ctrl_reg = OS_128 ;                     //Set ALT bit to zero and enable back Active mode
  MPL3115A2_WriteByte(MPL3115A2_ADDRESS,CTRL_REG1, ctrl_reg);
}

/**
 * @brief Set the pressure offset correction.
 * @param P_Offset Offset varies from -128 to +128 Pa, 4 Pa per LSB
 * @return none
 */
void MPL3115A2_SetPressureOffset(unsigned char P_Offset)
{
	if((P_Offset > -128) || (P_Offset < 128))
	{
		MPL3115A2_WriteByte(MPL3115A2_ADDRESS,OFF_P,P_Offset);
	}
}

/**
 * @brief Get minimum stored pressure.
 * @return pressure Minimum stored pressure during active measurement mode
 */
float MPL3115A2_GetMinimumPressure(void)
{
	char minPressure[3] = {0x00};	
	MPL3115A2_ReadByteArray(MPL3115A2_ADDRESS,P_MIN_MSB,minPressure,3);
	
	unsigned char m_altitude = minPressure[0];
	unsigned char c_altitude = minPressure[1];	
	float l_altitude = (float)(minPressure[2]>>4)/4; 			//dividing by 4, since two lowest bits are fractional value
	return((float)(m_altitude<<10 | c_altitude<<2)+l_altitude); //shifting 2 to the left to make room for LSB
	
}

/**
 * @brief Get maximum stored pressure.
 * @return pressure Maximum stored pressure during active measurement mode
 */
float MPL3115A2_GetMaximumPressure(void)
{
	char maxPressure[3] = {0x00};	
	MPL3115A2_ReadByteArray(MPL3115A2_ADDRESS,P_MAX_MSB,maxPressure,3);
	
	unsigned char m_altitude = maxPressure[0];
	unsigned char c_altitude = maxPressure[1];
	float l_altitude = (float)(maxPressure[2]>>4)/4; 			//dividing by 4, since two lowest bits are fractional value
	return((float)(m_altitude<<10 | c_altitude<<2)+l_altitude); //shifting 2 to the left to make room for LSB
}

/**
 * @brief Returns barometric pressure input used for calculating altitude
 * @return pressure  Equivalent sea level pressure
 */
unsigned int MPL3115A2_ReadBarometicPressureInput(void)
{
	unsigned char barMSB =  MPL3115A2_ReadByte(MPL3115A2_ADDRESS,BAR_IN_MSB);
	unsigned char barLSB =  MPL3115A2_ReadByte(MPL3115A2_ADDRESS,BAR_IN_LSB);
	
	return ((barMSB << 8) | barLSB);
}

/**
 * @brief Reads the current pressure in Pa
 * @return Pressure Pressure in Pa
 */
float MPL3115A2_ReadBarometricPressure(void)
{
	unsigned char status = MPL3115A2_ReadByte(MPL3115A2_ADDRESS,STATUS);
	char pbyte[3] = {0x00};
	
    //Check PDR bit, if it's not set then toggle OST
    if((status & (1<< PDR)) == 0) 
	{
		MPL3115A2_ToggleOneShot(); //Toggle the OST bit causing the sensor to immediately take another reading
	}
	
	MPL3115A2_ReadByteArray(MPL3115A2_ADDRESS,OUT_P_MSB,pbyte,3);
	
  //this function takes values from the read buffer and converts them to pressure units
  unsigned long m_altitude = pbyte[0];
  unsigned long c_altitude = pbyte[1];
  float l_altitude = (float)(pbyte[2]>>4)/4; //dividing by 4, since two lowest bits are fractional value
  return((float)(m_altitude<<10 | c_altitude<<2)+l_altitude); //shifting 2 to the left to make room for LSB
}

/**
 * @brief  Get a barometric pressure reading 
 * @param units One of the unitsType
 * @return Pressure Pressure in units
 */
float MPL3115A2_ReadPressure(unitsType units)
{
	float pressure = MPL3115A2_ReadBarometricPressure();
	
    switch (units)
    {
        case PSI:
            return pressure * 0.000145037738;    
        case INHG:
            return pressure * 0.00029529983071;
        case MMHG:
            return pressure * 0.007500615613;
    }    
    return pressure;
}

/**
 * @brief Configure alarm threshold 
 * @param thresh reister to read value form
 * @return none
 */
void MPL3115A2_SetPressureAlarmThreshold(unsigned int thresh)
{
   unsigned char threshMSB = (unsigned char) (thresh << 8);
   unsigned char threshLSB = (unsigned char) thresh;   
   MPL3115A2_WriteByte(MPL3115A2_ADDRESS,P_TGT_MSB,threshMSB);
   MPL3115A2_WriteByte(MPL3115A2_ADDRESS,P_TGT_LSB,threshLSB);
}

/**
 * @brief Configure alarm target and window 
 * @param target Contents of target register.
 * @param window Contents of window register that activates the interrupt
 * @return none
 */
void MPL3115A2_SetPressureTargetWindow(unsigned int target,unsigned int window)
{
   unsigned char tmpMSB = (unsigned char) (window << 8);
   unsigned char tmpLSB = (unsigned char) (window);
   MPL3115A2_WriteByte(MPL3115A2_ADDRESS,P_WND_MSB,tmpMSB);
   MPL3115A2_WriteByte(MPL3115A2_ADDRESS,P_WND_LSB,tmpLSB);
   
   tmpMSB = (unsigned char) (target << 8);
   tmpLSB = (unsigned char) (target);
   MPL3115A2_WriteByte(MPL3115A2_ADDRESS,P_WND_MSB,tmpMSB);
   MPL3115A2_WriteByte(MPL3115A2_ADDRESS,P_WND_LSB,tmpLSB);    
}

/**
 * @brief  Get a temperature reading from the sensor
 * @return Temperature as a floating point number
 */
float MPL3115A2_ReadTemperature(void)
{
	char temperature[2] = {0x00};	
	MPL3115A2_ReadByteArray(MPL3115A2_ADDRESS,OUT_T_MSB,temperature,2);
	
	float templsb = (temperature[1]>>4) / 16.0; 			//temp, fraction of a degree
    float unpacked = (float)(temperature[0] + templsb);
	
	return unpacked;
}

/**
 * @brief  Get minimum recorded temperature reading
 * @return temp Maximum temperature as a floating point number
 */
float MPL3115A2_GetMinimumTemperature(void)
{
	char temperature[2] = {0x00};	
	MPL3115A2_ReadByteArray(MPL3115A2_ADDRESS,T_MIN_MSB,temperature,2);
	
	float templsb = (temperature[1]>>4) / 16.0; 			//temp, fraction of a degree
    float minTemp = (float)(temperature[0] + templsb);
	
	return minTemp;
}

/**
 * @brief  Get maximum recorded temperature reading
 * @return temp Maximum temperature as a floating point number
 */
float MPL3115A2_GetMaximumTemperature(void)
{
	char temperature[2] = {0x00};	
	MPL3115A2_ReadByteArray(MPL3115A2_ADDRESS,T_MAX_MSB,temperature,2);
	
	float templsb = (temperature[1]>>4) / 16.0; 			//temp, fraction of a degree
    float maxTemp = (float)(temperature[0] + templsb);
	
	return maxTemp;
}

/**
 * @brief Configures the temperature target and window for the interrupts.
 * @param target The target temeprature
 * @param window Temperature window around the target.
 * @return 
 */
void MPL3115A2_SetTempTargetWindow(unsigned int target,unsigned int window)
{
	MPL3115A2_WriteByte(MPL3115A2_ADDRESS,T_TGT,target);
	MPL3115A2_WriteByte(MPL3115A2_ADDRESS,T_WND,window);
}

/**
 * @brief Configure the temeperature threshold.
 * @param thresh Temperature threshold.
 * @return none
 */
void MPL3115A2_SetTemperatureThreshold(unsigned char thresh)
{
	MPL3115A2_WriteByte(MPL3115A2_ADDRESS,CTRL_REG4,INT_EN_TTH);		//enable temperature interrupt
	MPL3115A2_WriteByte(MPL3115A2_ADDRESS,CTRL_REG5,INT_EN_TTH);		//map to interrupt
}

/**
 * @brief Used to correct the measured sensor temeperature by a particular offset.
 * @param T_Offset The temperature offset as a two's complement number.
 * @return none
 */
void MPL3115A2_SetTempOffset(char T_Offset)
{
	MPL3115A2_WriteByte(MPL3115A2_ADDRESS,OFF_T,T_Offset);
}

/**
 * @brief Configures the output sample rate.The higher the oversample rate the greater the time between data samples.
 * @param sampleRate	Constant that determines the sample rate
 * @return none
 */
void MPL3115A2_OutputSampleRate(unsigned char sampleRate)
{
  unsigned char ctrl_reg = MPL3115A2_ReadByte(MPL3115A2_ADDRESS,CTRL_REG1); //Read current settings
  ctrl_reg &= ~(1 << SBYB);                               
  MPL3115A2_WriteByte(MPL3115A2_ADDRESS,CTRL_REG1, ctrl_reg);               //Put sensor in Standby mode

  if(sampleRate > 7) sampleRate = 7;                      //OS cannot be larger than 0b.0111
  sampleRate <<= 3;                                       //Align it for the CTRL_REG1 register

  ctrl_reg = MPL3115A2_ReadByte(MPL3115A2_ADDRESS,CTRL_REG1);               //Read current settings
  ctrl_reg |= sampleRate;                                 //Mask in new Output Sample bits
  MPL3115A2_WriteByte(MPL3115A2_ADDRESS,CTRL_REG1, ctrl_reg);               //Update sample rate settings
}

/**
 * @brief Set the acquisition time step in seconds. 
 * @param ST_Value timeStep is set from 0x1 - 0xF corresponding steps 1 - 32,768 seconds.
 * @return none
 */
void MPL3115A2_SetAcquisitionTimeStep(unsigned char ST_Value)
{
  unsigned char ctrl_reg = MPL3115A2_ReadByte(MPL3115A2_ADDRESS,CTRL_REG1); //Read current settings
  ctrl_reg &= ~(1 << SBYB);                               
  MPL3115A2_WriteByte(MPL3115A2_ADDRESS,CTRL_REG1, ctrl_reg);               //Put sensor in Standby mode

  if (ST_Value <= 0xF) 
  {
       ctrl_reg = MPL3115A2_ReadByte(MPL3115A2_ADDRESS,CTRL_REG2);          //Read current settings
       MPL3115A2_WriteByte(MPL3115A2_ADDRESS,CTRL_REG1, ctrl_reg|ST_Value);
  }
}

/**
 * @brief Enables the pressure and temp measurement event flags so that we can test against them. 
 *	      This is recommended during setup.
 * @return none
 */
void MPL3115A2_EnableEventFlags(void)
{
  MPL3115A2_WriteByte(MPL3115A2_ADDRESS,PT_DATA_CFG, 0x07); 	// Enable all three pressure and temp event flags
}

/**
 * @brief Causes the sensor to immediately take another reading , needed to sample faster than 1Hz
 * @return none
 */
void MPL3115A2_ToggleOneShot(void)
{
  unsigned char tempSetting = MPL3115A2_ReadByte(MPL3115A2_ADDRESS,CTRL_REG1); //Read current settings
  tempSetting &= ~(1<<1);                                   //Clear OST bit
  MPL3115A2_WriteByte(MPL3115A2_ADDRESS,CTRL_REG1, tempSetting);
  tempSetting = MPL3115A2_ReadByte(MPL3115A2_ADDRESS,CTRL_REG1);              //Read current settings to be safe
  tempSetting |= (1<<1);                                    //Set OST bit
  MPL3115A2_WriteByte(MPL3115A2_ADDRESS,CTRL_REG1, tempSetting);
}

/**
 * @brief Configure Interrupt pins
 * @param intrrpt Register to read value form
 * @param pin One of the two pins.
 * @return none
 */
void MPL3115A2_ConfigureInterruptPin(unsigned char intrrpt,unsigned char pin)
{
    MPL3115A2_WriteByte(MPL3115A2_ADDRESS,CTRL_REG5,(pin << intrrpt));
}

/**
 * @brief Configure Pressure Interrupt 
 * @return none
 */
void MPL3115A2_ConfigurePressureInterrupt(void)
{
   MPL3115A2_WriteByte(MPL3115A2_ADDRESS,CTRL_REG3,PP_OD1|PP_OD2);          //Configure Interrupt pins for open drain active low
}

/**
 * @brief Configure altitude interrupt pin
 * @return none
 */
void MPL3115A2_ConfigureAltitudeInterrupt(void)
{
   MPL3115A2_WriteByte(MPL3115A2_ADDRESS,CTRL_REG3,PP_OD1|PP_OD2); 	//Configure Interrupt pins for open drain active low
}

/**
 * @brief Clear any existing interrupts
 * @return none
 */
void MPL3115A2_ClearInterrupts(void)
{
   MPL3115A2_ReadByte(MPL3115A2_ADDRESS,OUT_P_MSB); 
   MPL3115A2_ReadByte(MPL3115A2_ADDRESS,OUT_P_CSB);          //Configure Interrupt pins for open drain active low
   MPL3115A2_ReadByte(MPL3115A2_ADDRESS,OUT_P_LSB); 
   MPL3115A2_ReadByte(MPL3115A2_ADDRESS,OUT_T_MSB);
   MPL3115A2_ReadByte(MPL3115A2_ADDRESS,OUT_T_LSB);
   MPL3115A2_ReadByte(MPL3115A2_ADDRESS,F_STATUS);
}

/// @}

/**
 * @brief Read byte value from register
 * @param reg Register to read value form
 * @return value Register value
 */
unsigned char MPL3115A2_ReadByte(unsigned char address,char reg)
{
    return I2C_ReadByteRegister(address,reg);		//Read register current value
}

/**
 * @brief Read byte value from register
 * @param reg Register to read value form
 * @param *buffer Array buffer to be written
 * @param length Length of buffer array
 * @return value Register value
 */
void MPL3115A2_ReadByteArray(unsigned char address,char reg,char *buffer, unsigned int length)
{    
	I2C_ReadByteArray(address,reg,buffer, length);
}

/**
 * @brief Write a single byte to the register
 * @param reg register address
 * @param value value to write onto register
 * @return none
 */
void MPL3115A2_WriteByte(unsigned char address,char reg, char value)
{
	I2C_WriteByteRegister(address,reg,value);			//Write value to register
}

/**
 * @brief Writes an array of bytes to the sensor
 * @param reg Register to read value form
 * @param *buffer Array buffer to be written
 * @param length Length of buffer array
 * @return none
 */
void MPL3115A2_WriteByteArray(unsigned char address,char reg, char* buffer, unsigned int length)
{
	I2C_WriteByteArray(address,reg,buffer,length);			//Write value to register
}
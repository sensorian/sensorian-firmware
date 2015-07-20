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
 * @file FXOS8700CQR1/Example1/FXOS8700CQ.c
 * @author D.Qendri
 * @date 30 May 2015
 * @brief Accelerometer / Magnetometer driver.
 *
 */
 
#include <unistd.h>
#include "FXOS8700CQ.h"
#include "MemoryMap.h"
#include "i2c.h"

/// \defgroup accelerometer Accelerometer and Magnetometer 
/// These functions expose the accelerometer and magnetometer functionality
/// @{

/**
 *@brief Initialize FXOS8700CQ, configure for an output rate of 200Hz with a +/- 2g scale
 *@return none
 */
void  FXOS8700CQ_Initialize(void)
{
  FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG2, RST_MASK);   				//Reset sensor, and wait for reboot to complete
  bcm2835_delay(2);																//Wait at least 1ms after issuing a reset before attempting communications.
  
  FXOS8700CQ_StandbyMode();
  while (FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,CTRL_REG2) & RST_MASK);
  FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,M_CTRL_REG1, (HYBRID_ACTIVE|M_OSR2_MASK|M_OSR1_MASK|M_OSR0_MASK) );      // OSR=max, Hybrid Mode 
  FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,M_CTRL_REG2, M_HYB_AUTOINC_MASK);       							//Enable Hyb Mode Auto Increments  in order to read all data
  FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG4, INT_EN_DRDY_MASK );           						// Enable interrupts for DRDY (TO, Aug 2012)
  FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,XYZ_DATA_CFG, FULL_SCALE_2G);             						//Full Scale of +/-2g
  FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG1, (HYB_ASLP_RATE_25HZ|HYB_DATA_RATE_50HZ));     		//System Output Data Rate of 200Hz (5ms), Sleep Mode Poll Rate of 50Hz (20ms)
}

/**
 *@brief Return the value of the status register
 *@return STATUS register value
 */
char FXOS8700CQ_ReadStatusReg(void)
{
  return FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,STATUS);
}

/**
 *@brief Starts up the sensor in Active mode
 *@return none
 */
void FXOS8700CQ_ActiveMode (void)
{
  char reg1 = FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,CTRL_REG1);
  FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG1, (reg1 | ACTIVE_MASK));   //Set the Active bit in System Control 1 Register.
}

/**
 *@brief Starts up the sensor in Active mode
 *@return none
 */
char FXOS8700CQ_StandbyMode (void)
{
  char n = FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,CTRL_REG1);
  FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG1, n & (~ACTIVE_MASK));
  return (n & ~ACTIVE_MASK);
}

/**
 *@brief Configure the sensor in hybrid mode both the 
 * 		accelerometer and the magnetometer are active for data output
 *@return none
 */
void FXOS8700CQ_HybridMode(void)
{
  FXOS8700CQ_StandbyMode();
  FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,M_CTRL_REG1, (HYBRID_ACTIVE|M_OSR2_MASK|M_OSR1_MASK|M_OSR0_MASK) );      // OSR=max, hybrid mode (TO, Aug 2012)
  FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,M_CTRL_REG2, M_HYB_AUTOINC_MASK);       // enable hybrid autoinc
  FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG4, INT_EN_DRDY_MASK );           // Enable interrupts for DRDY (TO, Aug 2012)
  FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,XYZ_DATA_CFG, FULL_SCALE_2G);             // Set FSR of accel to +/-2g
  FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG1, (HYB_ASLP_RATE_25HZ|HYB_DATA_RATE_50HZ)  );     // Set ODRs
  FXOS8700CQ_ActiveMode();
}

/**
 *@brief Returns current mode of the chip
 *@return mode Current status mode of the chip.
 */
char FXOS8700CQ_GetChipMode(void)
{
    char mode = FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,SYSMOD);
    return mode;
}

/**
 *@brief Returns the chip ID
 *@return Chip ID , should be 0xC7
 */
char FXOS8700CQ_ID (void)
{
    char id  = FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,WHO_AM_I);
    return id;
}

/**
 *@brief Configure the accelerometer for data output
 *@return none
 */
void FXOS8700CQ_ConfigureAccelerometer(void)
{
  FXOS8700CQ_StandbyMode();
  FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG4, INT_EN_DRDY_MASK );           		// Enable interrupts for DRDY (TO, Aug 2012)
  FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,XYZ_DATA_CFG, FULL_SCALE_2G);            		// Set FSR of accel to +/-2g
  FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG1, (HYB_ASLP_RATE_25HZ|HYB_DATA_RATE_50HZ)  );     // Set ODRs
  FXOS8700CQ_ActiveMode();
}

/**
 * @brief Accelerometer data is left justified
 * @param *accel_data Pointer to accelerometer data to read into.
 * @return none
 */
void FXOS8700CQ_PollAccelerometer (rawdata_t *accel_data)
{
	char raw[6] = {0};
    FXOS8700CQ_ReadByteArray(FXOS8700CQ_ADDRESS,OUT_X_MSB, raw, 6);   
    accel_data->x = (raw[0] << 8) | raw[1];		// Pull out 16-bit, 2's complement magnetometer data
    accel_data->y = (raw[2] << 8) | raw[3];
    accel_data->z = (raw[4] << 8) | raw[5];
}

/**
 * @brief Enables or disables higpass filter on accelerometer data
 * @param status Enable or disable status
 * @return none
 */
void FXOS8700CQ_HighPassFilter(char status)
{
    FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,XYZ_DATA_CFG,status);
}

/**
 *@brief Configures the full scale range of the Accelerometer.
 *@param range full scale range.
 *@return mode
 */
void FXOS8700CQ_FullScaleRange(range_t range)
{
    FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,XYZ_DATA_CFG,range);   
}

/**
 *@brief Returns current mode of the chip
 *@param range Dynamic range settings of the accelerometer.
 *@return mode
 */
void FXOS8700CQ_SetAccelerometerDynamicRange(range_t range)
{
	FXOS8700CQ_StandbyMode();	
	switch(range)
	{
		case SCALE2G:
			FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,XYZ_DATA_CFG, (FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,XYZ_DATA_CFG) & ~FS_MASK));		//Write the 2g dynamic range value 
		break;
		case SCALE4G:
			FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,XYZ_DATA_CFG, (FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,XYZ_DATA_CFG) & ~FS_MASK));		//Write the 4g dynamic range value 
			FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,XYZ_DATA_CFG, (FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,XYZ_DATA_CFG) | FULL_SCALE_4G));
		break;
		case SCALE8G:
			FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,XYZ_DATA_CFG, (FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,XYZ_DATA_CFG) & ~FS_MASK));		//Write the 8g dynamic range value 
			FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,XYZ_DATA_CFG, (FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,XYZ_DATA_CFG) | FULL_SCALE_8G));
		break;
		
		default:
		
		break;	
	}	
	FXOS8700CQ_ActiveMode();
}

/**
 *@brief Configure the magnetometer for data output
 *@return none
 */
void FXOS8700CQ_ConfigureMagnetometer(void)
{
  FXOS8700CQ_StandbyMode();
  FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,M_CTRL_REG1, (HYBRID_ACTIVE|M_OSR2_MASK|M_OSR1_MASK|M_OSR0_MASK) );      // OSR=max, hybrid mode (TO, Aug 2012)
  FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,M_CTRL_REG2, M_HYB_AUTOINC_MASK);       // enable hybrid autoinc
  FXOS8700CQ_ActiveMode();
}

/**
 *@brief Return the raw magnetometer data
 *@param mag_data	Pointer to structure containing the 16 bit magnetometer data
 *@return none
 */
void FXOS8700CQ_PollMagnetometer (rawdata_t *mag_data)
{
	char raw[6] = {0};
    FXOS8700CQ_ReadByteArray(FXOS8700CQ_ADDRESS,M_OUT_X_MSB, raw, 6);   
    mag_data->x = (raw[0] << 8) | raw[1];			// Return 16-bit, 2's complement magnetometer data
    mag_data->y = (raw[2] << 8) | raw[3];
    mag_data->z = (raw[4] << 8) | raw[5];
}

/**
 *@brief  Get magnetometer status.
 *@return Status Magnetometer status.
 */
char FXOS8700CQ_MagnetometerStatus(void)
{
    char stat  = FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,M_DR_STATUS);
    return stat;
}

/**
 *@brief Get raw data from both accelerometer and magnetometer.
 *@param *accel_data Pointer to accelerometer data to read into.
 *@param *magn_data Pointer to magnetometer data to read into.
 *@return none
 */
void FXOS8700CQ_GetData(rawdata_t *accel_data, rawdata_t *magn_data)
{
	char raw[12] = {0};
    FXOS8700CQ_ReadByteArray(FXOS8700CQ_ADDRESS,OUT_X_MSB, raw, FXOS8700CQ_READ_LEN);
   
    magn_data->x = (raw[0] << 8) | raw[1];		// Pull out 16-bit, 2's complement magnetometer data
    magn_data->y = (raw[2] << 8) | raw[3];
    magn_data->z = (raw[4] << 8) | raw[5];
    
    accel_data->x = (raw[6] << 8) | raw[7];		// Pull out 14-bit, 2's complement, right-justified accelerometer data
    accel_data->y = (raw[8] << 8) | raw[9];
    accel_data->z = (raw[10] << 8) | raw[11];

    // Have to apply corrections to make the int16_t correct
    if(accel_data->x > UINT14_MAX/2) 
	{
        accel_data->x -= UINT14_MAX;
    }
    if(accel_data->y > UINT14_MAX/2) 
	{
        accel_data->y -= UINT14_MAX;
    }
    if(accel_data->z > UINT14_MAX/2) 
	{
        accel_data->z -= UINT14_MAX;
    }
}

/**
 *@brief Returns current mode of the chip
 *@param mode FIFO mode
 *@return none
 */
void FXOS8700CQ_FIFOMode(mode_t mode)
{
    FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,F_SETUP,6<<mode);
}

/**
 *@brief Get output data rate.
 *@return ODR Output data rate settings 
 */
char FXOS8700CQ_GetODR(void)
{
	unsigned char odr = 0;
	
	return odr;
}

/**
 *@brief Set output data rate.
 *@param DataRateValue 
 *@return none
 */
void FXOS8700CQ_SetODR (char DataRateValue)
{
	DataRateValue <<= 3; 		//Adjust the desired Output Data Rate value as needed.
	FXOS8700CQ_StandbyMode();
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG1,FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,CTRL_REG1) & ~DR_MASK);		//Write in the Data Rate value into Ctrl Reg 1 
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG1, FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,CTRL_REG1)| DataRateValue); 
	FXOS8700CQ_ActiveMode();
}

/**
 *@brief Returns the silicon die chip temperature
 *@return temp Chip temperature
 */
char FXOS8700CQ_GetTemperature(void)
{
    char temp = FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,TEMP);
    
    return temp;
}

/**
 *@brief Returns current mode of the chip
 *@return mode Orientation mode
 */
char FXOS8700CQ_GetOrientation(void)
{
   unsigned char orientation = FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,PL_STATUS);      
   return orientation;
}

/**
 *@brief Configure Orientation mode
 *@return none
 */
void FXOS8700CQ_ConfigureOrientation(void)
{
	unsigned char CTRL_REG1_Data = FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,0x2A); 			//read contents of register 
	CTRL_REG1_Data &= 0xFE; 											//Set last bit to 0. 
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,0x2A, CTRL_REG1_Data); 						//Write the updated value in CTRL_REG1  Put the part into Standby Mode 

	//Set the data rate to 50 Hz (for example, but can choose any sample rate). 

	CTRL_REG1_Data = FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,0x2A); 			//Note: Can combine this step with above 
	CTRL_REG1_Data &= 0xC7; 								//Clear the sample rate bits 
	CTRL_REG1_Data |= 0x20; 								//Set the sample rate bits to 50 Hz 
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,0x2A, CTRL_REG1_Data); 			//Write updated value into the register. 

	//Set the PL_EN bit in Register 0x11 PL_CFG. This will enable the orientation detection. 
	unsigned char PLCFG_Data = FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,0x11); 
	PLCFG_Data |= 0x40; 
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,0x11, PLCFG_Data); 

	// Set the Back/Front Angle trip points in register 0x13 following the table in the data sheet.

	unsigned char PL_BF_ZCOMP_Data = FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,0x13); 
	PL_BF_ZCOMP_Data &= 0x3F; 							//Clear bit 7 and 6 
	//Select one of the following to set the B/F angle value: 
	PL_BF_ZCOMP_Data |= 0x00; 							// This does nothing additional and keeps bits [7:6] = 00 
	PL_BF_ZCOMP_Data |= 0x40; 							// Sets bits[7:6] = 01
	PL_BF_ZCOMP_Data |= 0x80; 							// Sets bits[7:6] = 02
	PL_BF_ZCOMP_Data |= 0xC0;							// Sets bits[7:6] = 03 
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,0x13, PL_BF_ZCOMP_Data); //Write in the updated Back/Front Angle
	//Set the Z-Lockout angle trip point in register 0x13 following the table in datasheet
	PL_BF_ZCOMP_Data = FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,0x1C); 	//Read out contents of the register (can be read by all 
	PL_BF_ZCOMP_Data &= 0xF8; 						//Clear the last three bits of the register 
	PL_BF_ZCOMP_Data |= 0x00; 						//This does nothing additional but the Z-lockout selection will remain at 14°
	//PL_BF_ZCOMP_Data | = 0x01; //Set the Z-lockout angle to 18° 
	//PL_BF_ZCOMP_Data | = 0x02; //Set the Z-lockout angle to 21°
	//PL_BF_ZCOMP_Data | = 0x03; //Set the Z-lockout angle to 25°
	//PL_BF_ZCOMP_Data | = 0x04; //Set the Z-lockout angle to 29° 
	//PL_BF_ZCOMP_Data | = 0x05; //Set the Z-lockout angle to 33° 
	//PL_BF_ZCOMP_Data | = 0x06; //Set the Z-lockout angle to 37° 
	//PL_BF_ZCOMP_Data | = 0x07; //Set the Z-lockout angle to 42° 
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,0x13, PL_BF_ZCOMP_Data); //Write in the updated Z-lockout angle

	//Set the Trip Threshold Angle 
	//NOTE: This register is readable in all versions of MMA845xQ but it is only modifiable in the MMA8451Q. 

	unsigned char P_L_THS_Data = FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,0x14); 					//(can be read by all versions of MMA845xQ)  //The remaining parts of this step only apply to MMA8451Q
	P_L_THS_Data &= 0x07; //Clear the Threshold values 

	//Choose one of the following options
	P_L_THS_Data |= (0x07)<<3; //Set Threshold to 15°
	//P_L_THS_Data | = (0x09)<<3; //Set Threshold to 20° 
	//P_L_THS_Data | = (0x0C)<<3; //Set Threshold to 30° 
	//P_L_THS_Data | = (0x0D)<<3; //Set Threshold to 35° 
	//P_L_THS_Data | = (0x0F)<<3; //Set Threshold to 40°
	//P_L_THS_Data | = (0x10)<<3; //Set Threshold to 45°
	//P_L_THS_Data | = (0x13)<<3; //Set Threshold to 55°
	//P_L_THS_Data | = (0x14)<<3; //Set Threshold to 60° 
	//P_L_THS_Data | = (0x17)<<3; //Set Threshold to 70° 
	//P_L_THS_Data | = (0x19)<<3; //Set Threshold to 75°
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,0x14,P_L_THS_Data); 

	// Set the Hysteresis Angle 
	P_L_THS_Data = FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,0x14); 
	//NOTE: The remaining parts of this step only apply to the MMA8451Q. 
	P_L_THS_Data &= 0xF8;//Clear the Hysteresis values
	//P_L_THS_Data | = 0x01; //Set Hysteresis to ±4° 
	P_L_THS_Data |= 0x02;//Set Threshold to ±7° 
	//P_L_THS_Data | = 0x03;//Set Threshold to ±11° 
	//P_L_THS_Data | = 0x04;//Set Threshold to ±14° 
	//P_L_THS_Data | = 0x05;//Set Threshold to ±17° 
	//P_L_THS_Data | = 0x06; //Set Threshold to ±21° 
	//P_L_THS_Data | = 0x07; //Set Threshold to ±24° 
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,0x14,P_L_THS_Data); 

	//Register 0x2D, Control Register 4 configures all embedded features for interrupt detection.
	/*
	To set this device up to run an interrupt service routine: 
	Program the Orientation Detection bit in Control Register 4. 
	Set bit 4 to enable the orientation detection “INT_EN_LNDPRT”. 
	*/
	unsigned char CTRL_REG4_Data = FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,0x2D); 	//Read out the contents of the register
	CTRL_REG4_Data |= 0x10;						//Set bit 4 
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,0x2D, CTRL_REG4_Data);		//Set the bit and write into CTRL_REG4


	// Register 0x2E is Control Register 5 which gives the option of routing the interrupt to either INT1 or INT2 
	/*
	Depending on which interrupt pin is enabled and configured to the processor: 
	Set bit 4 “INT_CFG_LNDPRT” to configure INT1, or,
	Leave the bit clear to configure INT2. 
	*/

	unsigned char CTRL_REG5_Data = FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,0x2E); 			//In the next two lines choose to clear bit 4 to route to INT2 or set bit 4 to route to INT1 
	CTRL_REG5_Data &= 0xEF;							//Clear bit 4 to choose the interrupt to route to INT2 
	CTRL_REG5_Data |= 0x10;						//Set bit 4 to choose the interrupt to route to INT1 
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,0x2E, CTRL_REG5_Data);				//Write in the interrupt routing selection 

	// Set the debounce counter in register 0x12
	/*
	This value will scale depending on the application-specific required ODR. 
	If the device is set to go to sleep, reset the debounce counter before the device goes to sleep. This setting 
	helps avoid long delays since the debounce will always scale with the current sample rate. The debounce 
	can be set between 50 ms - 100 ms to avoid long delays. 
	*/
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,0x12, 0x05);//This sets the debounce counter to 100 ms at 50 Hz 

	//Put the device in Active Mode 
	CTRL_REG1_Data = FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,0x2A);//Read out the contents of the register 
	CTRL_REG1_Data |= 0x01;//Change the value in the register to Active Mode. 
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,0x2A, CTRL_REG1_Data);//Write in the updated value to put the device in Active Mode 

}

 /**
  *@brief  Configure Single Tap mode
  *@return none
  */
void FXOS8700CQ_ConfigureGenericTapMode(void)
{
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_CFG, 0x55);	//Example X, Y and Z configured for Single Tap with Latch enabled
	//FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_CFG, 0x6A);	//Example X, Y and Z configured for Double Tap with Latch enabled
	//FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_CFG, 0x7F);	//Example X, Y and Z configured for Single Tap and Double Tap with Latch enabled
	
	/**************Set Threesholds*************************/
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_THSX, 0x20); 	//Set X Threshold to 32 counts or 2g
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_THSY, 0x20); 	//Set Y Threshold to 32 counts or 2g
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_THSZ, 0x0C); 	//Set Z Threshold to 48 counts or 3g
	
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_TMLT, 0x06);	//Set the Pulse Time Limit for 30 ms at 200 Hz ODR in Normal Mode with the LPF Enabled A. 30 ms/5 ms = 6 counts
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_LTCY, 0x28);	//Set the Pulse Latency Timer to 200 ms, 200 Hz ODR Low Power Mode, LPF Not Enabled. 200 ms/5.0 ms = 40 counts
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_WIND, 0x0F);	//Set the Pulse window to 300 ms, 100 Hz ODR Low Power Mode, LPF Enabled 300 ms/20 ms = 15 counts	
	
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG4, 0x08);	//Enable Tap Interrupt in Register 0x2D.
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG5,0x08); 	//Set Tap to INT1
}

/**
 *@brief Configure Single Tap mode
 *@return none
 */
void  FXOS8700CQ_ConfigureSingleTapMode(void)
{
	//To set up any configuration make sure to be in Standby Mode.

	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG1, 0x08); 					//400 Hz, Standby Mode
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_CFG, 0x15);					//Enable X and Y and Z Single Pulse
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_THSX, 0x19); 				//Set X Threshold to 1.575g
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_THSY, 0x19); 				//Set Y Threshold to 1.575g 
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_THSZ, 0x2A); 				//Set Z Threshold to 2.65g
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_TMLT,0x50);  				//Set Time Limit for Tap Detection to 50 ms, Normal Mode, No LPF
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_LTCY,0xF0); 					// Set Latency Time to 300 ms
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG4, 0x08); 					//Route INT1 to System
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG5, 0x08); 					//Route Pulse Interrupt Block to INT1 hardware Pin 
	unsigned char CTRL_REG1_Data = FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,CTRL_REG1);		//Read out the contents of the register
	CTRL_REG1_Data |= 0x01; 								//Change the value in the register to Active Mode.

	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG1, CTRL_REG1_Data); 		//Write in the updated value to put the device in 
}

/**
 *@brief DoubleTap_LowPowerMode_NoLPF_400HzODR
 *@return none
 */
void FXOS8700CQ_ConfigureDoubleTapMode(void)
{
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG1, 0x08); 							//400 Hz, Standby Mode
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_CFG, 0x2A);							//Enable X, Y and Z Double Pulse with DPA = 0 no double pulse abort
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_THSX, 0x08); 						//Set X Threshold to 3g 
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_THSY, 0x08); 						//Set Y Threshold to 3g
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_THSZ, 0x03); 						//Set Z Threshold to 5g
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_TMLT,0x30); 							//60 ms Note: 400 Hz ODR, Time step is 1.25 ms per step
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_LTCY,0x50);							//200 ms Set Latency Time to 200 ms
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,PULSE_WIND,0x78); 							//300 ms Set Time Window for second tap to 300 ms
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG4, 0x08); 							//Enable Pulse Interrupt in System CTRL_REG4
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG5, 0x08); 							//Route Pulse Interrupt to INT1 hardware Pin CTRL_REG5

	unsigned char CTRL_REG1_Data = FXOS8700CQ_ReadByte(FXOS8700CQ_ADDRESS,CTRL_REG1); 	//Read out the contents of the register
	CTRL_REG1_Data |= 0x01; 										//Change the value in the register to Active Mode.
	
	FXOS8700CQ_WriteByte(FXOS8700CQ_ADDRESS,CTRL_REG1, CTRL_REG1_Data); 				//Write in the updated value to put the device in Active Mode.
}

/// @}

/**
 *@brief Writes a value to a register
 *@param reg Register address to write into.
 *@param value Data to write to register.
 *@return none
 */
void FXOS8700CQ_WriteByte(unsigned char address,char reg, char value)
{
	I2C_WriteByteRegister(address,reg,value);			//Write value to register
}

/**
 *@brief Writes an array of bytes to the sensor
 *@param reg Initial register address to write to.
 *@param *buffer Pointer to buffer array to read from.
 *@param length Length of buffer array
 *@return none
 */
void FXOS8700CQ_WriteByteArray(unsigned char address, char reg, char* buffer, char length)
{
	I2C_WriteByteArray(address,reg,buffer,length);			//Write values to register
}

/**
 *@brief Starts up the sensor in Active mode
 *@param reg Register address to read from.
 *@return data Contents of register.
 */
char FXOS8700CQ_ReadByte(unsigned char address,char reg)
{
    return I2C_ReadByteRegister(address,reg);		//Read register current value
}

/**
 *@brief Read a number of contigous bytes from the chip 
 *@param reg Initial register address to read from.
 *@param *buffer Pointer to buffer array to read into.
 *@param length Length of buffer array
 *@return none
 */
void FXOS8700CQ_ReadByteArray(unsigned char address,char reg, char *buffer, unsigned int length)
{
    I2C_ReadByteArray(address,reg,buffer,length);	//Read values starting from the reg address
}
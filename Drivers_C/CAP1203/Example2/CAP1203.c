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
 * @file /CAP1203/Example2/CAP1203.c
 * @author D.Qendri
 * @date 28 May 2015
 * @brief CAP1203 driver.
 */
 
#include "CAP1203.h"

/// \defgroup capTouch Capacitive Touch Controller
/// These functions let you communicate with the CAP1203 capacitive touch controller.
/// @{

/**
 *@brief Basic configuration for the capacitive touch controller. 
 *       Configures the sensor inactive mode and activates all three channels.
 *
 *@return none
 */	
void CAP1203_Initialize(void)
{
    CAP1203_ActiveMode();                       //All three sensors are monitored in Active mode
	CAP1203_Write(SENSINPUTEN,CS1|CS2|CS3);     //Set active inputs
    CAP1203_Write(AVERAGE_SAMP_CONF, AVG|SAMP_TIME|CYCLE_TIME);	//Setup averaging and sampling time
}

/**
 * @brief Configures the controller in active mode.
 * @return status Contents of status register
 */
unsigned char CAP1203_ActiveMode(void)
{
    unsigned char status = CAP1203_Read(MAIN_CTRL_REG);
    status &= ~STBY;
    CAP1203_Write(MAIN_CTRL_REG,status);
    return CAP1203_Read(MAIN_CTRL_REG);
}

/**
 * @brief Configures the controller in standby mode.
 * @return status Contents of status register
 */
unsigned char CAP1203_StandbyMode(void)
{
    CAP1203_Write(STANDBY_SENS,0x07);       //Set sensitivity in standby mode    
    unsigned char status = CAP1203_Read(MAIN_CTRL_REG);
    status |= STBY;
    CAP1203_Write(MAIN_CTRL_REG,status);
    return CAP1203_Read(MAIN_CTRL_REG);
}

/**
 * @brief Configures the controller in deep sleep mode
 * @return reg Status register
 */
unsigned char CAP1203_DeepSleep(void)
{
    unsigned char status = CAP1203_Read(MAIN_CTRL_REG);
    status |= SLEEP;                            //Set Sleep bit
    CAP1203_Write(MAIN_CTRL_REG,status);        //Update register
    return CAP1203_Read(MAIN_CTRL_REG);
}

/**
 * @brief Takes the controller out of deep sleep mode
 * @return reg Status register
 */
unsigned char CAP1203_ResumeFromDeepSleep(void)
{
    unsigned char status = CAP1203_Read(MAIN_CTRL_REG);
    status &= ~SLEEP;
    CAP1203_Write(MAIN_CTRL_REG,status);
    return CAP1203_Read(MAIN_CTRL_REG);
}

/**
 * @brief Enables the multi-touch capability of the controller
 * @param number touch type
 * @param mulchan One of the three channels.
 * @return none
 */
void CAP1203_ConfigureMultiTouch(touch_type number,unsigned char mulchan )
{
    CAP1203_Write(MULTITOUCH,((unsigned char)number)|MULTBLK_EN);       //Set number of simultaneous touches
    CAP1203_Write(MULTIPATCONF,MTP_EN|MTP_TH1|MTP_TH0|COMP_PTRN|MTP_ALERT);       //Enable multitouch
    CAP1203_Write(MULTIPATTERN,mulchan);
}

/**
 * @brief Return true if a multitouch event was detected.
 * @return none
 */
unsigned char CAP1203_MultitouchEvent(void)
{
	unsigned char mt = 0;
    unsigned char multi = CAP1203_Read(GEN_STATUS);
    if((multi & MULT) == MULT)
	{
		mt = 1;
	}
	return mt;
}

/**
 * @brief  Configures the button as a power button.
 * @param button One of the three buttons
 * @return none
 */
void CAP1203_SetPowerButton(button_type button)
{
    CAP1203_Write(PWR_BUTTON,button);
    CAP1203_Write(PWR_CONFIG,PWR_EN|TIME1120ms);		//Configure as power button in Active mode
}	


/**
 * @brief  Read the status of the power button.
 * @return powerbutton status
 */
unsigned char CAP1203_ReadPowerButton(void)
{
	unsigned char pressed = 0;
    unsigned char button = CAP1203_Read(GEN_STATUS);
	
	if (button & PWR)
	{
		pressed = 1;
	}else{
		pressed = 0;
	}
	return pressed;
}

/**
 * @brief Return id indicating which button was pressed.
 * @return Button pressed
 */
unsigned char CAP1203_ReadPressedButton(void)
{
    unsigned char buttonPressed = 0;
	unsigned char status = CAP1203_GetStatusReg();      //Check if touch bit was registered
    if (status & TOUCH)
    {
        unsigned char button = CAP1203_Read(SENSOR_INPUTS);
        switch(button)
        {
            case CS1:
					buttonPressed = 3;
                break;
            case CS2:
					buttonPressed = 2;
                break;
            case CS3:
					buttonPressed = 1;
                break;
            default:
					buttonPressed = 0;
                break;
        }
    }	
	CAP1203_Write(MAIN_CTRL_REG,0x00);		//Clear interrupt
	return buttonPressed;    
}

/**
 * @brief Reads the status register.
 * @return status Contents of status register
 */
unsigned char CAP1203_GetStatusReg(void)
{
    unsigned char status = CAP1203_Read(GEN_STATUS);
    return status;
}

/**
 *@brief Enable interrupt, the ALERT pin goes high once an interrupt occurs
 *@param pin Enables interrupt on the specific pin
 *@return none
 */
void CAP1203_EnableInterrupt(button_type pin)
{
    CAP1203_Write(REPEAT_RATE,pin);     //Enable repeat rate for the input pins
    CAP1203_Write(INT_ENABLE,pin);		//2:0 c2:c1:c0  last three bits are the specific channels
}

/**
 *@brief Sets the sensitivity of each channel.
 *@param sensitivity Change button sensitivity settings.
 *@return none
 */
void CAP1203_SetSensitivity(sensitivity_type sensitivity)
{
    CAP1203_Write(0x00,sensitivity);
}

/**
 *@brief Checks the sensor status
 *@return Status Current chip status
 */
unsigned char CAP1203_CheckSensorStatus(void)
{
    unsigned char sensor = 0;

    return sensor;
}

/**
 *@brief Clears any impending interrupts
 *@return intStatus Interrupt status
 */	
unsigned char CAP1203_ClearInterrupt(void)
{
    unsigned char intStatus = 0x00;
    CAP1203_Read(GEN_STATUS);
    return intStatus;
}

/**
 *@brief Returns product chip and manufacturing ID
 *@return chipID 16 bit chip and manufacturer ID
 */
unsigned int CAP1203_ReadID(void)
{
    unsigned char id = CAP1203_Read(PRODUCT_ID);
    unsigned char manu = CAP1203_Read(MAN_ID);
    return ((manu << 8)|id);
}

/// @}

/**
 * @brief Write data value to register address.
 * @param reg Register to write to.
 * @param data Value to write to register
 */
void CAP1203_Write(unsigned char reg, unsigned char data)
{
	I2C_WriteByteRegister(reg,data);				//Write data to register reg
}

/**
 * @brief Reads data from register reg address.
 * @param reg	Register to read from
 * @return Register value
 */
unsigned char CAP1203_Read(unsigned char reg)
{
    unsigned char value = I2C_ReadByteRegister(reg);
 
    return value;
}

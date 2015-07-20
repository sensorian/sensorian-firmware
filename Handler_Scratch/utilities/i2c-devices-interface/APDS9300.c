#include "APDS9300.h"
#include "i2c.h"
#include "math.h"

#pragma GCC diagnostic ignored "-pedantic"

/// \defgroup light Ambient Light Sensor 
/// These functions let you communicate with the APDS9300 ambient light sensor
/// @{

/**
 *@brief Powers on the sensor , sets sensor gain to 1 and the sampling time to 402 ms and disables the interrupts
 *@return control Contents of control register
 */
unsigned char AL_Initialize(void)
{    
	char control = APDS9300_ReadByte(CONTROL|CMD_CLEAR_INT|CMD_WORD);		//Value read should be 0x13
	AL_PowerState(POWER_ON);								//Power on sensor	
	return control;
}

/**
 *@brief Set the sensor power state 
 *@param state	variable fo type powerState denotes the power state of the sensor
 *@return none
 */
void AL_PowerState(powerState state)
{
	unsigned char cmd  = state;
    APDS9300_WriteByte(COMMAND|CMD_CLEAR_INT|CONTROL);         //Clear interrupts 
	APDS9300_WriteByte(cmd);           						//Power on/off the sensors by writing to control reg
}

/**
 *@brief Returns the CHIP id.
 *@return partID Chip ID
 */
unsigned char AL_ChipID(void)
{
    unsigned char partID = 0x00;

    partID = APDS9300_ReadByte(COMMAND|ID);
    return partID;
}

/**
 *@brief Reads the value from one of the two channels
 *@param chan Channel id
 *@return channelValue 16 bit sensor data representing channel value
 */
unsigned int AL_ReadChannel(channel chan)
{
    unsigned int channelValue = 0;
    switch(chan)
    {
        case CH0:
			APDS9300_WriteByte(COMMAND|CMD_CLEAR_INT|CMD_WORD|DATA0LOW);
            channelValue = APDS9300_ReadWord();
            break;
        case CH1:
			APDS9300_WriteByte(COMMAND|CMD_CLEAR_INT|CMD_WORD|DATA1LOW);
            channelValue = APDS9300_ReadWord();
            break;
        default:
			channelValue = 0;
        break;
    }
    return channelValue;
}

/**
 * @brief Returns LUX output from the sensor
 * @param ch0 Channel 1 value
 * @param ch1 Channel 2 value
 * @return Lux Float value of light intensity
 */
float AL_Lux(unsigned int ch0, unsigned int ch1)
{
	float k = ch1/ch0;
	float Lux=0;

	if((k>=0)&& (k<=0.52))
		Lux=(0.0315*ch0)-(0.0593*ch0*pow(k,1.4));
	else if((k>0.52)&& (k<=0.65))
		Lux=(0.0229*ch0)-(0.0291*ch1);
	else if((k>0.65)&& (k<=0.80))
		Lux=(0.0157*ch0)-(0.0180*ch1);
	else if((k>0.80)&& (k<=1.30))
		Lux=(0.00338*ch0)-(0.00260*ch1);
	else 
		Lux=0;
	return Lux;
}

/**
 *@brief Set the sensor gain. Default gain value is GAIN_1.
 *@param val Gain type can be GAIN_1 or GAIN_16 for gain of 1 or 16x
 *@return val Gain mode
 */
unsigned char AL_SetGain(gain val)
{
    unsigned char value = 0;
    switch(val)
    {
        case GAIN_1:
			value  = APDS9300_ReadByte(COMMAND|TIMING);       //Write to TIMING register
            value &= 0b11101111;
            APDS9300_WriteByte(COMMAND|TIMING);       //Write to TIMING register
			APDS9300_WriteByte(value);
            break;
        case GAIN_16:
			APDS9300_WriteByte(COMMAND|TIMING);		//Write to TIMING register
			APDS9300_WriteByte(GAIN);
            break;
        default:
            APDS9300_WriteByte(COMMAND|TIMING);
			APDS9300_WriteByte(GAIN);
    }
    return val;
}

/**
 *@brief Sets the sampling time for the sensor,can be one of three predetermined values
 *@param sampling_time can be 0x00,0x01,0x02
 *@return none
 */
void AL_SetSamplingTime(sampTime_t sampling_time)
{
    APDS9300_WriteByte(COMMAND|TIMING);
	APDS9300_WriteByte(sampling_time);
}

/**
 *@brief Sets the low threshold value for the interrupt
 *@param lowthreshvalue Interrupt low threshold value
 *@return none
 */
void AL_SetIntLowThreshold(unsigned int lowthreshvalue)
{
     APDS9300_WriteWord(COMMAND | THRESHLOWLOW| CMD_WORD,lowthreshvalue); //use the write word protocol to write these 16 bit values
}

/**
 *@brief Sets the high threshold value for the interrupt
 *@param highthreshvalue Interrupt high threshvalue value
 *@return none
 */
void AL_SetIntHighThreshold(unsigned int highthreshvalue)
{
    APDS9300_WriteWord(COMMAND|THRESHHIGHLOW| CMD_WORD,highthreshvalue);
}

/**
 *@brief Clear any existing interrupts
 *@return none
 */
void AL_Clear_Interrupt(void)
{
    APDS9300_WriteByte(COMMAND|CMD_CLEAR_INT);			//Set clear bit to 1
}

/**
 *@brief Enables or disables the interrupt and sets the persistence of interrupt occurences
 *		 The interrupt will fire after x occurences outside the threshold values
 *		 Default values are interrupts enabled and interrupts enabled
 *@param enable 	  Enables or disable interrupts 1/0 to enable/disable
 *@param persistence  Value from 0 to 16 for value 0 interrupts occur after every sample
 *@return none
 */
void AL_ConfigureInterrupt(unsigned char enable, unsigned char persistence)
{
    if(enable)
    {
        APDS9300_WriteByte(COMMAND|INTERRUPT|INTERR_ENA);
		APDS9300_WriteByte(persistence);
    }
    else
    {
        APDS9300_WriteByte(COMMAND|INTERRUPT);
		APDS9300_WriteByte(INTERR_DISMASK);
    }
}

/// @}


/**
 * @brief Writes a single byte of data to the current sensor register. 
 * 		  Function assumes that I2C peripheral is preconfigured.
 * @param data Data byte to write to the sensor.
 * @return none
 */
void APDS9300_WriteByte(unsigned char data)
{
	I2C_WriteByte(APDS9300ADDR,data);			//Sets a specific register to a certain value
}

/**
 * @brief Writes a byte to the specific register
 * @param reg Address of sensor register 
 * @param data Value to be written on the sensor register.
 * @return none
 */
void APDS9300_WriteRegister(unsigned char reg, unsigned char data)
{
	I2C_WriteByteRegister(APDS9300ADDR,reg,data);			//Writes data on the specific register
}

/**
 * @brief Writes a word to the specific register
 * @param reg  Address of sensor register
 * @param data 16 bit data  to be written on the sensor register. 
 * @return none
 */
void APDS9300_WriteWord(unsigned char reg, unsigned int data)
{
	unsigned char buffer[2];
	buffer[1] = (unsigned char) (data >> 8);			//MSB
	buffer[0] = (unsigned char) (data && 0xff);			//LSB
	I2C_WriteWordRegister(APDS9300ADDR,reg,buffer);
}

/**
 * @brief Reads a single byte from the given sensor register
 * @param reg Address of sensor register
 * @return Val Value of sensor register
 */
char APDS9300_ReadByte(char reg)
{
    return I2C_ReadByteRegister(APDS9300ADDR,reg);
}

/**
 * @brief Read word from register
 * @param reg Register address
 * @return val 16bit word content of register
 */
unsigned int APDS9300_ReadWordReg(char reg)
{	unsigned int val = I2C_ReadWordRegisterRS(APDS9300ADDR,reg);
    return val;
}

/**
 * @brief Read a single word from the sensor. Assumes register pointer is preset.
 * @return value 16bit word read from the preset register
 */
unsigned int APDS9300_ReadWord(void)
{
	char buff[2] = {0x00,0x00};
	bcm2835_i2c_setSlaveAddress(APDS9300ADDR);
	bcm2835_i2c_read(buff,2);
	

	return (buff[1] << 8)|buff[0];
}

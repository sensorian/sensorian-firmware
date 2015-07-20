#include <stdlib.h>
#include "i2c.h"

/**
 *@brief Initializes the I2C peripheral
 *@param address Address the I2C peripheral is communicating with.
 *@return none
 */
 
/*
void I2C_Initialize(unsigned char address)
{    					
	if (!bcm2835_init())						//Configure I2C pins
	{
		printf("BCM libray error.\n");
	}
	bcm2835_i2c_end();		//Close I2C peripheral to reconfigure it
	
	bcm2835_i2c_begin();						//Set pins as I2C
	bcm2835_i2c_set_baudrate(baudrate);			//Set I2C baudrate
	bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_2500);		//100 Khz
	bcm2835_i2c_setSlaveAddress(address);	//Set device address
}
*/
void I2C_Initialize(void)
{    					
	if (!bcm2835_init())						//Configure I2C pins
	{
		printf("BCM libray error.\n");
	}
	bcm2835_i2c_end();		//Close I2C peripheral to reconfigure it
	
	bcm2835_i2c_begin();						//Set pins as I2C
	bcm2835_i2c_set_baudrate(baudrate);			//Set I2C baudrate
	bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_2500);		//100 Khz
	//bcm2835_i2c_setSlaveAddress(address);	//Set device address
}

/**
 *@brief Writes a byte value to the I2C bus. This assumes the register pointer is preset.
 *@param bdata	Send single byte to I2C bus
 *@return none
 */
void I2C_WriteByte(unsigned char address,char bdata)
{
	bcm2835_i2c_setSlaveAddress(address);
	char data = bdata;
	bcm2835_i2c_write(&data, 1);
}

/**
 *@brief Writes a byte value to a register address
 *@param reg Address of sensor register.
 *@param data Data byte to be written on register.
 *@return none
 */
void I2C_WriteByteRegister(unsigned char address,unsigned char reg,unsigned char data)
{
	
	bcm2835_i2c_setSlaveAddress(address);
	
	unsigned char wr_buf[2];

	wr_buf[0] = reg;
	wr_buf[1] = data;

	bcm2835_i2c_write((const char *)wr_buf, 2);
}

/**
 *@brief Writes a word value (16 bit) to a register address.
 *@param reg Address of sensor register.
 *@param data Data word to be written on word size register.
 *@return none
 */
void I2C_WriteWordRegister(unsigned char address,unsigned char reg, unsigned char* data)
{
	
	bcm2835_i2c_setSlaveAddress(address);
	
	unsigned char wr_buf[3];
	
	wr_buf[0] = reg;
	wr_buf[1] = data[0];
	wr_buf[2] = data[1];

	bcm2835_i2c_write((const char *)wr_buf, 3);
}

/**
 *@brief Writes a buffer array to the registers
 *@param reg	Address of sensor register, address autoincrements
 *@param data	Pointer to byte data buffer array
 *@param length	length of buffer array
 *@return none
 */
void I2C_WriteByteArray(unsigned char address, char reg, char* data, unsigned int length)
{
	
	bcm2835_i2c_setSlaveAddress(address);
	
	char* wr_buf = (char*) malloc(sizeof(char) * length);
	if (wr_buf==NULL) 
	{
		printf("Error allocating memory!\n"); //print an error message
	}
	
	wr_buf[0] = reg;
	for(unsigned int i = 1;i<length;i++)
	{
		wr_buf[i] = data[i];
	}

	bcm2835_i2c_write((const char *)wr_buf, length);
}

/**
 *@brief Reads a byte from a register
 *@param reg Address of sensor register.
 *@return val Byte value of register.
 */
unsigned char I2C_ReadByteRegister(unsigned char address, char reg)
{
	
	bcm2835_i2c_setSlaveAddress(address);
	
	char val = 0;
 
	bcm2835_i2c_read_register_rs(&reg,&val,1);
	
	return val;
}
 
 /**
 *@brief Initializes the I2C peripheral
 *@param reg	Address of sensor register, address autoincrements
 *@param *buffer	Pointer to byte data buffer array
 *@param length	length of buffer array
 *@return none
 */
void I2C_ReadByteArray(unsigned char address,char reg,char *buffer,unsigned int  length)
{
	
	bcm2835_i2c_setSlaveAddress(address);
	
	bcm2835_i2c_read_register_rs(&reg,buffer,length);
}

 /**
 *@brief Readm result from a word length register
 *@param reg register to read from
 *@return val Word value of register
 */
unsigned int I2C_ReadWordRegisterRS(unsigned char address,char reg)
{
	
	bcm2835_i2c_setSlaveAddress(address);
	
	char cmd[1] = {reg}; 
	char receive[2] = {0};
	bcm2835_i2c_write_read_rs(cmd,1,receive,2);
	
	return (receive[0]<<8)|receive[1];
}

/**
 *@brief Read the value of a register that has already been select via the address pointer
 *@return Data Value of preset register
 */
unsigned int I2C_ReadWordPresetPointer(unsigned char address)
{
	
	bcm2835_i2c_setSlaveAddress(address);
	
	char val[2] = {0}; 
	bcm2835_i2c_read(val,2);
	unsigned int data = (val[0] << 8)|val[1];
	
	return data;
}
 
/**
 *@brief Closes the I2C peripheral
 *@return none
 */
void I2C_Close(void)
{
	bcm2835_i2c_end();
}


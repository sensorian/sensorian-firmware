#ifndef __I2C1_H__
#define __I2C1_H__

#include <stdio.h>
#include "bcm2835.h"

#define baudrate		100000

//void 			I2C_Initialize(unsigned char address);
void 			I2C_Initialize(void);

//void 			I2C_WriteByte(char byte);
void 			I2C_WriteByte(unsigned char address, char byte);

//void 			I2C_WriteByteRegister(unsigned char reg,unsigned char data);
void I2C_WriteByteRegister(unsigned char address,unsigned char reg,unsigned char data);

//void 			I2C_WriteWordRegister(unsigned char reg, unsigned char* data);
void I2C_WriteWordRegister(unsigned char address,unsigned char reg, unsigned char* data);

//void 			I2C_WriteByteArray(char reg, char* data, unsigned int length);
void I2C_WriteByteArray(unsigned char address, char reg, char* data, unsigned int length);

//unsigned char 	I2C_ReadByteRegister(char reg);
unsigned char I2C_ReadByteRegister(unsigned char address, char reg);

//void 			I2C_ReadByteArray(char reg,char *buffer,unsigned int  length);
void I2C_ReadByteArray(unsigned char address,char reg,char *buffer,unsigned int  length);

unsigned int 	I2C_ReadWordRegister(char reg);

//unsigned int 	I2C_ReadWordRegisterRS(char reg);
unsigned int I2C_ReadWordRegisterRS(unsigned char address,char reg);

//unsigned int 	I2C_ReadWordPresetPointer(void);
unsigned int I2C_ReadWordPresetPointer(unsigned char address);

void 			I2C_Close(void);

#endif



/* 
* This file is part of VL53L1 Platform 
* 
* Copyright (c) 2016, STMicroelectronics - All Rights Reserved 
* 
* License terms: BSD 3-clause "New" or "Revised" License. 
* 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met: 
* 
* 1. Redistributions of source code must retain the above copyright notice, this 
* list of conditions and the following disclaimer. 
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, 
* this list of conditions and the following disclaimer in the documentation 
* and/or other materials provided with the distribution. 
* 
* 3. Neither the name of the copyright holder nor the names of its contributors 
* may be used to endorse or promote products derived from this software 
* without specific prior written permission. 
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
* 
*/

#include "vl53l1_platform.h"
#include <string.h>
#include <time.h>
#include <math.h>
#include "I2C.h"

int8_t VL53L1_WriteMulti( uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count) {
	//	send device address, intent to write
	if (I2C_Start( 0x29, I2C_WRITE))
		return -1;

	//	write the first byte of the index
	if (I2C_Write8( (unsigned char)(index >> 8), I2C_NOSTOP))
		return -2;

	//	write the rest of the index
	if (I2C_Write8( (unsigned char)index, I2C_NOSTOP))
		return -2;

	for (int i = 0; i < count -1; i++) {
		//	write the command
		if (I2C_Write8( pdata[i], I2C_NOSTOP))
			return -3;
	}

	//	write the command
	if (I2C_Write8( pdata[count - 1], I2C_STOP))
		return -4;

	return 0;

}

int8_t VL53L1_ReadMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count){
	// start transaction with device address, intent to write
	if (I2C_Start(0x29, I2C_WRITE))
		return -1;

	//	write the first byte of the index
	if (I2C_Write8( (unsigned char)(index >> 8), I2C_NOSTOP))
		return -2;

	//	write the rest of the index
	if (I2C_Write8( (unsigned char)index, I2C_STOP))
		return -2;

	// start transaction with device address, intent to read
	if (I2C_Start(0x29, I2C_READ))
		return -3;

	// read the data bytes
	for (uint32_t i = 0; i < count; i++) {
		if (i == (count - 1)) {
			if (I2C_Read8(&pdata[i], I2C_ACK, I2C_STOP)){
				return -4;
			}
		}
		else {
			if (I2C_Read8(&pdata[i], I2C_NACK, I2C_NOSTOP))
				return -5;
		}
	}

	return 0;
}

int8_t VL53L1_WrByte(uint16_t dev, uint16_t index, uint8_t data) {
	//	send device address, intent to write
	if (I2C_Start( 0x29, I2C_WRITE))
		return -1;

	//	write the first byte of the index
	if (I2C_Write8( (unsigned char)(index >> 8), I2C_NOSTOP))
		return -2;

	//	write the rest of the index
	if (I2C_Write8( (unsigned char)index, I2C_NOSTOP))
		return -2;

	//	write the command
	if (I2C_Write8( data, I2C_NOSTOP))
		return -3;

	return 0;
}

int8_t VL53L1_WrWord(uint16_t dev, uint16_t index, uint16_t data) {
	// send device address, intent to write
	if (I2C_Start(0x29, I2C_WRITE))
		return -1;
	
	//	write the first byte of the index
	if (I2C_Write8( (unsigned char)(index >> 8), I2C_NOSTOP))
		return -2;

	//	write the rest of the index
	if (I2C_Write8( (unsigned char)index, I2C_NOSTOP))
		return -2;
	
	// write the command
	if (I2C_Write8((unsigned char)(data >> 8), I2C_NOSTOP))
		return -3;

	// write the command
	if (I2C_Write8((unsigned char)data, I2C_STOP))
		return -4;

	return 0;
}

int8_t VL53L1_WrDWord(uint16_t dev, uint16_t index, uint32_t data) {
	// send device address, intent to write
	if (I2C_Start(0x29, I2C_WRITE))
		return -1;

	//	write the first byte of the index
	if (I2C_Write8( (unsigned char)(index >> 8), I2C_NOSTOP))
		return -2;

	//	write the rest of the index
	if (I2C_Write8( (unsigned char)index, I2C_NOSTOP))
		return -2;

	// write the command
	if (I2C_Write8((unsigned char)(data >> 24), I2C_NOSTOP))
		return -3;

	// write the command
	if (I2C_Write8((unsigned char)(data >> 16), I2C_NOSTOP))
		return -4;

	// write the command
	if (I2C_Write8((unsigned char)(data >> 8), I2C_NOSTOP))
		return -5;

	// write the command
	if (I2C_Write8(data, I2C_STOP))
		return -6;

	return 0;
}

int8_t VL53L1_RdByte(uint16_t dev, uint16_t index, uint8_t *data) {
	// start transaction with device adress, intent to write
	if (I2C_Start(0x29, I2C_WRITE))
		return -1;

	//	write the first byte of the index
	if (I2C_Write8( (unsigned char)(index >> 8), I2C_NOSTOP))
		return -2;

	//	write the rest of the index
	if (I2C_Write8( (unsigned char)index, I2C_STOP))
		return -3;

	// start transaction with device adress, intent to read
	if (I2C_Start(0x29, I2C_READ))
		return -4;

	// stop the transaction
	if (I2C_Read8(data, I2C_NACK, I2C_STOP))
		return -5;

	return 0;
}

int8_t VL53L1_RdWord(uint16_t dev, uint16_t index, uint16_t *data) {
	unsigned char ucDataH, ucDataL;
	// start transaction with device address, intent to write
	if (I2C_Start(0X29, I2C_WRITE))
		return -1;

	//	write the first byte of the index
	if (I2C_Write8( (unsigned char)(index >> 8), I2C_NOSTOP))
		return -2;

	//	write the rest of the index
	if (I2C_Write8( (unsigned char)index, I2C_STOP))
		return -2;

	// start transaction with device address, intent to read
	if (I2C_Start(0x29, I2C_READ))
		return -3;

	// read the low byte
	if (I2C_Read8(&ucDataL, I2C_ACK, I2C_NOSTOP))
		return -4;

	// read the high byte
	if (I2C_Read8(&ucDataH, I2C_NACK, I2C_STOP))
		return -5;

	*data = (ucDataL << 8) | ucDataH;

	return 0;	
}

int8_t VL53L1_RdDWord(uint16_t dev, uint16_t index, uint32_t *data) {
	unsigned char ucData[4];
	// start transaction with device address, intent to write
	if (I2C_Start(0X29, I2C_WRITE))
		return -1;

	//	write the first byte of the index
	if (I2C_Write8( (unsigned char)(index >> 8), I2C_NOSTOP))
		return -2;

	//	write the rest of the index
	if (I2C_Write8( (unsigned char)index, I2C_STOP))
		return -2;

	// start transaction with device address, intent to read
	if (I2C_Start(0x29, I2C_READ))
		return -3;

	// read the data bytes
	if (I2C_Read8(&ucData[0], I2C_ACK, I2C_NOSTOP))
		return -4;
	if (I2C_Read8(&ucData[1], I2C_ACK, I2C_NOSTOP))
		return -5;
	if (I2C_Read8(&ucData[2], I2C_ACK, I2C_NOSTOP))
		return -6;
	if (I2C_Read8(&ucData[3], I2C_NACK, I2C_STOP))
		return -7;

	*data = (ucData[0] << 24) | (ucData[1] << 16) | (ucData[2] << 8) | ucData[3];

	return 0;
}

int8_t VL53L1_WaitMs(uint16_t dev, int32_t wait_ms){
	return 0; // to be implemented
}

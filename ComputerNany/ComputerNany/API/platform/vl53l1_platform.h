/**
 * @file  vl53l1_platform.h
 * @brief Those platform functions are platform dependent and have to be implemented by the user
 */
 
#ifndef _VL53L1_PLATFORM_H_
#define _VL53L1_PLATFORM_H_

#include "vl53l1_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
	uint32_t dummy;
} VL53L1_Dev_t;

typedef VL53L1_Dev_t *VL53L1_DEV;

/** @brief VL53L1_WriteMulti() definition.
 * This function writes multiple bytes of data to a specific index in the VL53L1 sensor. 
 * The function takes the device address, 
 * index of the data, 
 * pointer to the data to be written, 
 * and the count of the data as inputs.
 */
int8_t VL53L1_WriteMulti(
		uint16_t 			dev,
		uint16_t      index,
		uint8_t      *pdata,
		uint32_t      count);
/** @brief VL53L1_ReadMulti() definition.
 * This function reads multiple bytes of data from a specific index in the VL53L1 sensor. 
 * The function takes the device address, 
 * index of the data, 
 * pointer to where the data should be stored, 
 * and the count of the data as inputs.
 */
int8_t VL53L1_ReadMulti(
		uint16_t 			dev,
		uint16_t      index,
		uint8_t      *pdata,
		uint32_t      count);
/** @brief VL53L1_WrByte() definition.
 * This function writes a single byte of data to a specific index in the VL53L1 sensor. 
 * The function takes the device address, 
 * index of the data, 
 * and the data to be written as inputs.
 */
int8_t VL53L1_WrByte(
		uint16_t dev,
		uint16_t      index,
		uint8_t       data);
/** @brief VL53L1_WrWord() definition.
 * This function writes a 16-bit word of data to a specific index in the VL53L1 sensor. 
 * The function takes the device address, 
 * index of the data, 
 * and the data to be written as inputs.
 */
int8_t VL53L1_WrWord(
		uint16_t dev,
		uint16_t      index,
		uint16_t      data);
/** @brief VL53L1_WrDWord() definition.
 *  This function writes a 32-bit double word of data to a specific index in the VL53L1 sensor. 
 * 	The function takes the device address, 
 * 	index of the data, 
 * 	and the data to be written as inputs.
 */
int8_t VL53L1_WrDWord(
		uint16_t dev,
		uint16_t      index,
		uint32_t      data);
/** @brief VL53L1_RdByte() definition.
 * This function reads a single byte of data from a specific index in the VL53L1 sensor. 
 * The function takes the device address, 
 * index of the data, and a pointer to where the data should be stored as inputs.
 */
int8_t VL53L1_RdByte(
		uint16_t dev,
		uint16_t      index,
		uint8_t      *pdata);
/** @brief VL53L1_RdWord() definition.
 * This function reads a 16-bit word of data from a specific index in the VL53L1 sensor. 
 * The function takes the device address, 
 * index of the data, 
 * and a pointer to where the data should be stored as inputs.
 */
int8_t VL53L1_RdWord(
		uint16_t dev,
		uint16_t      index,
		uint16_t     *pdata);
/** @brief VL53L1_RdDWord() definition.
 * This function reads a 32-bit double word of data from a specific index in the VL53L1 sensor. 
 * The function takes the device address, 
 * index of the data, 
 * and a pointer to where the data should be stored as inputs.
 */
int8_t VL53L1_RdDWord(
		uint16_t dev,
		uint16_t      index,
		uint32_t     *pdata);
/** @brief VL53L1_WaitMs() definition.
 * This function delays execution for a specified number of milliseconds. 
 * The function takes the device address and 
 * the number of milliseconds to wait as inputs.
 */
int8_t VL53L1_WaitMs(
		uint16_t dev,
		int32_t       wait_ms);

#ifdef __cplusplus
}
#endif

#endif

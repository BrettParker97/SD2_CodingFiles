/*
 * helpful_i2c.h
 *
 *  Created on: Apr 2, 2022
 *      Author: Brett
 */

#ifndef BRETTS_HELPER_INC_HELPFUL_I2C_H_
#define BRETTS_HELPER_INC_HELPFUL_I2C_H_

#include "main.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int WrByte(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t index, uint8_t data);

int Brett_I2CWrite(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pdata, uint32_t count);

int RdByte(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t index, uint8_t *data);

int Brett_I2CRead(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pdata, uint32_t count);

int ReadMulti(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t index, uint8_t *pdata, uint32_t count);



#endif /* BRETTS_HELPER_INC_HELPFUL_I2C_H_ */

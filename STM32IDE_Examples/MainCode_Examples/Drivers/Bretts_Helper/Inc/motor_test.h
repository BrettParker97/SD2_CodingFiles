/*
 * motor_test.h
 *
 *  Created on: Apr 8, 2022
 *      Author: Brett
 */

#include "stm32F4xx_hal.h"

#ifndef BRETTS_HELPER_INC_MOTOR_TEST_H_
#define BRETTS_HELPER_INC_MOTOR_TEST_H_


void MoveForward(I2C_HandleTypeDef *hi2c);
void Stop(I2C_HandleTypeDef *hi2c);
void LittleTurnRight(I2C_HandleTypeDef *hi2c);
void LittleTurnLeft(I2C_HandleTypeDef *hi2c);
void TurnRight(I2C_HandleTypeDef *hi2c);
void TurnLeft(I2C_HandleTypeDef *hi2c);
void FineTuneTurn(I2C_HandleTypeDef *hi2c,int *magDir,float *mag);
void TurnToNorth(I2C_HandleTypeDef *hi2c, float *mag);
void TurnDirection(I2C_HandleTypeDef *hi2c, int direction, float *mag, int *magDir);

#endif /* BRETTS_HELPER_INC_MOTOR_TEST_H_ */

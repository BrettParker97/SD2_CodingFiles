/*
 * motor_test.c
 *
 *  Created on: Apr 8, 2022
 *      Author: Brett
 */

#include "motor_test.h"
#include "stm32F4xx_hal.h"
#include "helpful_i2c.h"
#include "CONSTANTS.h"
#include "MPU9250.h"
static const uint8_t GPIO_expander = 0x20 << 1;

void LittleTurnRight(I2C_HandleTypeDef *hi2c)
{
	WrByte(hi2c, GPIO_expander, GPIO_write, 0x55);
	HAL_Delay(50);
	Stop(hi2c);
	HAL_Delay(10);
}

void LittleTurnLeft(I2C_HandleTypeDef *hi2c)
{
	WrByte(hi2c, GPIO_expander, GPIO_write, 0xAA);
	HAL_Delay(50);
	Stop(hi2c);
	HAL_Delay(10);
}

void TurnRight(I2C_HandleTypeDef *hi2c)
{
	WrByte(hi2c, GPIO_expander, GPIO_write, 0x55);
	HAL_Delay(50);
}

void TurnLeft(I2C_HandleTypeDef *hi2c)
{
	WrByte(hi2c, GPIO_expander, GPIO_write, 0xAA);
	HAL_Delay(50);
}

void MoveForward(I2C_HandleTypeDef *hi2c)
{
	WrByte(hi2c, GPIO_expander, GPIO_write, 0xA5);
	HAL_Delay(50);
}
void Stop(I2C_HandleTypeDef *hi2c)
{
	WrByte(hi2c, GPIO_expander, GPIO_write, GPIO_All_Off);
	HAL_Delay(50);
}

void TurnToNorth(I2C_HandleTypeDef *hi2c, float *mag)
{
	// if we are already pointing north, return
	float ans = HARDCODE_0 - mag[0];
	if ( ans >= -5 && ans <= 5)
		return;

	// turn right till pointing north +-5
	TurnRight(hi2c);
	while (1)
	{
		IMU_getAccurateMag(hi2c, mag);

		float ans = HARDCODE_0 - mag[0] >= -5;
		if ( ans >= -5 && ans <= 5)
		{
			break;
		}
	}
}
void FineTuneTurn(I2C_HandleTypeDef *hi2c, int *magDir, float *mag)
{
	int wantedMagVal;
	switch (*magDir)
	{
	case 0:
		wantedMagVal = HARDCODE_0;
		break;
	case 1:
		wantedMagVal = HARDCODE_1;
		break;
	case 2:
		wantedMagVal = HARDCODE_2;
		break;
	case 3:
		wantedMagVal = HARDCODE_3;
		break;
	case 4:
		wantedMagVal = HARDCODE_4;
		break;
	case 5:
		wantedMagVal = HARDCODE_5;
		break;
	case 6:
		wantedMagVal = HARDCODE_6;
		break;
	case 7:
		wantedMagVal = HARDCODE_7;
		break;
	}

	// assuming we atleast got close to our direction
	// small turn till you match the mag value
	HAL_Delay(10);
	IMU_getAccurateMag(hi2c, mag);
	if (*magDir >= 0 && *magDir <= 4)
	{
		if (mag[0] > wantedMagVal + 5)
		{
			int littleDir = 0;
			while(1)
			{
				if (mag[0]<wantedMagVal - 5)
					littleDir = 1;
				else
					littleDir = 0;

				switch(littleDir)
				{
					case 0:
						LittleTurnLeft(hi2c);
						break;
					case 1:
						LittleTurnRight(hi2c);
						break;
				}

				// check to break
				IMU_getAccurateMag(hi2c, mag);
				if (mag[0] <= wantedMagVal + 5)
					break;
			}
		}
		else
		{
			int littleDir = 0;
			while(1)
			{
				if (mag[0]<wantedMagVal - 5)
					littleDir = 1;
				else
					littleDir = 0;
				switch(littleDir)
				{
				case 0:
					LittleTurnLeft(hi2c);
					break;
				case 1:
					LittleTurnRight(hi2c);
					break;
				}

				// check to break
				IMU_getAccurateMag(hi2c, mag);
				if (mag[0] >= wantedMagVal - 5)
				break;
			}
		}
	}
	else
	{
		int littleDir = 0;
		if (mag[0] > wantedMagVal + 5)
		{
			while(1)
			{
				if (mag[0]<wantedMagVal + 5)
					littleDir = 1;
				else
					littleDir = 0;
				switch(littleDir)
				{
				case 0:
					LittleTurnLeft(hi2c);
					break;
				case 1:
					LittleTurnRight(hi2c);
					break;
				}

				// check to break
				IMU_getAccurateMag(hi2c, mag);
				if (mag[0] <= wantedMagVal + 5)
				break;
			}
		}
		else if (mag[0] < wantedMagVal - 5)
		{
			int littleDir = 0;
			while(1)
			{
				if (mag[0]<wantedMagVal - 5)
					littleDir = 1;
				else
					littleDir = 0;
				switch(littleDir)
				{
				case 0:
					LittleTurnRight(hi2c);
					break;
				case 1:
					LittleTurnLeft(hi2c);
					break;
				}

				// check to break
				IMU_getAccurateMag(hi2c, mag);
				if (mag[0] >= wantedMagVal - 5)
				break;
			}
		}
	}
}

void TurnDirection(I2C_HandleTypeDef *hi2c, int direction, float *mag, int *magDir)
{
	// if 0, we are already pointing that way
	if (direction == 0)
		return;

	// turn right
	int time = 950; // time to turn 45°
	if (direction >= 1 && direction <= 4)
	{
		// new magDir
		*magDir += direction;
		if (*magDir > 7)
			*magDir -= 8;

		// start turning
		TurnRight(hi2c);

		// wait for X time
		HAL_Delay(time * direction);
		Stop(hi2c);

		// fine tune to what expect
		//FineTuneTurn(hi2c, magDir, mag);
	}
	else
	{
		// new magDir
		*magDir += direction;
		if (*magDir > 7)
			*magDir -= 8;

		int count = 0;
		switch (direction)
		{
		case 7:
			count = 1;
			break;
		case 6:
			count = 2;
			break;
		case 5:
			count = 3;
			break;
		}

		// start turning
		TurnLeft(hi2c);

		// wait for X time
		HAL_Delay(time * count);
		Stop(hi2c);

		// fine tune to what expect
		//FineTuneTurn(hi2c, magDir, mag);
	}
}



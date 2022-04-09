/*
 * OuterRing.h
 *
 *  Created on: Mar 30, 2022
 *      Author: Brett
 */

#include <stdbool.h>
#include "stm32F4xx_hal.h"

int OuterRing_init(float *initalHitDis);

int OuterRing_control(int distances[], int reason, bool initalDetachHit,
		float initalHitDis, int *magDir, float *mag, I2C_HandleTypeDef *hi2c2);

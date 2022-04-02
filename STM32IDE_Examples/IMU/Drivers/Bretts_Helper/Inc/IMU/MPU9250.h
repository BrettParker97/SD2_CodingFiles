

#include "main.h"
#include <stdbool.h>
#include "stm32F4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include "helpful_i2c.h"

#define GYRO_XOUT_H_AD 0x43
#define PWR_MGMT_1_AD 0x6B
#define GYRO_CONFIG_AD 0x1B
#define ACCEL_CONFIG_1_AD 0x1C
#define ACCEL_CONFIG_2_AD 0x1D
#define CONFIG_AD 0x1A
#define INT_BYPASS_CONFIG_AD 0x37
#define CNTL1_AD 0x0A
#define ASAX_AD 0x10
#define ACCEL_XOUT_H_AD 0x3B
#define DATA_READY_MASK 0x01
#define MAGIC_OVERFLOW_MASK 0x8
#define STATUS_1_AD 0x02
#define STATUS_2_AD 0x09
#define HXL_AD 0x03
#define ACCEL_SENS 8192.0f
#define GYRO_SENS 131.0f
#define SCALE 0.1499f  // 4912/32760 uT/tick

int IMU_init();

int IMU_getData(I2C_HandleTypeDef *hi2c2, float *gyro, float *accel, float *mag);

int IMU_getGyroData(I2C_HandleTypeDef *hi2c2, float *gyro);

int IMU_getAccelData(I2C_HandleTypeDef *hi2c2, float *accel);

int IMU_getMagData(I2C_HandleTypeDef *hi2c2, float *mag);

int calibration_GetMag(I2C_HandleTypeDef *hi2c2, int16_t *mag);

void magcalMPU9250(I2C_HandleTypeDef *hi2c2, float * dest1, float * dest2);

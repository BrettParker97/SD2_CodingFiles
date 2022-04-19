/*
 * MPU9250.c
 *
 *  Created on: Apr 2, 2022
 *      Author: Brett
 */

#include "MPU9250.h"

// i2c addresses
static const uint8_t MPU9250_AD = 0x68 << 1;
static const uint8_t MAG_AD = 0x0C << 1;

// specs for mpu
static const float MPU9250mRes = 10.*4912./32760.0;

// saved from calibration
static const float d1[3] = {12, 108, 304};
static const float d2[3] = {1.497, .795, .930599};

float asax;
float asay;
float asaz;

int IMU_init(I2C_HandleTypeDef *hi2c2)
{
	int status = 0;
	uint8_t sensAdjust[3];

	// setup clock
	// set the clock reference to X axis gyroscope to get a better accuracy
	status = WrByte(hi2c2, MPU9250_AD, PWR_MGMT_1_AD, 0x01);

	// setup gyro and accel
	// Set the accel scale to 4g
	status = WrByte(hi2c2, MPU9250_AD, ACCEL_CONFIG_1_AD, 0x08);
	// Set the gyro scale to 500 °/s and FCHOICE_B
	status = WrByte(hi2c2, MPU9250_AD, GYRO_CONFIG_AD, 0x08);
	// Turn on the internal low-pass filter for accelerometer with 10.2Hz bandwidth
	status = WrByte(hi2c2, MPU9250_AD, ACCEL_CONFIG_2_AD, 0x05);
	// Turn on the internal low-pass filter for gyroscope with 10Hz bandwidth
	status = WrByte(hi2c2, MPU9250_AD, CONFIG_AD, 0x05);

	// setup the magnetometer
	// turn on the bypass multiplexer
	status = WrByte(hi2c2, MPU9250_AD, INT_BYPASS_CONFIG_AD, 0x02);

	// setup the Magnetometer to fuse ROM access mode to get the Sensitivity Adjustment values and 16-bit output
	status = WrByte(hi2c2, MAG_AD, CNTL1_AD, 0x1F);
	HAL_Delay(100);  //wait for the mode changes

	//read the Sensitivity Adjustment values
	status = ReadMulti(hi2c2, MAG_AD, ASAX_AD, &sensAdjust[0], 3);
	asax = (float)(sensAdjust[0] -128)/256. + 1.;
	asay = (float)(sensAdjust[1] -128)/256. + 1.;
	asaz = (float)(sensAdjust[2] -128)/256. + 1.;

	//reset the Magnetometer to power down mode
	status = WrByte(hi2c2, MAG_AD, CNTL1_AD, 0x00);
	HAL_Delay(100);  //wait for the mode changes

	//set the Magnetometer to continuous mode 2（100Hz) and 16-bit output
	status = WrByte(hi2c2, MAG_AD, CNTL1_AD, 0x16);
	//wait for the mode changes
	HAL_Delay(100);

	return status;
}

int IMU_getData(I2C_HandleTypeDef *hi2c2, float *gyro, float *accel, float *mag)
{
	int status = 0;
	status = IMU_getGyroData(hi2c2, gyro);
	status = IMU_getAccelData(hi2c2, accel);
	status = IMU_getMagData(hi2c2, mag);
	return status;

	// DEBUG
	// differnt attempts to calibrate
//	printf(" MAGNE_X1: %f\n\r", (magneX*asax*SCALE));
//	printf(" MAGNE_X1: %f\n\r", (MPU9250mRes*asax));
//	printf(" MAGNE_X1: %f\n\r", (dest1[0]));
//	printf(" MAGNE_X1: %f\n\r", (magneX - dest1[0]) * MPU9250mRes*asax);
//	printf(" MAGNE_X1: %i\n\r", (int)((magneX - (MPU9250mRes*asax*dest1[0]))));
//	printf(" MAGNE_X: %i\n\r", (int)(magneX*MPU9250mRes*asax - dest1[0]));
//	printf(" MAGNE_Y: %i\n\r", (int)(magneY*MPU9250mRes*asay - dest1[1]));
//	printf(" MAGNE_Z: %i\n\r", (int)(magneZ*MPU9250mRes*asaz - dest1[2]));
}

int IMU_getGyroData(I2C_HandleTypeDef *hi2c2, float *gyro)
{
	int status = 0;
	uint8_t gyroBuf[6];
	status = ReadMulti(hi2c2, MPU9250_AD, GYRO_XOUT_H_AD, &gyroBuf[0], 6);
	if (status != 0)
		return -1;
	int16_t tempGyro [3];
	tempGyro[0] = gyroBuf[0] | (gyroBuf[1]<<8);
	tempGyro[1] = gyroBuf[2] | (gyroBuf[3]<<8);
	tempGyro[2] = gyroBuf[4] | (gyroBuf[5]<<8);
	gyro[0] = tempGyro[0] / GYRO_SENS;
	gyro[1] = tempGyro[1] / GYRO_SENS;
	gyro[2] = tempGyro[2] / GYRO_SENS;
	return status;
}

int IMU_getAccelData(I2C_HandleTypeDef *hi2c2, float *accel)
{
	int status = 0;
	uint8_t accelBuf[6];
	status = ReadMulti(hi2c2, MPU9250_AD, ACCEL_XOUT_H_AD, &accelBuf[0], 6);
	if (status != 0)
		return -2;
	int16_t tempAccel [3];
	tempAccel[0] = accelBuf[0] | (accelBuf[1]<<8);
	tempAccel[1] = accelBuf[2] | (accelBuf[3]<<8);
	tempAccel[2] = accelBuf[4] | (accelBuf[5]<<8);
	accel[0] = tempAccel[0] / ACCEL_SENS;
	accel[1] = tempAccel[1] / ACCEL_SENS;
	accel[2] = tempAccel[2] / ACCEL_SENS;
	return status;
}

int IMU_getAccurateMag(I2C_HandleTypeDef *hi2c2, float *mag)
{
	float magData[3];
	for (int i = 0; i < 40; i++)
	{
		IMU_getMagData(hi2c2, mag);
		magData[0] += mag[0];
		magData[1] += mag[1];
		magData[2] += mag[2];
		HAL_Delay(10);
	}

	mag[0] = magData[0]/40;
	mag[1] = magData[1]/40;
	mag[2] = magData[2]/40;
	return 0;
}

int IMU_getMagData(I2C_HandleTypeDef *hi2c2, float *mag)
{
	int status = 0;
	uint8_t magBuff[6];
	uint8_t DRDYbit = 0;
	uint8_t magicbit = 0;

	// wait for data to be ready
	while(!DRDYbit)
	{
		status = RdByte(hi2c2, MAG_AD, STATUS_1_AD, &DRDYbit);
		if (DRDYbit && DATA_READY_MASK)
			break;
		printf("mag data not ready\n\r");
		HAL_Delay(10);
	}

	status = ReadMulti(hi2c2, MAG_AD, HXL_AD, &magBuff[0], 6);
	if (status != 0)
		return -3;
	status = RdByte(hi2c2, MAG_AD, STATUS_2_AD, &magicbit);
	if (!(magicbit && MAGIC_OVERFLOW_MASK))
		return -4;

	int16_t tempMag [3];
	tempMag[0] = magBuff[0] | (magBuff[1]<<8);
	tempMag[1] = magBuff[2] | (magBuff[3]<<8);
	tempMag[2] = magBuff[4] | (magBuff[5]<<8);
	mag[0] = ((float)tempMag[0]-d1[0])*MPU9250mRes*asax;
	mag[1] = ((float)tempMag[1]-d1[1])*MPU9250mRes*asay;
	mag[2] = ((float)tempMag[2]-d1[2])*MPU9250mRes*asaz;
	return status;
}

int calibration_GetMag(I2C_HandleTypeDef *hi2c2, int16_t *mag)
{
	int status = 0;
	uint8_t magBuff[6];
	uint8_t DRDYbit = 0;
	uint8_t magicbit = 0;

	// wait for data to be ready
	while(!DRDYbit)
	{
		status = RdByte(hi2c2, MAG_AD, STATUS_1_AD, &DRDYbit);
		if (DRDYbit && DATA_READY_MASK)
			break;
		printf("mag data not ready\n\r");
		HAL_Delay(10);
	}

	status = ReadMulti(hi2c2, MAG_AD, HXL_AD, &magBuff[0], 6);
	if (status != 0)
		return -3;
	status = RdByte(hi2c2, MAG_AD, STATUS_2_AD, &magicbit);
	if (magicbit && MAGIC_OVERFLOW_MASK)
		return -4;

	mag[0] = magBuff[0] | ((int16_t)magBuff[1]<<8);
	mag[1] = magBuff[2] | ((int16_t)magBuff[3]<<8);
	mag[2] = magBuff[4] | ((int16_t)magBuff[5]<<8);

	return status;
}

void magcalMPU9250(I2C_HandleTypeDef *hi2c2, float * dest1, float * dest2)
{
	uint16_t ii = 0, sample_count = 0;
	int32_t mag_bias[3] = {0, 0, 0}, mag_scale[3] = {0, 0, 0};
	int16_t mag_max[3] = {-32767, -32767, -32767}, mag_min[3] = {32767, 32767, 32767}, mag_temp[3] = {0, 0, 0};
	uint8_t MPU9250Mmode = 0x06;

	printf("Mag Calibration: Wave device in a figure eight until done!\n\r");
	HAL_Delay(4000);

	// shoot for ~fifteen seconds of mag data
	if(MPU9250Mmode == 0x02) sample_count = 128;  // at 8 Hz ODR, new mag data is available every 125 ms
	if(MPU9250Mmode == 0x06) sample_count = 3000;  // at 100 Hz ODR, new mag data is available every 10 ms
	for(ii = 0; ii < sample_count; ii++)
	{
		int16_t magBuff [6];
		calibration_GetMag(hi2c2, &magBuff[0]);
		mag_temp[0] = (int16_t)(magBuff[0] | (magBuff[1]<<8));
		mag_temp[1] = (int16_t)(magBuff[2] | (magBuff[3]<<8));
		mag_temp[2] = (int16_t)(magBuff[4] | (magBuff[5]<<8));
		for (int jj = 0; jj < 3; jj++)
		{
			if(mag_temp[jj] > mag_max[jj]) mag_max[jj] = mag_temp[jj];
			if(mag_temp[jj] < mag_min[jj]) mag_min[jj] = mag_temp[jj];
		}
		if(MPU9250Mmode == 0x02) HAL_Delay(135);  // at 8 Hz ODR, new mag data is available every 125 ms
		if(MPU9250Mmode == 0x06) HAL_Delay(12);  // at 100 Hz ODR, new mag data is available every 10 ms
	}

	// Get hard iron correction
	mag_bias[0]  = (mag_max[0] + mag_min[0])/2;  // get average x mag bias in counts
	mag_bias[1]  = (mag_max[1] + mag_min[1])/2;  // get average y mag bias in counts
	mag_bias[2]  = (mag_max[2] + mag_min[2])/2;  // get average z mag bias in counts

	dest1[0] = (float) mag_bias[0];  // save mag biases in G for main program
	dest1[1] = (float) mag_bias[1];
	dest1[2] = (float) mag_bias[2];

	// Get soft iron correction estimate
	mag_scale[0]  = (mag_max[0] - mag_min[0])/2;  // get average x axis max chord length in counts
	mag_scale[1]  = (mag_max[1] - mag_min[1])/2;  // get average y axis max chord length in counts
	mag_scale[2]  = (mag_max[2] - mag_min[2])/2;  // get average z axis max chord length in counts

	float avg_rad = mag_scale[0] + mag_scale[1] + mag_scale[2];
	avg_rad /= 3.0;

	dest2[0] = avg_rad/((float)mag_scale[0]);
	dest2[1] = avg_rad/((float)mag_scale[1]);
	dest2[2] = avg_rad/((float)mag_scale[2]);

	printf("Mag Calibration done!\n\r");
}

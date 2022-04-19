/*
 * helpful_i2c.c
 *
 *  Created on: Apr 2, 2022
 *      Author: Brett
 */


#include "helpful_i2c.h"

uint8_t _I2CBuffer[64];

int WrByte(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t index, uint8_t data) {
	int status = 0;

    _I2CBuffer[0] = index;
    _I2CBuffer[1] = data;

    status = Brett_I2CWrite(hi2c, DevAddress, _I2CBuffer, 2);

    return status;
}

int Brett_I2CWrite(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pdata, uint32_t count) {
	int status = 0;

	status = 0;
    int i2c_time_out = 10+ count* 1;

    status = HAL_I2C_Master_Transmit(hi2c, DevAddress, pdata, count, i2c_time_out);

    return status;
}

int RdByte(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t index, uint8_t *data) {
	int status = 0;

	status = Brett_I2CWrite(hi2c, DevAddress, &index, 1);
    if(status != 0)
    	goto done;

    status = Brett_I2CRead(hi2c, DevAddress, data, 1);

done:
    return status;
}

int Brett_I2CRead(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pdata, uint32_t count) {
	int status = 0;

	status = 0;
    int i2c_time_out = 10+ count* 1;

    status = HAL_I2C_Master_Receive(hi2c, DevAddress|1, pdata, count, i2c_time_out);

    return status;
}

// the ranging_sensor_comms.dll will take care of the page selection
int ReadMulti(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t index, uint8_t *pdata, uint32_t count) {
	int status = 0;

    status = Brett_I2CWrite(hi2c, DevAddress, &index, 1);
    if (status != 0)
        goto done;

    status = Brett_I2CRead(hi2c, DevAddress, pdata, count);

done:
    return status;
}

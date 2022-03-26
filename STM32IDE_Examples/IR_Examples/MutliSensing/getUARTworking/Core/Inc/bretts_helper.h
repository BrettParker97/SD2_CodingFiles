

// includes
#include "stm32f4xx_hal.h"
#include "vl53l0x_api.h"

// prototypes
VL53L0X_API VL53L0X_Error Brett_VL53L0X_StartMultiSensing(VL53L0X_DEV Dev);

VL53L0X_API VL53L0X_Error Brett_VL53L0X_FinishMultiSensing(VL53L0X_DEV Dev,
		VL53L0X_RangingMeasurementData_t *pRangingMeasurementData);

int Brett_IR_StartMulti(VL53L0X_Dev_t Devs[], int distances[], int howManyDevs);

int Brett_IR_InitIrSensor(VL53L0X_Dev_t devs[], I2C_HandleTypeDef *i2c,int howManyDevs);

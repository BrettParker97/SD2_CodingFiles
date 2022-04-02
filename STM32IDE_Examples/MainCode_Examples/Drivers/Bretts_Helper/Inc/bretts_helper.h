

// includes
#include "stm32f4xx_hal.h"
#include "vl53l0x_api.h"
#include "NoObject.h"
#include "CONSTANTS.h"
#include "OuterRing.h"
#include "InnerRing.h"
#include "MPU9250.h"

// prototypes
VL53L0X_API VL53L0X_Error Brett_VL53L0X_StartMultiSensing(VL53L0X_DEV Dev);

VL53L0X_API VL53L0X_Error Brett_VL53L0X_FinishMultiSensing(VL53L0X_DEV Dev,
		VL53L0X_RangingMeasurementData_t *pRangingMeasurementData);

int Brett_IR_StartMulti(VL53L0X_Dev_t Devs[], int distances[], int howManyDevs);

int Brett_IR_InitIrSensors(VL53L0X_Dev_t devs[], I2C_HandleTypeDef *i2c,int howManyDevs);

int Brett_IR_StartSingle(VL53L0X_DEV Dev, int *distance);

// -1 timing budget means default
int Brett_IR_InitIrSensor(VL53L0X_DEV Dev, I2C_HandleTypeDef *i2c, int mode, int timingBudget);

// from datasheet
// time budget = 200ms
// application = precise measurement
VL53L0X_API VL53L0X_Error Brett_IR_SetHighSpeed(VL53L0X_DEV Dev, int timingBudget);

// from datasheet
// time budget = 20ms
// application = high speed where, accuracy is not priority
VL53L0X_API VL53L0X_Error Brett_IR_SetHighAcc(VL53L0X_DEV Dev, int timingBudget);

// from datasheet
// time budget = 33ms
// application = long ranging, only for dark conditions
VL53L0X_API VL53L0X_Error Brett_IR_SetLongRange(VL53L0X_DEV Dev, int timingBudget);

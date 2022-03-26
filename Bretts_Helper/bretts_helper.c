





// includes
#include "bretts_helper.h"

// vars

//functions
VL53L0X_Error Brett_VL53L0X_StartMultiSensing(VL53L0X_DEV Dev)
{
	VL53L0X_Error Status = VL53L0X_ERROR_NONE;

	Status = VL53L0X_SetDeviceMode(Dev, VL53L0X_DEVICEMODE_SINGLE_RANGING);

	if (Status == VL53L0X_ERROR_NONE)
		Status = VL53L0X_StartMeasurement(Dev);

	return Status;
}

VL53L0X_Error Brett_VL53L0X_FinishMultiSensing(VL53L0X_DEV Dev,
		VL53L0X_RangingMeasurementData_t *pRangingMeasurementData)
{
	VL53L0X_Error Status = VL53L0X_ERROR_NONE;
	VL53L0X_DeviceModes DeviceMode;


	// make sure sensor is done
	if (Status == VL53L0X_ERROR_NONE)
		Status = VL53L0X_measurement_poll_for_completion(Dev);

	/* Get Current DeviceMode */
	Status = VL53L0X_GetDeviceMode(Dev, &DeviceMode);

	/* Change PAL State in case of single ranging or single histogram */
	if (Status == VL53L0X_ERROR_NONE
		&& DeviceMode == VL53L0X_DEVICEMODE_SINGLE_RANGING)
		PALDevDataSet(Dev, PalState, VL53L0X_STATE_IDLE);

	if (Status == VL53L0X_ERROR_NONE)
		Status = VL53L0X_GetRangingMeasurementData(Dev,
			pRangingMeasurementData);


	if (Status == VL53L0X_ERROR_NONE)
		Status = VL53L0X_ClearInterruptMask(Dev, 0);

	return Status;
}

int Brett_IR_StartMulti(VL53L0X_Dev_t devs[], int distances[], int howManyDevs)
{
	int bretts_status = 0;
	VL53L0X_Error Status = VL53L0X_ERROR_NONE;
	VL53L0_DEV Dev;

	// first start each dev measuring
	for (int i = 0; i < howManyDevs; i++)
	{
		// set point to a dev
		Dev = &devs[i];

		// start sensing
		Status = VL53L0X_SetDeviceMode(Dev, VL53L0X_DEVICEMODE_SINGLE_RANGING);
		Status = Brett_VL53L0X_StartMultiSensing(Dev);

		// return if error happened
		if (Status != VL53L0X_ERROR_NONE)
			return bretts_status = -1;
	}

	// finally wait for each dev to finish, then store data received
	for (int i = 0; i < howManyDevs; i++)
	{
		// data struct IRs use
		VL53L0X_RangingMeasurementData_t RangingMeasurementData;

		// point to a dev
		Dev = &devs[i];

		// get data from sensor
		Status = Brett_VL53L0X_FinishMultiSensing(Dev, &RangingMeasurementData);

		// if range status isnt 0 then assume max distance
		if (RangingMeasurementData.RangeStatus != 0)
			distances[i] = 8100;
		else
			distances[i] = RangingMeasurementData.RangeMilliMeter;

		// return if error happened
		if (Status != VL53L0X_ERROR_NONE)
			return bretts_status = -2;
	}

	return bretts_status;
}

// !!FUCNTION NOT COMPLETE YET!!
// uses random GPIO
int Brett_IR_InitIrSensor(VL53L0X_Dev_t devs[], I2C_HandleTypeDef *i2c, int howManyDevs)
{
	VL53L0X_Error status = VL53L0X_ERROR_NONE;
	uint32_t refSpadCount;
	uint8_t isApertureSpads;
	uint8_t VhvSettings;
	uint8_t PhaseCal;
	uint8_t newi2c = 0x52;
	int bretts_status = 0;

	for (int i = 0; i < howManyDevs; i++)
	{
		// point to dev
		VL53L0_DEV	Dev;
		Dev = &devs[i];

		// FOR TESTING ONLY
		// turn on XSHUT
		if (i == 0)
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_Delay(10);

		// general dev inits
		Dev->comms_type =  1;
		Dev->I2cDevAddr = 0x52;
		Dev->I2cHandle = i2c;
		Dev->comms_speed_khz = 100;

		// i2c address setting
		newi2c = Dev->I2cDevAddr + (i + 1) * 2;
		status = VL53L0X_SetDeviceAddress(Dev, newi2c);
		if (status != VL53L0X_ERROR_NONE)
			return bretts_status = -3;
		Dev->I2cDevAddr = newi2c;

		// base init for all IR devs
		status = VL53L0X_WaitDeviceBooted( Dev );
		status = VL53L0X_DataInit( Dev );
		status = VL53L0X_StaticInit( Dev );
		status = VL53L0X_PerformRefCalibration(Dev, &VhvSettings, &PhaseCal);
		status = VL53L0X_PerformRefSpadManagement(Dev, &refSpadCount, &isApertureSpads);
		status = VL53L0X_SetDeviceMode(Dev, VL53L0X_DEVICEMODE_SINGLE_RANGING);

		// we can break this out later
		// init for long ranging mode
		status = VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
		status = VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
		status = VL53L0X_SetLimitCheckValue(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, (FixPoint1616_t)(0.1*65536));
		status = VL53L0X_SetLimitCheckValue(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, (FixPoint1616_t)(60*65536));
		status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(Dev, 33000);
		status = VL53L0X_SetVcselPulsePeriod(Dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, 18);
		status = VL53L0X_SetVcselPulsePeriod(Dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 14);

		// one last status check
		if (status != VL53L0X_ERROR_NONE)
			return bretts_status = -4;
	}
	return bretts_status;
}







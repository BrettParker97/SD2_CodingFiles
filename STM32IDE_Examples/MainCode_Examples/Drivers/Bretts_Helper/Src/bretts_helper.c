





// includes
#include "bretts_helper.h"
#include "OuterRing.h"
#include "CONSTANTS.h"

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
int Brett_IR_InitIrSensors(VL53L0X_Dev_t devs[], I2C_HandleTypeDef *i2c, int howManyDevs)
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

		// turn on XSHUT
		if (i == 0)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
			if (bretts_status != 0)
				printf("GPIO turn on 0 didnt work\n\r");
		}
		else if (i == 1)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
			if (bretts_status != 0)
				printf("GPIO turn on 1 didnt work\n\r");
		}
		else if (i == 2)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
			if (bretts_status != 0)
				printf("GPIO turn on 2 didnt work\n\r");
		}
		else if (i == 3)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
			if (bretts_status != 0)
				printf("GPIO turn on 3 didnt work\n\r");
		}
		else if (i == 4)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
			if (bretts_status != 0)
				printf("GPIO turn on 4 didnt work\n\r");
		}
		else if (i == 5)
		{
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
			if (bretts_status != 0)
				printf("GPIO turn on 5 didnt work\n\r");
		}
		else if (i == 6)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
			if (bretts_status != 0)
				printf("GPIO turn on 6 didnt work\n\r");
		}
		else if (i == 7)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
			if (bretts_status != 0)
				printf("GPIO turn on 7 didnt work\n\r");
		}
		else
		{
			printf("this shouldnt happen in GPIO_expander area\n\r");
		}
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


int Brett_IR_StartSingle(VL53L0X_DEV Dev, int *distance)
{
	int bretts_status = 0;
	VL53L0X_Error Status = VL53L0X_ERROR_NONE;
	VL53L0X_RangingMeasurementData_t RangingMeasurementData;

	// start sensing
	Status = VL53L0X_PerformSingleRangingMeasurement(Dev, &RangingMeasurementData);

	// if range status isnt 0 then assume max distance
	if (RangingMeasurementData.RangeStatus != 0)
		*distance = 8100;
	else
		*distance = (int)RangingMeasurementData.RangeMilliMeter;

	// return if error happened
	if (Status != VL53L0X_ERROR_NONE)
		return bretts_status = -2;

	return bretts_status;
}

int Brett_IR_InitIrSensor(VL53L0X_DEV Dev, I2C_HandleTypeDef *i2c, int mode, int timingBudget)
{
	VL53L0X_Error status = VL53L0X_ERROR_NONE;
	uint32_t refSpadCount;
	uint8_t isApertureSpads;
	uint8_t VhvSettings;
	uint8_t PhaseCal;
	int bretts_status = 0;

	// default timing budget
	if (timingBudget == -1)
	{
		if (mode == 1)
			timingBudget = 20000;
		else if (mode == 2)
			timingBudget = 200000;
		else if (mode == 3)
			timingBudget = 33000;
		else
		{
			// do nothing
		}
	}


	// general dev inits
	Dev->comms_type =  1;
	Dev->I2cDevAddr = 0x52;
	Dev->I2cHandle = i2c;
	Dev->comms_speed_khz = 100;

	// base init for all IR devs
	status = VL53L0X_WaitDeviceBooted( Dev );
	status = VL53L0X_DataInit( Dev );
	status = VL53L0X_StaticInit( Dev );
	status = VL53L0X_PerformRefCalibration(Dev, &VhvSettings, &PhaseCal);
	status = VL53L0X_PerformRefSpadManagement(Dev, &refSpadCount, &isApertureSpads);
	status = VL53L0X_SetDeviceMode(Dev, VL53L0X_DEVICEMODE_SINGLE_RANGING);

	// init based on mode
	if (mode == 1)					// high speed range
		Brett_IR_SetHighSpeed(Dev, timingBudget);
	else if (mode == 2)				// high accuracy range
		Brett_IR_SetHighAcc(Dev, timingBudget);
	else if (mode == 3)				// long range
		Brett_IR_SetLongRange(Dev, timingBudget);
	else
	{
		// do nothing 				// default mode
	}

	// one last status check
	if (status != VL53L0X_ERROR_NONE)
		return bretts_status = -4;

	return bretts_status;
}

VL53L0X_Error Brett_IR_SetHighSpeed(VL53L0X_DEV Dev, int timingBudget)
{
	VL53L0X_Error status = VL53L0X_ERROR_NONE;

	status = VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
	status = VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
	status = VL53L0X_SetLimitCheckValue(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, (FixPoint1616_t)(0.25*65536));
	status = VL53L0X_SetLimitCheckValue(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, (FixPoint1616_t)(32*65536));
	status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(Dev, timingBudget);

	return status;
}

VL53L0X_Error Brett_IR_SetHighAcc(VL53L0X_DEV Dev, int timingBudget)
{
	VL53L0X_Error status = VL53L0X_ERROR_NONE;

	status = VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
	status = VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
	status = VL53L0X_SetLimitCheckValue(Dev ,VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, (FixPoint1616_t)(0.25*65536));
	status = VL53L0X_SetLimitCheckValue(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, (FixPoint1616_t)(18*65536));
	status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(Dev, timingBudget);

	return status;
}

VL53L0X_Error Brett_IR_SetLongRange(VL53L0X_DEV Dev, int timingBudget)
{
	VL53L0X_Error status = VL53L0X_ERROR_NONE;

	status = VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
	status = VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
	status = VL53L0X_SetLimitCheckValue(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, (FixPoint1616_t)(0.1*65536));
	status = VL53L0X_SetLimitCheckValue(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, (FixPoint1616_t)(60*65536));
	status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(Dev, timingBudget);
	status = VL53L0X_SetVcselPulsePeriod(Dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, 18);
	status = VL53L0X_SetVcselPulsePeriod(Dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 14);

	return status;
}










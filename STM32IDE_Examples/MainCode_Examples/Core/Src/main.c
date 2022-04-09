/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

#include "main.h"
#include "stm32F4xx_hal.h"
#include "vl53l0x_api.h"
#include "bretts_helper.h"

I2C_HandleTypeDef hi2c2;

RTC_HandleTypeDef hrtc;

UART_HandleTypeDef huart2;

int brett_status = 0;
VL53L0X_Dev_t dev[AMOUNT_IRS];
VL53L0X_DEV Devs = &dev[0];
int distances[AMOUNT_IRS];
float initalHitDis;
bool initalDetachHit;
uint timer_IR;
uint timer_GPS;
static const uint8_t GPIO_expander = 0x20 << 1;
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
Brett_Timer timeStruct;
float gyro[3];
float accel[3];
float mag[3];
int magDir;

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, 0xFFFF);
	return ch;
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	HAL_Init();

	SystemClock_Config();

	MX_GPIO_Init();
	MX_I2C2_Init();
	MX_USART2_UART_Init();
	MX_RTC_Init();

	Stop(&hi2c2);
	Brett_GetTime(&hrtc, &timeStruct);

	// loop 1 init GPS and run pre launch loop

	// loop 2 breakout

	// init all sensors
	// init IR sensors
	brett_status = 0;
	brett_status = Brett_IR_InitIrSensors(dev, &hi2c2, AMOUNT_IRS);
	if (brett_status != 0)
		printf("IR init failed\n\r");

	// do a full multi sense to initalize distances
	for (int i = 0; i < AMOUNT_IRS; i++)
	{
	  brett_status = 0;
	  brett_status = Brett_IR_StartMulti(dev, distances, AMOUNT_IRS);
	  if (brett_status != 0)
		  printf("first multi sense bad\n\r");
	}

	// init IMU
	IMU_init(&hi2c2);

//	while(1)
//	{
//		IMU_getAccurateMag(&hi2c2, mag);
//		printf("%f\n\r",mag[0]);
//	}

	// TODO: init GPS

	// TODO: init antenna
	// TODO: init video

	// start IR multi sense so we can get the data when timer is up
	for (int i = 0; i < AMOUNT_IRS; i++)
	{
		brett_status = 0;
		brett_status = Brett_VL53L0X_StartMultiSensing(&Devs[i]);
		if (brett_status != 0)
		  printf("start of second multi sense bad\n\r");
	}

	// init update timers
	Brett_GetTime(&hrtc, &timeStruct);
	timer_IR = timeStruct.time_in_ms + DELAY_IR;
	timer_GPS = timeStruct.time_in_ms + DELAY_GPS;

	// turn towards north (this wont work for now)
	IMU_getAccurateMag(&hi2c2, mag);
	//TurnToNorth(&hi2c2, mag);
	magDir = 0;

	// start loop 3
	MoveForward(&hi2c2);
	brett_status = Loop3_NoObject();
}

int Loop3_NoObject()
{
	while (1)
	{
		// grab current clock time
		Brett_GetTime(&hrtc, &timeStruct);
		uint currentTime = timeStruct.time_in_ms;

		// check GPS
		if (timer_GPS < currentTime)
		{
			// update GPS
			// TODO: Write GPS stuff here

			// update gps timer
			timer_GPS = currentTime + DELAY_GPS;
		}

		// check IRs
		if (timer_IR < currentTime)
		{
			// update IRs
			VL53L0X_RangingMeasurementData_t tempRanges[AMOUNT_IRS];
			for (int i = 0; i < AMOUNT_IRS; i++)
			{
				brett_status = 0;
				brett_status = Brett_VL53L0X_FinishMultiSensing(&Devs[i], &tempRanges[i]);
				if (brett_status != 0)
				{
					Stop(&hi2c2);
					while(1)
					{
						// do a full multi sense to initalize distances
						for (int i = 0; i < AMOUNT_IRS; i++)
						{
						  brett_status = 0;
						  brett_status = Brett_IR_StartMulti(dev, distances, AMOUNT_IRS);
						  if (brett_status != 0)
						  {
							  printf("back up multi sense bad%d\n\r", i);
							  break;
						  }
						}
						if (!brett_status)
							break;
					}
					MoveForward(&hi2c2);
					printf("start of second multi sense bad %d\n\r", i);
				}

				if (tempRanges[i].RangeStatus != 0)
					distances[i] = 8100;
				else
					distances[i] = tempRanges[i].RangeMilliMeter;
			}
			for (int i = 0; i < AMOUNT_IRS; i++)
			{
				brett_status = 0;
				brett_status = Brett_VL53L0X_StartMultiSensing(&Devs[i]);
				if (brett_status != 0)
				  printf("start of second multi sense bad\n\r");
			}

			// update IR timer
			timer_IR = currentTime + DELAY_IR;

			// check if object is in front of us
			bool objectInOuterRing = false;
			if (distances[SENSOR_FRONT] < RANGE_OUTER)
				objectInOuterRing = true;
			if (objectInOuterRing)
			{
				Stop(&hi2c2);
				// init
				OuterRing_init(&initalHitDis);

				// loop
				int res = 0;
				int reason = 1;
				while (1)
				{
					// control
					res = OuterRing_control(distances, reason, initalDetachHit, initalHitDis, &magDir, mag, &hi2c2);


					// res == 1 returns to no object loop
					if (res == 1)
						break;

					// Loop4
					reason = Loop4_OuterLoop(&reason);
					Stop(&hi2c2);
				}
			}
		}
	}
	return 0;
}

int Loop4_OuterLoop(int *reason)
{
	// update IRs
	VL53L0X_RangingMeasurementData_t tempRanges[AMOUNT_IRS];
	for (int i = 0; i < AMOUNT_IRS; i++)
	{
		brett_status = 0;
		brett_status = Brett_VL53L0X_FinishMultiSensing(&Devs[i], &tempRanges[i]);
		if (brett_status != 0)
		{
			Stop(&hi2c2);
			printf("start of second multi sense bad\n\r");
		}
		for (int i = 0; i < AMOUNT_IRS; i++)
		{
			brett_status = 0;
			brett_status = Brett_VL53L0X_StartMultiSensing(&Devs[i]);
			if (brett_status != 0)
			  printf("start of second multi sense bad\n\r");
		}

		if (tempRanges[i].RangeStatus != 0)
			distances[i] = 8100;
		else
			distances[i] = tempRanges[i].RangeMilliMeter;
	}

	MoveForward(&hi2c2);
	while (1)
	{
		// grab current clock time
		Brett_GetTime(&hrtc, &timeStruct);
		uint currentTime = timeStruct.time_in_ms;

		// check GPS
		if (timer_GPS < currentTime)
		{
			// update GPS
			// TODO: Write GPS stuff here

			// update gps timer
			timer_GPS = currentTime + DELAY_GPS;
		}

		// check IRs
		if (timer_IR < currentTime)
		{
			// update IRs
			tempRanges[AMOUNT_IRS];
			for (int i = 0; i < AMOUNT_IRS; i++)
			{
				brett_status = 0;
				brett_status = Brett_VL53L0X_FinishMultiSensing(&Devs[i], &tempRanges[i]);
				if (brett_status != 0)
				{
					Stop(&hi2c2);
					printf("start of second multi sense bad\n\r");
				}
				for (int i = 0; i < AMOUNT_IRS; i++)
				{
					brett_status = 0;
					brett_status = Brett_VL53L0X_StartMultiSensing(&Devs[i]);
					if (brett_status != 0)
					  printf("start of second multi sense bad\n\r");
				}

				if (tempRanges[i].RangeStatus != 0)
					distances[i] = 8100;
				else
					distances[i] = tempRanges[i].RangeMilliMeter;
			}

			// update IR timer
			timer_IR = currentTime;

			// check if there is an object in inner ring
			//TODO: test this last
//			bool objectInInnerRing = false;
//			int hitDir;
//			for (int i = 0; i < AMOUNT_IRS; i++)
//			{
//				if (distances[i] < RANGE_INNER)
//				{
//					objectInInnerRing = true;
//					hitDir = i;
//					break;
//				}
//			}

			// if there is something in inner ring
			// call inner loop
//			if (objectInInnerRing)
//			{
//				// init
//
//				// control
//				InnerRing_control(hitDir);
//
//				//loop
//				Loop5_InnerRing(hitDir);
//			}
		}

		// check for initalDetachHit
		if (!initalDetachHit)
		{
			// check detach sensor
			if (distances[SENSOR_DETACH] < RANGE_OUTER)
				initalDetachHit = true;
		}

		// check for objects infront of us
		if (distances[SENSOR_FRONT] < RANGE_OUTER)
			return 1;

		// check if we detached
		if (initalDetachHit)
		{
			if (distances[SENSOR_DETACH] > RANGE_OUTER)
						return 2;
		}
	}

	// maybe make a saftey exit
	// like 10-20sec then revert to main loop
	return 0;
}

int Loop5_InnerRing(int hitDir)
{
	while(1)
	{
		// grab current clock time
		time_t currentTime = clock();

		// check GPS
		if (timer_GPS + DELAY_GPS >= currentTime)
		{
			// update GPS
			// TODO: Write GPS stuff here

			// update gps timer
			timer_GPS = currentTime;
		}

		// check IRs
		if (timer_IR + DELAY_IR >= currentTime)
		{
			// update IRs
			VL53L0X_RangingMeasurementData_t tempRanges[AMOUNT_IRS];
			for (int i = 0; i < AMOUNT_IRS; i++)
			{
				brett_status = 0;
				brett_status = Brett_VL53L0X_FinishMultiSensing(&Devs[i], &tempRanges[i]);
				if (brett_status != 0)
				  printf("start of second multi sense bad\n\r");

				if (tempRanges[i].RangeStatus != 0)
					distances[i] = 8100;
				else
					distances[i] = tempRanges[i].RangeMilliMeter;
			}

			// update IR timer
			timer_IR = currentTime;
		}

		// move until X distance away from inner hit
		// TODO: 50mm is a random number, test this
		if (distances[hitDir] > RANGE_INNER + 50)
		{
			return 0;
		}
	}
	return 0;
}



/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  //RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

static void MX_RTC_Init(void)
{

	/* USER CODE BEGIN RTC_Init 0 */
	RTC_TimeTypeDef time = {0};
	RTC_DateTypeDef date = {0};
	/* USER CODE END RTC_Init 0 */

	/* USER CODE BEGIN RTC_Init 1 */

	/* USER CODE END RTC_Init 1 */
	/** Initialize RTC Only
	*/
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
	Error_Handler();
	}
	/* USER CODE BEGIN RTC_Init 2 */
	time.Hours = 0;
	time.Minutes = 0;
	time.Seconds = 0;
	time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	time.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}
	date.WeekDay = RTC_WEEKDAY_WEDNESDAY;
	date.Month = RTC_MONTH_APRIL;
	date.Date = 6;
	date.Year = 22;

	if (HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE END RTC_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
						  |GPIO_PIN_10, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

	/*Configure GPIO pin : B1_Pin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);



	/*Configure GPIO pins : LD2_Pin PA7 PA8 PA9
						   PA10 */
	GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
						  |GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : PC7 */
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : PB4 PB5 PB6 */
	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}


/* USER CODE BEGIN 4 */



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


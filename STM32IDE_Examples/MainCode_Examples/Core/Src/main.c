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
#include <Time.h>

#include "main.h"
#include "stm32F4xx_hal.h"
#include "vl53l0x_api.h"
#include "bretts_helper.h"

I2C_HandleTypeDef hi2c2;

UART_HandleTypeDef huart2;

int brett_status = 0;
int distances[AMOUNT_IRS];
float initalHitDis;
bool initalDetachHit;
Time_t timer_IR = startTime;
Time_t timer_GPS = startTime;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_USART2_UART_Init(void);

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



  // loop 1 init GPS and run pre launch loop

  // loop 2 breakout

  // loop 3 main loop, no object
  // TODO: init sensors
  // TODO: init antenna
  // TODO: init video

  // do a full multi sense to initalize distances
  // TODO: send multiple Devs into multi sense and not 1 at a time
  // Brett_VL53L0X_StartMultiSensing(VL53L0X_DEV Dev)
  // TODO: sned multipl Devs and Distances not 1 at a time
  // VL53L0X_Error Brett_VL53L0X_FinishMultiSensing(VL53L0X_DEV Dev, VL53L0X_RangingMeasurementData_t *pRangingMeasurementData);

  // init update timers
  Time_t startTime = clock();
  timer_IR = startTime;
  timer_GPS = startTime;

  // start IR multi sense so we can get the data when timer is up
  // Brett_VL53L0X_StartMultiSensing(VL53L0X_DEV Dev)

  // start loop 3
  brett_status = Loop3_NoObject();
}

int Loop3_NoObject()
{
	// grab current clock time
	Time_t currentTime = clock();

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
		// TODO: update args
		// VL53L0X_Error error_IR = Brett_VL53L0X_FinishMultiSensing(VL53L0X_DEV Dev, VL53L0X_RangingMeasurementData_t *pRangingMeasurementData);
		// error_IR = Brett_VL53L0X_StartMultiSensing(VL53L0X_DEV Dev);
		// TODO: check error_IR

		// update IR timer
		timer_IR = currentTime;

		// check if object is in front of us
		bol objectInOuterRing = false;
		if (distaces[SENSOR_FRONT] < RANGE_OUTER)
			objectInOuterRing = true;
		if (objectInOuterRing)
		{
			// init
			OuterRing_init(&initalHitDis);

			// loop
			int res = 0;
			int reason = 1;
			while (1)
			{
				// control
				res = OuterRing_control(distances, reason, initalDetachHit, initalHitDis);

				// res == 1 returns to no object loop
				if (res == 1)
					break;

				// Loop4
				reason = Loop4_OuterLoop(&reason);
			}
		}
	}
}

int Loop4_OuterLoop(int *reason)
{
	while (1)
	{
		// grab current clock time
		Time_t currentTime = clock();

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
			// TODO: update args
			// VL53L0X_Error error_IR = Brett_VL53L0X_FinishMultiSensing(VL53L0X_DEV Dev, VL53L0X_RangingMeasurementData_t *pRangingMeasurementData);
			// error_IR = Brett_VL53L0X_StartMultiSensing(VL53L0X_DEV Dev);
			// TODO: check error_IR

			// update IR timer
			timer_IR = currentTime;

			// check if there is an object in inner ring
			bol objectInInnerRing = false;
			int hitDir;
			for (int i = 0; i < AMOUNT_IRS; i++)
			{
				if (distances[i] < RANGE_INNER)
				{
					objectInInnerRing = true;
					hitDir = i;
					break;
				}
			}

			// if there is something in inner ring
			// call inner loop
			if (objectInOuterRing)
			{
				// init

				// control
				InnerRing_control(hitDir);

				//loop
				Loop5_InnerRing(hitDir);
			}
		}

		// check for initalDetachHit
		if (!initalDetachHit)
		{
			// check detach sensor
			if (distances[SENSOR_DETACH] < RANGE_OUTER)
				initalDetachHit = true;
		}

		// check for objects infront of us
		if (distnaces[SENSOR_FRONT < RANGE_OUTER])
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
		Time_t currentTime = clock();

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
			// TODO: update args
			// VL53L0X_Error error_IR = Brett_VL53L0X_FinishMultiSensing(VL53L0X_DEV Dev, VL53L0X_RangingMeasurementData_t *pRangingMeasurementData);
			// error_IR = Brett_VL53L0X_StartMultiSensing(VL53L0X_DEV Dev);
			// TODO: check error_IR

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

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


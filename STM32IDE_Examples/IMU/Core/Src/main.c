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
#include "main.h"
#include <stdbool.h>
#include "stm32F4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

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

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
int status;
static const uint8_t MPU9250_AD = 0x68 << 1;
static const uint8_t MAG_AD = 0x0C << 1;
uint8_t _I2CBuffer[64];
float asax;
float asay;
float asaz;

uint8_t sensAdjust[3];
uint8_t gyroBuf[6];
uint8_t accelBuf[6];
uint8_t magBuff[6];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, 0xFFFF);
	return ch;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

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


	// setup clock
	//#define MPU9250_AD 0x68
	//#define PWR_MGMT_1_AD 0x6B

	// set the clock reference to X axis gyroscope to get a better accuracy
	status = WrByte(&hi2c2, MPU9250_AD, PWR_MGMT_1_AD, 0x01);



	// setup gyro and accel

    // Set the accel scale to 4g
    status = WrByte(&hi2c2, MPU9250_AD, ACCEL_CONFIG_1_AD, 0x08);

    // Set the gyro scale to 500 °/s and FCHOICE_B
    status = WrByte(&hi2c2, MPU9250_AD, GYRO_CONFIG_AD, 0x08);

    // Turn on the internal low-pass filter for accelerometer with 10.2Hz bandwidth
    status = WrByte(&hi2c2, MPU9250_AD, ACCEL_CONFIG_2_AD, 0x05);

    // Turn on the internal low-pass filter for gyroscope with 10Hz bandwidth
    status = WrByte(&hi2c2, MPU9250_AD, CONFIG_AD, 0x05);



    // setup the magnetometer

    // turn on the bypass multiplexer
    status = WrByte(&hi2c2, MPU9250_AD, INT_BYPASS_CONFIG_AD, 0x02);


    // setup the Magnetometer to fuse ROM access mode to get the Sensitivity Adjustment values and 16-bit output
    status = WrByte(&hi2c2, MAG_AD, CNTL1_AD, 0x1F);
    HAL_Delay(100);  //wait for the mode changes


    //read the Sensitivity Adjustment values
	status = ReadMulti(&hi2c2, MAG_AD, ASAX_AD, &sensAdjust[0], 3);
	asax = (sensAdjust[0] -128)*0.5/128+1;
	asay = (sensAdjust[1] -128)*0.5/128+1;
	asaz = (sensAdjust[2] -128)*0.5/128+1;

	//reset the Magnetometer to power down mode
	status = WrByte(&hi2c2, MAG_AD, CNTL1_AD, 0x00);
	HAL_Delay(100);  //wait for the mode changes

	//set the Magnetometer to continuous mode 2（100Hz) and 16-bit output
	status = WrByte(&hi2c2, MAG_AD, CNTL1_AD, 0x16);
	//wait for the mode changes
	HAL_Delay(100);

	float dest1[3] = {29, -8, -200};
	float dest2[3] = {1.497, .795, .930599};
	float MPU9250mRes = 10.*4912./32760.0;


	//magcalMPU9250(&dest1[0],&dest2[0]);
	printf("dest1x= %f, dest1y= %f, dest1z= %f\n\r", dest1[0], dest1[1], dest1[2]);
	printf("dest2x= %f, dest2y= %f, dest2z= %f\n\r", dest2[0], dest2[1], dest2[2]);

	while(1)
	{
		// read gyro
		GetGyroData();
		uint16_t gyroX = gyroBuf[0] | (gyroBuf[1]<<8);
		uint16_t gyroY = gyroBuf[2] | (gyroBuf[3]<<8);
		uint16_t gyroZ = gyroBuf[4] | (gyroBuf[5]<<8);

		// read accel
		GetAccelData();
		uint16_t accelX = accelBuf[0] | (accelBuf[1]<<8);
		uint16_t accelY = accelBuf[2] | (accelBuf[3]<<8);
		uint16_t accelZ = accelBuf[4] | (accelBuf[5]<<8);

		// read magnetometer
		GetMagData();
		int16_t magneX = (int16_t)(magBuff[0] | (magBuff[1]<<8));
		int16_t magneY = (int16_t)(magBuff[2] | (magBuff[3]<<8));
		int16_t magneZ = (int16_t)(magBuff[4] | (magBuff[5]<<8));

		// print everything
		printf(" GYRO_X: %i\n\r", (int)(gyroX/GYRO_SENS));
		printf(" GYRO_Y: %i\n\r", (int)(gyroY/GYRO_SENS));
		printf(" GYRO_Z: %i\n\r", (int)(gyroZ/GYRO_SENS));
		printf(" ACCEL_X: %i\n\r", (int)(accelX/ACCEL_SENS));
		printf(" ACCEL_Y: %i\n\r", (int)(accelY/ACCEL_SENS));
		printf(" ACCEL_Z: %i\n\r", (int)(accelZ/ACCEL_SENS));
		printf(" MAGNE_X1: %f\n\r", (magneX*asax*SCALE));
		printf(" MAGNE_X1: %f\n\r", (MPU9250mRes*asax));
		printf(" MAGNE_X1: %f\n\r", (dest1[0]));
		printf(" MAGNE_X1: %f\n\r", (magneX - dest1[0]) * MPU9250mRes*asax);
		printf(" MAGNE_X1: %i\n\r", (int)((magneX - (MPU9250mRes*asax*dest1[0]))));
		printf(" MAGNE_X: %i\n\r", (int)(magneX*MPU9250mRes*asax - dest1[0]));
		printf(" MAGNE_Y: %i\n\r", (int)(magneY*MPU9250mRes*asay - dest1[1]));
		printf(" MAGNE_Z: %i\n\r", (int)(magneZ*MPU9250mRes*asaz - dest1[2]));
		printf("\n\r");

		// delay 1 sec
		HAL_Delay(1000);
	}
}

void magcalMPU9250(float * dest1, float * dest2)
{
	uint16_t ii = 0, sample_count = 0;
	int32_t mag_bias[3] = {0, 0, 0}, mag_scale[3] = {0, 0, 0};
	int16_t mag_max[3] = {-32767, -32767, -32767}, mag_min[3] = {32767, 32767, 32767}, mag_temp[3] = {0, 0, 0};
	uint8_t MPU9250Mmode = 0x06;

	printf("Mag Calibration: Wave device in a figure eight until done!\n\r");
	HAL_Delay(4000);

	// shoot for ~fifteen seconds of mag data
	if(MPU9250Mmode == 0x02) sample_count = 128;  // at 8 Hz ODR, new mag data is available every 125 ms
	if(MPU9250Mmode == 0x06) sample_count = 1500;  // at 100 Hz ODR, new mag data is available every 10 ms
	for(ii = 0; ii < sample_count; ii++)
	{
		GetMagData();
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

int GetGyroData()
{
	status = 0;
	status = ReadMulti(&hi2c2, MPU9250_AD, GYRO_XOUT_H_AD, &gyroBuf[0], 6);

	return status;
}

int GetAccelData()
{
	status = 0;
	status = ReadMulti(&hi2c2, MPU9250_AD, ACCEL_XOUT_H_AD, &accelBuf[0], 6);

	return status;
}

int GetMagData()
{
	status = 0;
	uint8_t DRDYbit = 0;
	uint8_t magicbit = 0;

	// wait for data to be ready
	while(!DRDYbit)
	{
		status = RdByte(&hi2c2, MAG_AD, STATUS_1_AD, &DRDYbit);
		if (DRDYbit && DATA_READY_MASK)
			break;
		printf("mag data not ready\n\r");
		HAL_Delay(10);
	}

	status = ReadMulti(&hi2c2, MAG_AD, HXL_AD, &magBuff[0], 6);
	if (status)
		return status;
	status = RdByte(&hi2c2, MAG_AD, STATUS_2_AD, &magicbit);
	if (magicbit && MAGIC_OVERFLOW_MASK)
		return 2;
	return status;
}

int WrByte(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t index, uint8_t data) {

    _I2CBuffer[0] = index;
    _I2CBuffer[1] = data;

    status = _I2CWrite(hi2c, DevAddress, _I2CBuffer, 2);

    return status;
}

int _I2CWrite(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pdata, uint32_t count) {

	status = 0;
    int i2c_time_out = 10+ count* 1;

    status = HAL_I2C_Master_Transmit(hi2c, DevAddress, pdata, count, i2c_time_out);

    return status;
}

int RdByte(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t index, uint8_t *data) {

	status = _I2CWrite(hi2c, DevAddress, &index, 1);
    if(status != 0)
    	goto done;

    status = _I2CRead(hi2c, DevAddress, data, 1);

done:
    return status;
}

int _I2CRead(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pdata, uint32_t count) {

	status = 0;
    int i2c_time_out = 10+ count* 1;

    status = HAL_I2C_Master_Receive(hi2c, DevAddress|1, pdata, count, i2c_time_out);

    return status;
}

// the ranging_sensor_comms.dll will take care of the page selection
int ReadMulti(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t index, uint8_t *pdata, uint32_t count) {

    status = _I2CWrite(hi2c, DevAddress, &index, 1);
    if (status != 0)
        goto done;

    status = _I2CRead(hi2c, DevAddress, pdata, count);

done:
    return status;
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


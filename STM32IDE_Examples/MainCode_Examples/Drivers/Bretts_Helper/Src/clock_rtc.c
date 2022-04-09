/*
 * clock_rtc.c
 *
 *  Created on: Apr 7, 2022
 *      Author: Brett
 */

#include <clock_rtc.h>
#include <stdio.h>
#include <inttypes.h>
#include "stm32F4xx_hal.h"
#include "clock_rtc.h"

void Brett_GetTime(RTC_HandleTypeDef *hrtc, Brett_Timer *timeStruct)
{
	// get time
	HAL_RTC_GetTime(hrtc, &timeStruct->sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(hrtc, &timeStruct->sDate, RTC_FORMAT_BIN);

	// check that the registers are updated correctly
	if (timeStruct->sTime.SubSeconds < timeStruct->lastSub && timeStruct->lastSec == timeStruct->sTime.Seconds)
		timeStruct->time_in_ms = ((timeStruct->sTime.Minutes * 60) + timeStruct->sTime.Seconds + 1 + ((float)timeStruct->sTime.SubSeconds / (float)timeStruct->sTime.SecondFraction)) * 1000;
	else
		timeStruct->time_in_ms = ((timeStruct->sTime.Minutes * 60) + timeStruct->sTime.Seconds + ((float)timeStruct->sTime.SubSeconds / (float)timeStruct->sTime.SecondFraction)) * 1000;

	// save data for next getTime call
	timeStruct->lastSub = timeStruct->sTime.SubSeconds;
	timeStruct->lastSec = timeStruct->sTime.Seconds;
}

//printf("Time: %02d.%02d.%02d.%" PRIu32 "\r\n",
//	timeStruct.sTime.Hours,timeStruct.sTime.Minutes,
//	timeStruct.sTime.Seconds,timeStruct.sTime.SubSeconds);



#ifndef BRETTS_HELPER_INC_CLOCK_RTC_H_
#define BRETTS_HELPER_INC_CLOCK_RTC_H_

#include "stm32F4xx_hal.h"
#include <stdio.h>
#include <inttypes.h>

typedef struct {

	RTC_TimeTypeDef sTime;	// time struct for RTC
	RTC_DateTypeDef sDate;	// date struct for RTC
	uint32_t lastSub;		// last subseconds for accuracy
	uint8_t lastSec;		// last seconds for accuracy
	uint time_in_ms;		// holds time in ms

} Brett_Timer;

void Brett_GetTime(RTC_HandleTypeDef *hrtc, Brett_Timer *timeStruct);

#endif

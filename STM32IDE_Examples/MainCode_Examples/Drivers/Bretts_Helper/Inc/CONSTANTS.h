/*
 * CONSTANTS.h
 *
 *  Created on: Mar 30, 2022
 *      Author: Brett
 */

#ifndef BRETTS_HELPER_INC_CONSTANTS_H_
#define BRETTS_HELPER_INC_CONSTANTS_H_


#define DELAY_IR 200 	// us
#define DELAY_GPS 1000 	// ms

#define RANGE_INNER 100 // mm
#define RANGE_OUTER 450 // mm

#define AMOUNT_IRS 4 	// # of devs

#define SENSOR_FRONT 0	// index of sensor
#define SENSOR_DETACH 3 // index of sensor

#define GPIO_read 0x41
#define GPIO_write 0x40

#define GPIO_All_Off 0x00
#define GPIO_P0_On 0x01
#define GPIO_P1_On 0x02
#define GPIO_P2_On 0x04
#define GPIO_P3_On 0x08
#define GPIO_P4_On 0x10
#define GPIO_P5_On 0x20
#define GPIO_P6_On 0x40
#define GPIO_P7_On 0x80

#define HARDCODE_0 17
#define HARDCODE_1 55
#define HARDCODE_2 145
#define HARDCODE_3 162
#define HARDCODE_4 342
#define HARDCODE_5 286
#define HARDCODE_6 160
#define HARDCODE_7 45


#endif /* BRETTS_HELPER_INC_CONSTANTS_H_ */

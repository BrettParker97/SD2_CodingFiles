/*
 * OuterRing.c
 *
 *  Created on: Mar 30, 2022
 *      Author: Brett
 */

#include "OuterRing.h"
#include "CONSTANTS.h"
#include <stdbool.h>
#include "motor_test.h"

// TODO: give ptr to GPS data
int OuterRing_init(float *initalHitDis)
{
	// TODO: Capture our current position

	return 0;
}


int OuterRing_control(int distances[], int reason, bool initalDetachHit,
		float initalHitDis, int *magDir, float *mag, I2C_HandleTypeDef *hi2c2)
{
	// reason == 1 means object ahead
	if (reason == 1)
	{
		// turn to first open position, clockwise
		int direction;
		int backup = 1;	// highest valued direction
		for (int i = 0; i < AMOUNT_IRS; i++)
		{
			// if greater than outer range then its open
			if (distances[i] >= RANGE_OUTER)
			{
				direction = i;
				break;
			}

			// keep tack of farthest reading
			if (distances[i] > backup)
				backup = i;

			// if we didnt find anything just settle for backup
			// this really shoulnt happen
			// TODO: consider printf debug here
			if (i == AMOUNT_IRS - 1)
				direction = backup;
		}

		// turn to choosen direction
		IMU_getAccurateMag(hi2c2, mag);
		TurnDirection(hi2c2, direction, mag, magDir);

		// update initalDetachHit
		if (distances[SENSOR_DETACH] < RANGE_OUTER)
			initalDetachHit = true;
		else
			initalDetachHit = false;

		// return to Loop 3
		return 0;
	}
	else if (reason == 2)
	{
		// test code
		// if detach then turn north and return to main loop
		IMU_getAccurateMag(hi2c2, mag);
		int direction = 0 - *magDir;
		if (direction < 0)
			direction += 8;
		TurnDirection(hi2c2, direction, mag, magDir);
		return 1;



		// check that current distance to home is
		// less than initalHitDis
		// TODO: grab gps data and make distance
		// TODO: compare to initalHitDis
		// return 1 if we are to return to no object mode

		// turn to detach + 1
		// TODO: turn(detach + 1 direction)

		// update initalDetachHit
		if (distances[SENSOR_DETACH] < RANGE_OUTER)
			initalDetachHit = true;
		else
			initalDetachHit = false;

		// return to Loop 3
		return 0;
	}
	else
	{
		// do nothing
	}
	return 0;
}

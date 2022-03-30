/*
 * OuterRing.c
 *
 *  Created on: Mar 30, 2022
 *      Author: Brett
 */

#include "OuterRing.h"
#include "CONSTANTS.h"



// TODO: give ptr to GPS data
int OuterRing_init(int *initalHitDis)
{
	// TODO: Capture our current position

	return 0;
}


int OuterRing_control(int distances[], int reason, bool initalDetachHit, int initalHitDis)
{
	// reason == 1 means object ahead
	if (reason == 1)
	{
		// turn to first open position, clockwise
		int direction;
		int backup = distances[0];	// highest valued direction
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
		//TODO: turn(direction)

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

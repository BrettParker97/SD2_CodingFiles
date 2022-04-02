/*
 * InnerRing.c
 *
 *  Created on: Mar 31, 2022
 *      Author: Brett
 */

#include "InnerRing.h"


int InnerRing_control(int hitDir)
{
	// find direction oposite of hitDir
	int newDir = hitDir + 4;
	if (newDir > 7)
		newDir -= 8;

	// turn towards newDir
	// TODO: turn(newdir)

	// return to go to Loop 5 (inner ring)
	return 0;
}

/*
 * airplane.h
 *
 *  Created on: Mar. 19, 2023
 *      Author: Charlie Huang
 */

#ifndef SRC_AIRPLANE_H_
#define SRC_AIRPLANE_H_

class airplane {
	int ID;
	Vector Position;
	Vector Speed;

	Public:
	bool airplane_entering = false; // bool value will be set to true when the airplane is actually entering the airspace
	bool airplane_exiting = false; // bool value will be set to true when the airplane is actually exiting the airspace
	long timestamp = 0; // time of last update for the airplane
};

#endif /* SRC_AIRPLANE_H_ */

/*
 * airplane.h
 *
 *  Created on: Mar. 23, 2023
 *      Author: kasra
 */

#ifndef AIRPLANE_H_
#define AIRPLANE_H_

#include <stdio.h>
#include <iostream>
#include <time.h>

#include <sync.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <sys/syspage.h>
#include <inttypes.h>
#include <stdint.h>

class airplane {
public:
	//constructor and destructor
	airplane(int id, int arrival_t, int departure_t, int position[3], int speed[3]);
	~airplane();

	bool airplane_entering = false; // bool value will be set to true when the airplane is actually entering the airspace
	bool airplane_exiting = false; // bool value will be set to true when the airplane is actually exiting the airspace
	long timestamp = 0;

	//functions below
	//getters
	int getId() const;
	int getArrival();
	int getDeparture();
	int* getPosition();
	int* getSpeed();
	//setters
	void setPosition(int x, int y, int z);
	void setSpeed(int x, int y, int z);


private:
	int Id;
	int Arrival_t;
	int Departure_t;
	int Position[3];
	int Speed[3];
};

#endif /* AIRPLANE_H_ */

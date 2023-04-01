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
#include <stdlib.h>

#include <sync.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <sys/syspage.h>
#include <inttypes.h>
#include <stdint.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <pthread.h>
#include "System_Specifications.h"

class airplane {
public:
//	constructor and destructor
	airplane(int departure_t, int position[3], int speed[3]);
	~airplane();
//	static std::vector<airplane*> VectorPlanes;
	bool airplane_entering = false; // bool value will be set to true when the airplane is actually entering the airspace
	bool airplane_exiting = false; // bool value will be set to true when the airplane is actually exiting the airspace
	long timestamp = 0;

//	functions below
		//getters
	int getDeparture();
	int* getPosition();
	int* getSpeed();
		//setters
	void* setPosition(int x, int y, int z);
	void* setSpeed(int x, int y, int z);
		//other functions
	void MakeThread(); //Creates the plane thread and launches the thread with the function below
	void* PlaneStart(void); //Function of the thread
	void UpdatePosition(); //Set position to += speed
	void CheckAirspace(); //Checks to see if plane is out of bounds
	void OutputPosition();
	friend void * plane_start_routine(void* arg);
//	friend void * start_routine(void* arg); //start routine for shm

//	data
	int Arrival_t;
	int Departure_t;
	int Position[3];
	int Speed[3];
//	static int threadCounter;

//	Thread
	pthread_t ThreadID;
	pthread_attr_t attr;


//	shm
	pthread_mutex_t plane_mutex;
	int shm_fd;
	void *ptr;
	const char *name = "/my_shm";
	void initialize_airplane();


};

#endif /* AIRPLANE_H_ */

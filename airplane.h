/*
 * airplane.h
 *
 *  Created on: Mar. 23, 2023
 *      Author: kasra
 */
#pragma once

//#include <stdio.h>
//#include <time.h>
//#include <stdlib.h>
//#include <sync.h>
//#include <sys/siginfo.h>
//#include <sys/neutrino.h>
//#include <sys/netmgr.h>
//#include <sys/syspage.h>
//#include <inttypes.h>
//#include <stdint.h>
//#include <sys/types.h>
//#include <unistd.h>

#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include "System_Specifications.h"
#include "Timer.h"

using namespace std;

struct PlaneParams {
	int id;
	double Position[3];
	double Speed[3];

	bool posFlag, speedFlag;
	double cmdPosition[3];
	double cmdSpeed[3];

	void print() {
		cout<<"ID: " << id << " X: "<<Position[0]<<" Y: "<<Position[1]<<" Z:"<<Position[2] << " Speed X: " <<Speed[0]<<"Speed Y: "<<Speed[1]<<"Speed Z:"<<Speed[2] << endl;
	}
};

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
	friend void* plane_start_routine(void* arg);
//	friend void * start_routine(void* arg); //start routine for shm

	void pthreadJoin();

	void WriteToSHM();
	void GetCommand();

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
	string planeName;
	void initialize_airplane();
};


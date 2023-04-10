/*
 * data_display.h
 *
 *  Created on: Mar. 19, 2023
 *      Author: Huang
 */
#ifndef DATA_DISPLAY_H_
#define DATA_DISPLAY_H_

#include <iostream>
#include <string.h>
#include <pthread.h>
#include "System_Specifications.h"
using namespace std;

class data_display{
	public:
		data_display();
		~data_display();
	private:
		pthread_t initializeDataDisplayThread();
		void *dataDisplayBody(void * arg);
		void print_airspace();
		void print_AircraftInfo();
		void print_Warnings();
};

#endif /* DATA_DISPLAY_H_ */

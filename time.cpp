/*
 * time.cpp
 *
 *  Created on: Mar. 29, 2023
 *      Author: Shangirna
 */


#include <iostream>
#include <string.h>
#include <sys/siginfo.h>
#include <time.h>
using namespace std;

class Time {
public:
	Time(int chid){
		coid = ConnectAttach(0,0,chid,0,0);
		if (coid == -1){
			fprintf("Channel not created.");
			exit(EXIT_FAILURE);
		}
		SIGEV_PULSE_INIT(&event,coid, SIGEV_PULSE_PRIO_INHERIT, CODE_TIMER,0);

        if (timer_create(CLOCK_REALTIME, &event, &timer_id) == -1){
            fprintf("Timer not created.");
            exit(EXIT_FAILURE);
        }
	}

    void setTimer(int offset, int period){
        struct itimerspec its;
		its.it_value.tv_sec = offset;
		its.it_value.tv_nsec = 0;
		its.it_interval.tv_sec = period;
		its.it_interval.tv_nsec = 0;

		if (timer_settime(timerid,0,&its, NULL)==-1){
			fprintf("Timer not set.");
		}

    }

};



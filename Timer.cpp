/*
 * time.cpp
 *
 *  Created on: Apr. 7, 2023
 *      Author: Shangirna
 */

#include "Timer.h"

Timer::Timer(uint32_t seconds, uint32_t msec){
	chid = ChannelCreate(0);
	coid = ConnectAttach(0, 0, chid, 0, 0);
	if (coid == -1) {
		printf("Channel not created.");
		exit(EXIT_FAILURE);
	}
	SIGEV_PULSE_INIT(&event, coid, SIGEV_PULSE_PRIO_INHERIT, 1, 0);

	if (timer_create(CLOCK_REALTIME, &event, &timer_id) == -1) {
		printf("Timer not created.");
		exit(EXIT_FAILURE);
	}

	setTimer(seconds, 1000000*msec);
//	cyclesPerSecond = SYSPAGE_ENTRY(qtime)->cyclesPerSecond
}

Timer::~Timer(){
	// TODO Auto-generated destructor stub
}

void Timer::setTimer(uint32_t sec, uint32_t nano) {
	struct itimerspec its;
	its.it_value.tv_sec = sec;
	its.it_value.tv_nsec = nano;
	its.it_interval.tv_sec = sec;
	its.it_interval.tv_nsec = nano;
	timer_settime(timer_id,0,&its,NULL);
}

void Timer::waitTimer() {
	int rcvid;
	rcvid = MsgReceive(chid, NULL, NULL, NULL);
}

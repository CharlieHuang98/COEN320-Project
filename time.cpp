<<<<<<< Updated upstream
/*
 * time.cpp
 *
 *  Created on: Apr. 7, 2023
 *      Author: Shangirna
 */

#include "time.h"

time::time(uint32_t seconds, uint32_t msec){
	chid = ChannelCreate(0)
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
	
	setTimerSpec(sec,1000000*msec);
	cyclesPerSecond = SYSPAGE_ENTRY(qtime)->cyclesPerSecond
}

time::~time(){
	// TODO Auto-generated destructor stub
}

void time::setTimer(uint32_t sec, uint32_t nano) {
	struct itimerspec its;
	its.it_value.tv_sec = sec;
	its.it_value.tv_nsec = nano;
	its.it_interval.tv_sec = sec;
	its.it_interval.tv_nsec = nano;
	timer_settime(timer_id,0,&its,NULL);
}

void time::waitTimer(){
	int rcvid;
	rcvid = MsgReceive(channel_id, &msg_buffer, sizeof(msg_buffer), NULL);
>>>>>>> Stashed changes

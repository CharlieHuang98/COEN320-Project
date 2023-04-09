/*
 * time.h
 *
 *  Created on: Apr. 7, 2023
 *      Author: Shangirna
 */

#ifndef SRC_TIME_H_
#define SRC_TIME_H_

class Time {
	Time(int chid);
	int coid;
	timer_t timer_id;
	struct sigevent event;
	void setTimer(int offset, int period);
};



#endif /* SRC_TIME_H_ */

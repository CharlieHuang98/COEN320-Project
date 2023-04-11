
/*
 * time.h
 *
 *  Created on: Apr. 7, 2023
 *      Author: Shangirna
 */

#ifndef SRC_TIME_H_
#define SRC_TIME_H_

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sync.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <sys/syspage.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>

class Timer {

public:

    Timer(uint32_t sec, uint32_t msec);
    ~Timer();
    void waitTimer();

private:
    timer_t timer_id;
    struct sigevent event;
    int chid;
    int coid;

    void setTimer(uint32_t sec, uint32_t nano);
};


#endif /* SRC_TIME_H_ */

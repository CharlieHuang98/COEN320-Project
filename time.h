<<<<<<< Updated upstream
/*
 * time.h
 *
 *  Created on: Apr. 7, 2023
 *      Author: Shangirna
 */

#ifndef SRC_TIME_H_
#define SRC_TIME_H_





#endif /* SRC_TIME_H_ */
=======
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

class time {
public:
    timer_t timer_id;
    struct sigevent event;
    int coid;

    time(int chid);
    void setTimer(int offset, int period);
};


#endif /* SRC_TIME_H_ */
>>>>>>> Stashed changes

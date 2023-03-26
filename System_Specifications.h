/*
 * System_Specifications.h
 *
 *  Created on: Mar. 23, 2023
 *      Author: Charlie
 */

#ifndef SYSTEM_SPECIFICATIONS_H
#define SYSTEM_SPECIFICATIONS_H

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

#include <unistd.h>

#include <pthread.h>

/*
 * Airspace Boundaries
 */
#define X_VALUE_MINIMUM 0
#define X_VALUE_MAXIMUM 100000
#define Y_VALUE_MINIMUM 0
#define Y_VALUE_MAXIMUM 100000
#define Z_VALUE_MINIMUM 0
#define Z_VALUE_MAXIMUM 25000
#define VALUE_ELEVATION 15000

/*
 * ATC Boundaries
 */
#define X_VALUE_MINIMUM 0
#define X_VALUE_MAXIMUM 100000
#define Y_VALUE_MINIMUM 0
#define Y_VALUE_MAXIMUM 100000
#define Z_VALUE_MINIMUM 0
#define Z_VALUE_MAXIMUM 25000
#define VALUE_ELEVATION 15000

#endif /* SYSTEM_SPECIFICATIONS_H */

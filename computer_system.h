/*
 * computer_system.h
 *
 *  Created on: Mar. 19, 2023
 *      Author: Charlie Huang
 */

#ifndef SRC_COMPUTER_SYSTEM_H_
#define SRC_COMPUTER_SYSTEM_H_

#include <iostream>
#include <string.h>
using namespace std;
#include <pthread.h>
#include "Types.h"

class computerSystem{
public:
    computerSystem();
    ~computerSystem();
    int calculationInterval;
    int clockCycles;
private:
    pthread_t initializeComputerSystemThread();
    void * computerSystemBody(void * arg);
    void updateAirplaneForCompSys(Msg2ComputerSys msg);
    bool MovingAircraftTest(Airplane A, Airplane B, double &t);
};

#endif /* COMPUTERSYSTEM_H_ */"

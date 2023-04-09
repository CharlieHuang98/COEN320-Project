/*
 * operator_control.h
 *
 *  Created on: Mar. 30, 2023
 *      Author: Huang
 */

#ifndef SRC_OPERATOR_CONTROL_H_
#define SRC_OPERATOR_CONTROL_H_


#include <iostream>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/dispatch.h>
#include <pthread.h>

#include "computer_system.h"

typedef struct _pulse msg_header_t;

typedef enum {OperatorSetCalculationInterval, AirplaneRadarUpdate, ClockTimerCompUpdate} MsgToComputerSystemType;

typedef struct{
		msg_header_t hdr;
		MsgToComputerSystemType type;
		int id;
		int calculationInterval;
		double x, y, z;
		double dx, dy, dz;
		long timestamp;
		bool isAirplaneEntering;
		bool isAirplaneLeaving;
} MsgToComputerSystem;

class operator_control {
public:
    operator_control();
    void requestAircraftInfo(int airplane_id, int airplane_speed, int airplane_position);
    void changeAirplaneFlightLevel(int new_flight_level);
    void changeAirplanePosition(int new_position_x, int new_position_y, int new_position_z);
    void changeAirplaneSpeed(int new_speed);

    bool sendMsgToComputer_System(MsgToComputerSystem msg);

};

#endif /* SRC_OPERATOR_CONTROL_H_ */

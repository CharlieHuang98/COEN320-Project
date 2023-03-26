/*
 * operator_control.h
 *
 *  Created on: Mar. 19, 2023
 *      Author: Huang
 */

#ifndef SRC_OPERATOR_CONTROL_H_
#define SRC_OPERATOR_CONTROL_H_

#include <iostream>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "computer_system.h"

class operator_control {
public:
    operator_control();
    void requestAircraftInfo(int airplane_id, int airplane_speed, int airplane_position);
    void changeAirplaneFlightLevel(int new_flight_level);
    void changeAirplanePosition(int new_position_x, int new_position_y, int new_position_z);
    void changeAirplaneSpeed(int new_speed);

};

#endif /* SRC_OPERATOR_CONTROL_H_ */

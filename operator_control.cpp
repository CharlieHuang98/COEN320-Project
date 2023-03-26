/*
 * operator_control.cpp
 *
 *  Created on: Mar. 19, 2023
 *      Author: Huang
 */

#include <iostream>
#include <string.h>
using namespace std;

#include "computer_system.cpp"
#include "operator_control.h"


class OperatorController {
public:

    OperatorController(){
        // add connection to computer system and data display
    }

    void requestAircraftInfo(int airplane_id, int airplane_speed, int airplane_position){
        // send request to computer system to get aircraft ID
        // Update Data Display
    }

    void changeAirplaneFlightLevel(int new_flight_level){
        string command = "change flight level" + to_string(new_flight_level);
        // Send command to computer system to change the airplane's flight level
    }

    void changeAirplanePosition(int new_position_x, int new_position_y, int new_position_z){
        string command = "change position" + to_string(new_position_x) + " " + to_string(new_position_y) + " " + to_string(new_position_z) + "\n";
        // Send command to computer system to change the airplane's position
    }

    void changeAirplaneSpeed(int new_speed){
        string command = "change speed" + to_string(new_speed) + "\n";
        // Send command to computer system to change the airplane's speed
    }
};

enum Available_Commands{
    change_flight_level,
    change_speed,
    change_position,
    more_information,
    invalid
};

int main(){
    OperatorController operator_controller;
    Available_Commands user_input = Available_Commands::invalid;

    /*
     * The first while loop is to change the airplane's speed or position.
     */
    while(true){
        cout << "Enter a command to get or change the airplane's Flight Level, Speed or Position: ";
        string command;
        getline(cin, command);

        if(command == "change position"){
            int new_position_x, new_position_y, new_position_z;
            cout << "Enter a new position (x, y, z): ";
            cin >> new_position_x >> new_position_y >> new_position_z;
            cin.ignore(); // ignore the newline character left in the input buffer
            operator_controller.changeAirplanePosition(new_position_x, new_position_y, new_position_z);
            user_input = Available_Commands::change_position;
        } else if(command == "change speed"){
            int new_speed;
            cout << "Enter a new speed for the airplane: ";
            cin >> new_speed;
            cin.ignore(); // ignore the newline character left in the input buffer
            operator_controller.changeAirplaneSpeed(new_speed);
            user_input = Available_Commands::change_speed;
        } else if(command == "change flight level"){
            int new_flight_level;
            cout << "Enter a new flight level for the airplane: ";
            cin >> new_flight_level;
            cin.ignore(); // ignore the newline character left in the input buffer
            operator_controller.changeAirplaneFlightLevel(new_flight_level);
            user_input = Available_Commands::change_flight_level;
        } else {
            user_input = Available_Commands::invalid;
            cout << "the command entered was invalid" << endl;
        }
    }
    return 0;
}

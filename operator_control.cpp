/*
 * computer_system.cpp
 *
 *  Created on: Mar. 5, 2023
 *      Author:
 *      Charlie Huang
 *
 */


#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

#include "operator_control.h"

class OperatorController {
public:
    OperatorController() {}

    void requestAircraftInfo(int airplane_id, int airplane_speed, int airplane_position){
        string message = "get more aircraft information " + to_string(airplane_id) + " " + to_string(airplane_speed) + " " + to_string(airplane_position) + "\n";
        sendMessageToServer(message);
    }

    void changeAirplaneFlightLevel(int new_flight_level){
        string message = "change flight level " + to_string(new_flight_level) + "\n";
        sendMessageToServer(message);
    }

    void changeAirplanePosition(int new_position_x, int new_position_y, int new_position_z){
        string message = "change position " + to_string(new_position_x) + " " + to_string(new_position_y) + " " + to_string(new_position_z) + "\n";
        sendMessageToServer(message);
    }

    void changeAirplaneSpeed(int new_speed){
        string message = "change speed " + to_string(new_speed) + "\n";
        sendMessageToServer(message);
    }

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
    } else if(command == "more information"){
    	string more_info = "more information";
    	cout << "More information was requested";
    	cin >> more_info;
    	user_input = Available_Commands::more_information;

    	// Send message to server to request more aircraft information
    	MsgToComputerSystem msg;
    	msg.id = MORE_AIRCRAFT_INFO;
    	if (operator_controller.sendMsgToComputer_System(msg)) {
    		cout << "Message sent successfully!" << endl;
    	} else {
    		cout << "Failed to send message to server." << endl;
    	}
    }
    else {
        user_input = Available_Commands::invalid;
        cout << "the command entered was invalid" << endl;
    }

    return 0;
}


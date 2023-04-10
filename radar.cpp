/*
 * radar.cpp
 *
 *  Created on: Mar. 19, 2023
 *      Author: Huang
 */


#include <iostream>
#include <string.h>
using namespace std;

#include "computer_system.cpp"
#include "airplane.cpp"
#include <time.h>
#include <System_Specifications.h>

radar::radar(){
	vector<airplane> planesInbound; // List of planes ready to send message to the radar and is in the
							 // airspace.

	time time;

	// Channel for the timer
	int chid = ChannelCreate(0);
	if(chid == -1){
		printf("Failed to create a channel in Radar.");
		exit(EXIT_FAILIURE);
	}
	time(chid);
	time.setTimer(0,1000);
}

vector<airplane*> radar::scan(){
	// Scans for new aircrafts that are in the airspace and add it to the list
	// Iterate through airplanes
	//for(aiplane a : list of airplanes){
	//    if(check boundary == true){
	//    planesInbound.push_back(a);}
	//   }
	 return planesInbound;
}

void radar::SSR(airplane ap){
	Message msg;
	Message rsp;

	// Creating a message passing channel for communication between the radar and aircraft object thread
	int radarToAircraftCoid = name_open(RADAR_TO_AIRCRAFT_CHANNEL, 0);
	int aircraftToRadarCoid = name_open(AIRCRAFT_TO_RADAR_CHANNEL, 0);

	// Send airplane information to the radar thread

	MsgSend(radarToAircraftCoid, &msg, sizeof(msg), NULL, 0);

	// Wait for response from the radar thread
	struct radar_response_message_t rsp;
	MsgReceive(aircraftToRadarCoid, &rsp, sizeof(rsp), NULL);

	// create an object that stores flight info to send to the Computer system.
}

void radar::infotoCS(computerSystem cs){
	Message msg;
	Message rsp;

	// Creating a message passing channel for communication between the radar and computer system object thread
	int radarToComputerSystemCoid = name_open(RADAR_TO_COMPUTER_SYSTEM_CHANNEL, 0);
	int computerSystemToRadarCoid = name_open(COMPUTER_SYSTEM_TO_RADAR_CHANNEL, 0);

	//Process airplane info
	MsgSend(radarToComputerSystemCoid, &message, sizeof(message), NULL, 0);

	// Wait for response from computer system thread
	struct computer_system_response_message_t response;
	MsgReceive(computerSystemToRadarCoid, &response, sizeof(response), NULL);

}





















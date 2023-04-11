/*
 * ATC.cpp
 *
 *  Created on: Apr. 2, 2023
 *      Author: Charlie Huang
 */


#include <iostream>
#include <sys/dispatch.h>
#include "airplane.h"
//#include "computer_system.h"
//#include "operator_control.h"
#include "communication_system.h"
#include "System_Specifications.h"

void* thread_example(void* arg);

int main(){
    cout << "Welcome to the Air Traffic Control System" << endl;

	int pos[3] = {50000, 50000, 15000};
	int speed[3] = {100, 100, 100};
	airplane a1(10, pos, speed);

	// start other threads too:
//    pthread_t computer_system_thread = initializeComputerSystemThread();
    pthread_t communication_system_thread = InitializeCommunicationSystemThread();

    pthread_t exThread;
    pthread_create(&exThread, NULL, thread_example, NULL);

    // pthread_join(computer_system_thread,NULL);


	a1.pthreadJoin();
    pthread_join(communication_system_thread,NULL);
    pthread_join(exThread, NULL);
    return 0;
}

void* thread_example(void* args) {


	while (1) {
		string s;
		getline(cin, s);

		if (s.find("change speed") != string::npos) {
			CommonMessage msg;
			msg.hdr.type = Command;
			msg.hdr.subtype = Change_Speed;
			string ss = s.substr(12);

			msg.Speed[0] = stoi(ss);
			msg.Speed[1] = stoi(ss);
			msg.Speed[2] = stoi(ss);

			int server_coid; //server connection ID
			if ((server_coid = name_open(COMMUNICATION_CHANNEL, 0)) == -1) {
					perror("Error occurred while attaching the channel");
				}
			if(MsgSend(server_coid, &msg, sizeof(msg), NULL, 0) == -1){
				printf("Error while sending the speed message");
			}
			/* Close the connection */
			name_close(server_coid);
		}
		else if (s.find("change pos") != string::npos) {
			CommonMessage msg;
			msg.hdr.type = Command;
			msg.hdr.subtype = Change_Position;
			string ss = s.substr(10);
			int i;
			try {
				i = stoi(ss);
			}catch (...) {
				i = 0;
			}

			msg.Position[0] = i;
			msg.Position[1] = i;
			msg.Position[2] = i;

			int server_coid; //server connection ID
			if ((server_coid = name_open(COMMUNICATION_CHANNEL, 0)) == -1) {
					perror("Error occurred while attaching the channel");
				}
			if(MsgSend(server_coid, &msg, sizeof(msg), NULL, 0) == -1){
				printf("Error while sending the speed message");
			}
			/* Close the connection */
			name_close(server_coid);
		}
		else if (s.find("get info") != string::npos) {

			CommonMessage msg, reply;
			msg.hdr.type = Command;
			msg.hdr.subtype = Request_Plane_Info;

			int server_coid; //server connection ID

			if ((server_coid = name_open(COMMUNICATION_CHANNEL, 0)) == -1) {
					perror("Error occurred while attaching the channel");
				}

			if (MsgSend(server_coid, &msg, sizeof(msg), &reply, sizeof(reply)) == -1){
				printf("Error while getting the info message");
			}else {

				cout << "Plane ID: " << reply.plane_id << endl;
				cout << "X: " << reply.Position[0] << " Y: " << reply.Position[1] << " Z: " << reply.Position[2] << endl;
				cout << "Speed X: " << reply.Speed[0] << " Speed Y: " << reply.Speed[1] << " Speed Z: " << reply.Speed[2] << endl;
			}

			/* Close the connection */
			name_close(server_coid);

		} else continue;


	}

	return NULL;
}

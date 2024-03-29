/*
 * communication_system.cpp
 *
 *  Created on: Mar. 12, 2023
 *      Author: Huang
 */

#include <iostream>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>
using namespace std;

#include "communication_system.h"
//#include "computer_system.cpp"
#include "System_Specifications.h"
#include "airplane.h"


//#define ATTACH_POINT "communication_system_channel" // change ATTACH_POINT to server name

/* We specify the header as being at least a pulse */
/* All of your messages should start with this header */
/* This struct contains a type/subtype field as the first 4 bytes.
 * The type and subtype field can be used to identify the message that is being received.
/ * This allows you to identify data which isn't destined for your server. */

typedef struct _pulse msg_header_t;

/* Our real data comes after the header */
//typedef struct communication_system_data {
//	msg_header_t hdr;
//	double data;
//	MsgToAirplane type;
//} my_data_t;


/*
 * Initialization of the Communication Systems thread
 */

pthread_t InitializeCommunicationSystemThread(){
    int err_no;
	pthread_t Communication_threadD;
	pthread_attr_t attr;
	int policy;
	int detachstate;

	err_no = pthread_attr_init(&attr);
//	if(err_no)
//		ERROR("ERROR, RC from pthread_attr_init() is %d \n", err_no);

	// create thread with attributes
		err_no = pthread_create(&Communication_threadD, &attr, communication_system_main, NULL);

//	// detach state
	err_no = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
//	if(err_no)
//			ERROR("ERROR, RC from pthread_attr_init() is %d \n", err_no);
//

	return Communication_threadD;
}


/* Server Side of the code
 *
 * There are still a lot of errors that need to be fixed
 */
bool isCompSystemMessageGood(int rcvid, CommonMessage msg){

	/* Do your MsgReceive's here now with the chid */
		/* Server will block in this call, until a client calls MsgSend to send a message to
		 * this server through the channel named "myname", which is the name that we set for the channel,
		 * i.e., the one that we stored at ATTACH_POINT and used in the name_attach call to create the channel. */

		if (rcvid == -1) {/* Error condition, exit */
			return false;
		}

		if (rcvid == 0) {/* Pulse received */
			switch (msg.hdr.code) {
			case _PULSE_CODE_DISCONNECT:
				/*
				 * A client disconnected all its connections (called
				 * name_close() for each name_open() of our name) or
				 * terminated
				 */
				ConnectDetach(msg.hdr.scoid);
				break;
			case _PULSE_CODE_UNBLOCK:
				/*
				 * REPLY blocked client wants to unblock (was hit by
				 * a signal or timed out).  It's up to you if you
				 * reply now or later.
				 */
				break;
			default:
				/*
				 * A pulse sent by one of your processes or a
				 * _PULSE_CODE_COIDDEATH or _PULSE_CODE_THREADDEATH
				 * from the kernel?
				 */
				break;
			}
			return false;
		}

		/* name_open() sends a connect message, must EOK this */
		if (msg.hdr.type == _IO_CONNECT ) {
			MsgReply( rcvid, EOK, NULL, 0 );
			return false;
		}

		/* Some other QNX IO message was received; reject it */
		if (msg.hdr.type > _IO_BASE && msg.hdr.type <= _IO_MAX ) {
			MsgError( rcvid, ENOSYS );
			return false;
		}

		/* Here are the messages that you will work on, i.e., messages that will have a meaning for
		 * your application. Let's assume that you have one message (e.g., data to be displayed) and several subtypes.
		 * Thus, we first test to check if it is a message we expect. Next, we can have a switch that check
		 * what is the subtype of the message. In your project, for instance, you can have a subtype for each
		 * variable, e.g., (0x01 - speed, 0x02 - temperature, 0x03 - gear, and so on...).
		 * Then, based on the subtype the server is receiving, it would display the information
		 * contained in msg.data in the proper place, e.g., at the proper location in a GUI.
		 * You can use that as well to work on the output your thread should provide.
		 *
		 * In addition, you might have another type of message. For instance, you might have a type of message
		 * that would be used for configuration (e.g., type==0x01 used for configuration and type==0x00 for data).
		 * This can be used to implement the mechanism to change the period of your consumer thread (server).
		 * For instance, let's assume that you implemented this server in a separate thread and, instead of having
		 * a forever loop, you implement the thread as a periodic task as we have seen (this is not necessary in this
		 * case because the server will block when waiting for a message, i.e., it will not be hogging CPU.). Then, the
		 * configuratin message could be used to send the period/frequency in which the client would be sending messages.
		 * Thus, the server could consider that while doing other things. */

		if (msg.hdr.type == Command) {
			/*A motion message has been received*/
			if (msg.hdr.subtype == Change_Speed) {
				/* A velocity message has been received  */
				printf("The velocity of the object is  %.2f m/s \n", msg.data);
			}
			else if (msg.hdr.subtype == Change_Position) {
				/* A position message has been received  */
				printf("The x position of the object is  %.2f m \n", msg.data);
				printf("\n");
			}
//			else{
//				MsgError( rcvid, ENOSYS );
//				return false;
//			}
		}

		MsgReply(rcvid, EOK, 0, 0);

	return NULL;
}

// issue here on how to send message to airplane since airplane doesn't have a channel
//static bool SendCommandToAirplane(){
//	int server_coid; //server connection ID
//	const char* AirplaneChannel;
//	CommonMessage msg;
//
//	if ((server_coid = name_open(AirplaneChannel, 0)) == -1) {
//			perror("Error occurred while attaching the channel");
//			return false;
//		}
//
//	if(MsgSend(server_coid, &msg, sizeof(msg), NULL, 0) == -1){
//		printf("Error while sending the position message");
//		return false;
//	}
//
//	/* Close the connection */
//		name_close(server_coid);
//		return true;
//}

void* communication_system_main(void* arg){
	int rcvid;
    name_attach_t *attach;

    /* Create a local name (/dev/name/local/...) */
    if ((attach = name_attach(NULL, COMMUNICATION_CHANNEL, 0)) == NULL) {
 //   	ERROR("Error occurred while creating the channel", CommunicationSystem_Channel);
    	return NULL;
    }

    while (true) {
    		CommonMessage msg;
    		/* Server will block in this call, until a client calls MsgSend to send a message to
    		 * this server through the channel named "myname", which is the name that we set for the channel,
    		 * i.e., the one that we stored at ATTACH_POINT and used in the name_attach call to create the channel. */
    		rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL);

    		if (rcvid == -1) {/* Error condition, exit */
    			break;
    		}
 //   		bool CompSystemMessageGood = isCompSystemMessageGood(rcvid, msg);


    	// here the code should continue to handle the message sent to the airplane but the airplane.cpp doesn't receive messages

    		if(msg.hdr.subtype == Change_Speed){

    			int shm_fd = shm_open("plane2", O_RDWR, 0666);
    			if (shm_fd != -1){
    				ftruncate(shm_fd, sizeof(PlaneParams));
    				void* ptr = mmap(0, sizeof(PlaneParams), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    				if (ptr == MAP_FAILED) {
    					printf("Map failed\n");
    				}

    				// SHARED MEMORY OPEN

    				(*((PlaneParams*)ptr)).speedFlag = true;
    				for (int i = 0; i < 3; i++)
    					(*((PlaneParams*)ptr)).cmdSpeed[i] = msg.Speed[i];

    				shm_unlink("plane2");
    				close(shm_fd);
    			} else {
    				cout << "error with SH" << endl;
    			}

    			MsgReply(rcvid, EOK, 0, 0);

    		}
    		else if(msg.hdr.subtype == Change_Position){

    			int shm_fd = shm_open("plane2", O_RDWR, 0666);
    			if (shm_fd != -1){
    				ftruncate(shm_fd, sizeof(PlaneParams));
    				void* ptr = mmap(0, sizeof(PlaneParams), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    				if (ptr == MAP_FAILED) {
    					printf("Map failed\n");
    				}

    				// SHARED MEMORY OPEN

    				(*((PlaneParams*)ptr)).posFlag = true;
    				for (int i = 0; i < 3; i++)
    					(*((PlaneParams*)ptr)).cmdPosition[i] = msg.Position[i];

    				shm_unlink("plane2");
    				close(shm_fd);
    			} else {
    				cout << "error with SH" << endl;
    			}

    			MsgReply(rcvid, EOK, 0, 0);

    		}
    		else if (msg.hdr.subtype == Request_Plane_Info){
    			CommonMessage reply;

    			int shm_fd = shm_open("plane2", O_RDWR, 777);
    			if (shm_fd != -1){
    				ftruncate(shm_fd, sizeof(PlaneParams));
    				void* ptr = mmap(0, sizeof(PlaneParams), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    				if (ptr == MAP_FAILED) {
    					printf("Map failed\n");
    				}

    				reply.plane_id = (*((PlaneParams*)ptr)).id;
    				for (int i = 0; i < 3; i++) {
    					reply.Position[i] = (*((PlaneParams*)ptr)).Position[i];
    					reply.Speed[i] = (*((PlaneParams*)ptr)).Speed[i];
    				}

    				reply.hdr.type = Command;
    	    		reply.hdr.subtype = Response_Plane_Info;

    				shm_unlink("plane2");
    				close(shm_fd);
    				MsgReply(rcvid, EOK, &reply, sizeof(reply));

    			} else {
    				cout << "error with SH" << endl;
        			MsgReply(rcvid, EOK, 0, 0);

    			}


    		} else{
    			MsgReply(rcvid, EOK, 0, 0);
    		}
    }

    return EXIT_SUCCESS;
}

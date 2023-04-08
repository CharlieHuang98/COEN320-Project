/*
 * computer_system.cpp
 *
 *  Created on: Mar. 19, 2023
 *      Author: Charlie Huang
 */


#include <iostream>
#include <string.h>
#include <pthread.h>
using namespace std;
using namespace computerSystem;

#include "radar.cpp"
#include "communication_system.cpp"
#include "computer_system.h"

computerSystem(){
    calculationInterval = 180;
    clockCycle = 0;
}

pthread_t initializeComputerSystemThread(){
    int rc;
	pthread_t threadD;
	pthread_attr_t attr;
	int policy;
	int detachstate;

	/* Initialize attributes */
	rc = pthread_attr_init(&attr);
	if (rc)
		LOG_ERROR("ERROR, RC from pthread_attr_init() is %d \n", rc);

	/* Set detach state */
	rc = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	if (rc)
		LOG_ERROR("ERROR; RC from pthread_attr_setdetachstate() is %d \n", rc);


	/* Creating thread with constructed attribute object */
	rc = pthread_create(&threadD, &attr, computerSystemBody, NULL);
	if (rc)
		LOG_ERROR("ERROR when creating customized thread; Code is %d \n", rc);


	LOG_INFO("A Computer System thread with customized attributes is created!\n");

	/* Print out dtach state */
	pthread_attr_getdetachstate(&attr, &detachstate);
	LOG_INFO("   Detach state: %s\n",
			(detachstate == PTHREAD_CREATE_DETACHED) ?
					"PTHREAD_CREATE_DETACHED" :
					(detachstate == PTHREAD_CREATE_JOINABLE) ?
							"PTHREAD_CREATE_JOINABLE" : "???");

	/* Print out scheduling policy*/
	pthread_attr_getschedpolicy(&attr, &policy);
	LOG_INFO ("   Scheduling policy: %s\n\n",
			(policy == SCHED_OTHER) ? "SCHED_OTHER" :
					(policy == SCHED_FIFO)	? "SCHED_FIFO"  :
							(policy == SCHED_RR)	? "SCHED_RR" 	:
									"???");

	pthread_attr_destroy(&attr);
	return threadD;
}

void * computerSystemBody(void *arg){
   LOG_INFO("The Computer thread has started\n");

	char CHANNEL_NAME[64] = "CompSysChannel";

	// Create comm sys channel - AirplaneChannel_<id>

	LOG_INFO("Creating Message Server at ChannelName = %s for Computer System\n", CHANNEL_NAME);

	name_attach_t *attach;
	Msg2ComputerSys msg;
	int rcvid;

	/* Create a local name (/dev/name/local/...) */
	if ((attach = name_attach(NULL, CHANNEL_NAME, 0)) == NULL) {
		LOG_ERROR("ERROR, the %s channel couldn't be created \n", CHANNEL_NAME);
		//return EXIT_FAILURE;
		return NULL;
	}

	/* Do your MsgReceive's here now with the chid */
	while (1) {
		/* Server will block in this call, until a client calls MsgSend to send a message to
		 * this server through the channel named "myname", which is the name that we set for the channel,
		 * i.e., the one that we stored at ATTACH_POINT and used in the name_attach call to create the channel. */
		rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL);
		/* In the above call, the received message will be stored at msg when the server receives a message.
		 * Moreover, rcvid */

		if (rcvid == -1) {/* Error condition, exit */
			return NULL; //return false;
		}

		// Check the message type and make sure the message is correctly formated
		// checks if the message is OK, returns true if the message is a valid message
		bool msgOk = isMessageOk(rcvid, msg);

		// skip invalid messages
		if(msgOk == false) {
			continue;
		}

		// TODO: add handling of messages
		/* A message (presumable ours) received, handle */
		LOG_DEBUG("Commputer Sys message server received a message [msg.type=%d]\n",msg.type);

		// comm system forwards messgage by operator to sets airplane speed
		if (msg.type == OperatorSetCalculationInterval) {
			LOG_INFO("\tA message OperatorSetCalculationInterval [msg.type=%d] received\n", msg.type);

			LOG_INFO("Calculation interval N is set to new value %d sec, (old value %d sec)", msg.calculationInterval, calculationInterval);

			calculationInterval = msg.calculationInterval; // the n parameter



			MsgReply(rcvid, EOK, 0, 0);

		// comm system forwards messgage by operator to get airplane data
		} else if (msg.type == AirplaneRadarUpdate){
			LOG_DEBUG("\tA message AirplaneRadarUpdate [msg.type=%d] received for airplane [id=%d]\n", msg.type, msg.id );

			// add airplane information to the atcAirplanes
			updateAirplaneForCompSys(msg);

			MsgReply(rcvid, EOK, 0, 0);

		} else if (msg.type == ClockTimerCompUpdate){
			LOG_DEBUG("\tA message ClockTimerCompUpdate [msg.type=%d] received for airplane [id=%d]\n", msg.type, msg.id );

			if (clockCycles%30 == 0 ) {
				//happens every 30 seconds
				//log to file
			}

			double collisionTime;

			for (size_t i=0; i<atcAirplanes.size(); i++) {
				for ( size_t y=i+1; y<atcAirplanes.size(); y++) {
					bool collision = MovingAirplaneTest(atcAirplanes[i], atcAirplanes[y], collisionTime);

					// display warining message if the colision is present and is within [0, calculationInterval]
					if (collision && ((int)collisionTime <= calculationInterval)) {
						LOG_WARN("Airplanes [id1=%d] and [id2=%d] collide in %.3f seconds\n", atcAirplanes[i].getId(), atcAirplanes[y].getId(), collisionTime);
					}
				}
			}


			if (clockCycles%5 == 0) {
				for (Airplane plane : atcAirplanes) {
					printf("Airplane [id=%d]: %s\n", plane.getId(), plane.getAirplaneInfo().c_str());
				}
			}

			clockCycles++;
			MsgReply(rcvid, EOK, 0, 0);
		} else {
			LOG_DEBUG("\tAn Unknown Message [msg.type=%d] received\n", msg.type);

			MsgReply(rcvid, EOK, 0, 0);
		}
	}

	/* Remove the name from the space */
	name_detach(attach, 0);

	return EXIT_SUCCESS;
}

// update the vector atcAirplanes with new data
void updateAirplaneForCompSys(Msg2ComputerSys msg) {
    bool isNewAirplane = true;

    if (msg.type != AirplaneRadarUpdate) {
        LOG_ERROR("Incorect message type for updateAirplaneForCompSys()\n");
        return;
    }

    // find the corresponding airplane
    for (size_t i=0; i<atcAirplanes.size(); i++) {

        // update the airplane info
        if (atcAirplanes[i].getId() == msg.id) {
            atcAirplanes[i].setPosition(Vector{msg.x, msg.y,  msg.z});
            atcAirplanes[i].setVelocity(Vector{msg.dx, msg.dy,  msg.dz});
            atcAirplanes[i].timestamp = msg.timestamp;
            atcAirplanes[i].isAirplaneEntering = msg.isAirplaneEntering;
            atcAirplanes[i].isAirplaneLeaving = msg.isAirplaneLeaving;
            isNewAirplane = false;
            LOG_INFO("Computer System: Airplane [id=%d] updated with data (x,y,z)=(%f %f %f) (dx,dy,dz)=(%f %f %f) ent=%d lvg=%d t=%d\n",
                    atcAirplanes[i].getId(), atcAirplanes[i].getPosition().x, atcAirplanes[i].getPosition().y, atcAirplanes[i].getPosition().z,
                    atcAirplanes[i].getVelocity().x, atcAirplanes[i].getVelocity().y, atcAirplanes[i].getVelocity().z,
                    atcAirplanes[i].isAirplaneEntering, atcAirplanes[i].isAirplaneLeaving, atcAirplanes[i].timestamp );
        }
    }

    // add new airplane to atcAirplanes
    if(isNewAirplane==true /*&& msg.isAirplaneEntering*/) {
        Airplane plane = Airplane(msg.id, msg.timestamp, Vector{msg.x, msg.y,  msg.z}, Vector{msg.dx, msg.dy,  msg.dz});
        plane.timestamp = msg.timestamp;
        plane.isAirplaneEntering = msg.isAirplaneEntering;
        plane.isAirplaneLeaving = msg.isAirplaneLeaving;
        LOG_INFO("Computer System: Airplane [id=%d] entered with data (x,y,z)=(%f %f %f) (dx,dy,dz)=(%f %f %f) ent=%d lvg=%d t=%d\n",
                plane.getId(), plane.getPosition().x, plane.getPosition().y, plane.getPosition().z,
                plane.getVelocity().x, plane.getVelocity().y, plane.getVelocity().z,
                plane.isAirplaneEntering, plane.isAirplaneLeaving, plane.timestamp );
        atcAirplanes.push_back(plane);
    }

    //remove airplane if airplane isAirplaneLeaving flag is set
    if (msg.isAirplaneLeaving == true) {

        for( std::vector<Airplane>::iterator iter = atcAirplanes.begin(); iter != atcAirplanes.end(); ++iter )
        {
            if( iter->getId() == msg.id )
            {
                LOG_INFO("Computer System: Airplane [id=%d] left with data (x,y,z)=(%f %f %f) (dx,dy,dz)=(%f %f %f) ent=%d lvg=%d t=%d\n",
                        iter->getId(), iter->getPosition().x, iter->getPosition().y, iter->getPosition().z,
                        iter->getVelocity().x, iter->getVelocity().y, iter->getVelocity().z,
                        iter->isAirplaneEntering, iter->isAirplaneLeaving, iter->timestamp );
                atcAirplanes.erase( iter );
                LOG_INFO("Num. Airplanes in the ATC area: %d\n", atcAirplanes.size());
                break;
            }
        }
    } 
}

bool MovingAirplaneTest(Airplane A, Airplane B, double &t){
    Vector s = A.getPosition().subtractVec(B.getPosition()); // vector between the centers of each sphere
    Vector v = A.getVelocity().subtractVec(B.getVelocity()); // relative velocity between spheres
    double r = 1500 + 1500; // pa->radius + pb->radius;

    double c = s.dot(s) - r*r; // if negative, they overlap
    
    if (c < 0.0){ // if true, they already overlap
        t = .0;
        return true;
    }

    float a = v.dot(v);
    float b = v.dot(s);

    if (b >= 0.0){return false}; // does not move towards each other

    float d = b*b - a*c;

    if (d < 0.0){return false;} // no real roots ... no collision

    t = (-b - sqrt(d)) / a;
    return true;

}

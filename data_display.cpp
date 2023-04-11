#include <iostream>
#include <string.h>
#include <pthread.h>
using namespace std;

#include "communication_system.cpp"
#include "Timer.cpp"
#include "data_display.h"

data_display::data_display(){};
data_display::~data_display(){};

pthread_t data_display::initializeDataDisplayThread(){
	int rc;
	pthread_t threadD;
	pthread_attr_t attr;
	int policy;
	int detachstate;

	/* Initialize attributes */
	rc = pthread_attr_init(&attr);

	/* Set detach state */
	rc = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


	/* Creating thread with constructed attribute object */
	rc = pthread_create(&threadD, &attr, dataDisplayBody, NULL);

	LOG_INFO("A Computer System thread with customized attributes is created!\n");

	/* Print out dtach state */
	pthread_attr_getdetachstate(&attr, &detachstate);

	/* Print out scheduling policy*/
	pthread_attr_getschedpolicy(&attr, &policy);

	pthread_attr_destroy(&attr);
	return threadD;
}

void * data_display::dataDisplayBody(void * arg){

	char CHANNEL_NAME[64] = "DataDisplayChannel";

	name_attach_t *attach;
	Msg2DataDisplay msg;
	int rcvid;
	bool airspaceGrid[25][25];
	time timer(5,0);

	/* Create a local name (/dev/name/local/...) */
	if ((attach = name_attach(NULL, CHANNEL_NAME, 0)) == NULL) {
		//return EXIT_FAILURE;
		return NULL;
	}

	/* Do your MsgReceive's here now with the chid */
	while (1) {
		rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL);

		if (rcvid == -1) {/* Error condition, exit */
			return NULL; //return false;
		}
		
		updateAirplaneforDataDisplay(msg);
		if (msg.type == DisplayAirplaneInfo) {
			for (size_t i=0; i<atcAirplanes.size(); i++) {
				// update the airplane info
				if (atcAirplanes[i].id == msg.id) {
					printf("Computer System: Airplane [id=%d] with data (x,y,z)=(%f %f %f) (dx,dy,dz)=(%f %f %f) ent=%d lvg=%d t=%d\n",
							 atcAirplanes[i].id, atcAirplanes[i].getPosition().x, atcAirplanes[i].getPosition().y, atcAirplanes[i].getPosition().z,
							 atcAirplanes[i].getSpeed().x, atcAirplanes[i].getSpeed().y, atcAirplanes[i].getSpeed().z,
							 atcAirplanes[i].airplane_entering, atcAirplanes[i].airplane_exiting, atcAirplanes[i].timestamp);
				MsgReply(rcvid, EOK, 0, 0);

				}
			}
		} else if (msg.type == UpdateTerminal){
			for (unsigned int i = 0; i < 25; i++) {
				for (unsigned int j = 0; j < 25; j++) {
					airspaceGrid[i][j] = false;
				}
			}
			//divide the aircraft coordinates to enter the scaled down grid
			for (size_t i=0; i<atcAirplanes.size(); i++) {
				airspaceGrid[atcAirplanes[i].getPosition().x / 4000][atcAirplanes[i].getPosition().y / 4000] = true;
				}
			printf("============================= AIRSPACE DISPLAY ============================\n");
			for (unsigned int j = 0; j < 25; j++) {
				for (unsigned int i = 0; i < 25; i++) {
					if(airspaceGrid[i][j]) {
						printf(" 0 ");
					} else {
						printf(" . ");
					}
				}
				printf("\n");
			}
			printf("==========================================================================\n");

			MsgReply(rcvid, EOK, 0, 0);
		} 
		timer.waitTimer();
	}

	/* Remove the name from the space */
	name_detach(attach, 0);

	return EXIT_SUCCESS;
}

void updateAirplaneforDataDisplay(Msg2DataDisplay msg) {
    bool isNewAirplane = true;

    // find the corresponding airplane
    for (size_t i=0; i<atcAirplanes.size(); i++) {

        // update the airplane info
        if (atcAirplanes[i].id() == msg.id) {
            atcAirplanes[i].setPosition(msg.x, msg.y,  msg.z);
            atcAirplanes[i].setSpeed(msg.dx, msg.dy,  msg.dz);
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
        plane.airplane_entering = msg.airplane_entering;
        plane.airplane_exiting = msg.airplane_exiting;
        atcAirplanes.push_back(plane);
    }

    //remove airplane if airplane isAirplaneLeaving flag is set
    if (msg.airplane_exiting == true) {
        for( std::vector<Airplane>::iterator iter = atcAirplanes.begin(); iter != atcAirplanes.end(); ++iter )
        {
            if( iter->getId() == msg.id )
            {
                atcAirplanes.erase(iter);
                LOG_INFO("Num. Airplanes in the ATC area: %d\n", atcAirplanes.size());
                break;
            }
        }
    } 
}


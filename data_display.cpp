#include <iostream>
#include <string.h>
#include <pthread.h>
using namespace std;

#include "communication_system.cpp"
#include "time.cpp"
#include "cTimer.cpp"

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
	if (rc)
		LOG_ERROR("ERROR, RC from pthread_attr_init() is %d \n", rc);

	/* Set detach state */
	rc = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	if (rc)
		LOG_ERROR("ERROR; RC from pthread_attr_setdetachstate() is %d \n", rc);


	/* Creating thread with constructed attribute object */
	rc = pthread_create(&threadD, &attr, dataDisplayBody, NULL);
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

void * data_display::dataDisplayBody(void * arg){

	char CHANNEL_NAME[64] = "DataDisplayChannel";

	// Create comm sys channel - AirplaneChannel_<id>

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

		bool msgOk = isMessageOk(rcvid, msg);

		if(msgOk == false) {
			continue;
		}

		if (msg.type == DisplayAirplaneInfo) {
			for (size_t i=0; i<atcAirplanes.size(); i++) {
				// update the airplane info
				if (atcAirplanes[i].getId() == msg.id) {
					printf("Computer System: Airplane [id=%d] updated with data (x,y,z)=(%f %f %f) (dx,dy,dz)=(%f %f %f) ent=%d lvg=%d t=%d\n",
							 atcAirplanes[i].getId(), atcAirplanes[i].getPosition().x, atcAirplanes[i].getPosition().y, atcAirplanes[i].getPosition().z,
							 atcAirplanes[i].getVelocity().x, atcAirplanes[i].getVelocity().y, atcAirplanes[i].getVelocity().z,
							 atcAirplanes[i].isAirplaneEntering, atcAirplanes[i].isAirplaneLeaving, atcAirplanes[i].timestamp);
				MsgReply(rcvid, EOK, 0, 0);

				}
			}
		} else if (msg.type == UpdateTerminal){
			for (unsigned int i = 0; i < 25; i++) {
				for (unsigned int j = 0; j < 25; j++) {
					grid[i][j] = false;
				}
			}
			//divide the aircraft coordinates to enter the scaled down grid
			for (size_t i=0; i<atcAirplanes.size(); i++) {
				grid[atcAirplanes[i].getPosition().x / 4000][atcAirplanes[i].getPosition().y / 4000] = true;
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

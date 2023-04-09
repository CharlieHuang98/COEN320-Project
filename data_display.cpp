/*
 * data_display.cpp
 *
 *  Created on: Mar. 19, 2023
 *      Author: Huang
 */


#include "data_display.h"
#include <iostream>
#include <string.h>
using namespace std;

data_display::data_display() {
	displayInitialize(); // Construction Initialization Call
}

data_display::~data_display() {
	shm_unlink("display");
}

int data_display::displayInitialize(){
    int rc = pthread_attr_init(&attr);
	if (rc) {
		printf("ERROR, RC from pthread_attr_init() is %d \n", rc);
	}

	rc = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	if (rc) {
		printf("ERROR; RC from pthread_attr_setdetachstate() is %d \n", rc);
	}

	sharedMemoryDisplay = shm_open("display", O_RDWR, 0666);
	if (sharedMemoryDisplay == -1) {
		perror("in shm_open() Display");
		exit(1);
	}

	pointerDisplay = mmap(0, SHARED_MEMORY_DISPLAY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemoryDisplay, 0);

	if (pointerDisplay == MAP_FAILED) {
		perror("in map() Display");
		exit(1);
	}

	return 0;
}

void data_display::displayStart(){
    time(&startTime);
    if(pthread_create(&displayThread, &attr, &data_display::displayBegin, (void *)this) != EOK){displayThread = 0;}
}

int data_display::displayStop() {
	pthread_join(displayThread, NULL); // Join thread on stop
	return 0;
}

void *data_display::displayBegin(void *context){
    ((data_display *)context)->displayUpdate();
    return NULL;
}

void *data_display::displayUpdate(void){
    int channelID = ChannelCreate(0);
    if (channelID == -1) {cout << "Failed to Create Display Channel!\n"};

//Change timer according to new Timer Class
    Timer timer(channelID);
    timer.setTimer(DISPLAY_PERIOD, DISPLAY_PERIOD);

    int recieverID;
    int logCounter = 1;
    Message message;
    ofstream out("log")

    while(true){
        if(recieverID == 0){
            pthread_mutex_lock(&mutex);

            int axis = 0, z = 0;
            string buffer = "";
            string x = "";
            string y = "";
            string displayBit = "";
            string id = "";

            for (int i =0; i < SHARED_MEMORY_DISPLAY_SIZE; i++){
                char readChar = *((char *)pointerDisplay + i);
                if (readChar == 't'){
                    cout << "Display Complete!\n";
                    time(&finishTime);
                    double executionTime = difftime(finishTime, startTime);
                    cout << "Display to execute for " << executionTime << " seconds\n"
                    ChannelDestroy(channelID);
                    out.close();
                    return 0;
                }
                if (readChar == ';'){break;}
                else if (readChar == ',' || readChar == '/'){
                    if (buffer.length() > 0){
                        switch (axis) {
                        case 0:
                            id = buffer;
                            break;
                        case 1:
                            x = buffer;
                            break;
                        case 2:
                            y = buffer;
                            break;
                        case 3:
                            z = stoi(buffer) + VALUE_ELEVATION;
                            break;
                        case 4:
                            displayBit = buffer;
                            break;
                        }
                    }
                    if (readChar == ','){
                        axis++;
                    	buffer = "";
                    } else if (readChar == '/') {
                        // One plane has finished loading, parsing and reset control values
                        if (map[(100000 - stoi(y)) / SCALER][stoi(x) / SCALER] == "") {
                            map[(100000 - stoi(y)) / SCALER][stoi(x) / SCALER] += id;
                        } else {
                            map[(100000 - stoi(y)) / SCALER][stoi(x) / SCALER] += "\\" + id;
                        }
                        // Height display control
                        if (display_bit == "1") {
                            height_display = height_display + "Plane " + id +
                                    " has height of " + std::to_string(z) + " ft\n";
                        }
                        x = "";
                        y = "";
                        z = 0;
                        id = "";
                        display_bit = "";
                        axis = 0;
                        buffer = "";
                    }
                } else {
                    buffer += readChar; // Keep loading buffer until delimiter has been detected
                }
            }
            pthread_mutex_unlock(&mutex);

            if (logCounter == 6) {
                cout << "Logging current airspace..." << endl;
                streambuf *coutbuf = cout.rdbuf(); // save old buf
                cout.rdbuf(out.rdbuf());

                // this used to print to stdout but now we redirected it to out.txt
                displayPrint(); // Display map and height command
                cout << endl;

                cout.rdbuf(coutbuf); // reset to standard output again
                logCounter = 1;
            }

            // if not during a multiple of 30 second period, just print to stdout
            // normally
            else {
                displayPrint(); // Display map and height command
            }

            logCounter++;

            height_display = ""; // Reset buffer
            // Reset map array
            memset(map, 0,
                    sizeof(map[0][0]) * block_count *
                    block_count); // Reset map to 0 for next set
        }
        recieverID = MsgReceive(channelID, &msg, sizeof(msg), NULL);
    }
    ChannelDestroy(channelID);
    return 0;
}

void Display::displayPrint() {
	for (int j = 0; j < block_count; j++) {
		for (int k = 0; k < block_count; k++) {
			// Print Empty block if no item
			if (map[j][k] == "") {
				std::cout << "_|";
			} else {
				// print plane ID if there are items
				std::cout << map[j][k] << "|";
			}
		}
		std::cout << std::endl;
	}
	// Display height
	printf("%s\n", height_display.c_str());
}

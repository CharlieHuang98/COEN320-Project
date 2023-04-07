/*
 * data_display.h
 *
 *  Created on: Mar. 19, 2023
 *      Author: Huang
 */

#ifndef SRC_DATA_DISPLAY_H_
#define SRC_DATA_DISPLAY_H_
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>

using namespace std;

#include "System_Specifications.h"
#include "computer_system.h"

// blockcount is the height and width of the display
const int block_count = (int)MARGIN / (int)SCALER + 1;

class data_display {
  public:
    data_display();
    ~data_display();
    int displayInitialize();
    void displayStart();
    int displayStop();
    static void *displayBegin(void *context);
    void *displayUpdate(void)
    void displayPrint()
  private:
  // execution time members
    time_t startTime;
    time_t finishTime;

    //Threads
    pthread_t displayThread;
    pthread_attr_t attribute;
    pthread_mutex_t mutex;

    // Temporary values
    string map[block_count][block_count] = {{""}}; // Shrink 100k by 100k map to 10 by 10, each block is 10k by 10k
    string height_display = "";

    //Shared Memory Members
    int sharedMemoryDisplay;
    void *pointerDisplay;

#endif /* SRC_DATA_DISPLAY_H_ */

/*
 * data_display.h
 *
 *  Created on: Mar. 19, 2023
 *      Author: Huang
 */

#ifndef SRC_DATA_DISPLAY_H_
#define SRC_DATA_DISPLAY_H_

#include <iostream>
#include <string.h>
#include <pthread.h>
using namespace std;

#include "computer_system.h"

class data_display {
  public:
    data_display();
    ~data_display();
    int initialize();
  private:
    //Threads
    pthread_t displayThread;
    pthread_attr_t attribute;
    pthread_mutex_t mutex;
    //Shared Memory Members
    int sharedMemoryDisplay;
    void *pointerDisplay;

#endif /* SRC_DATA_DISPLAY_H_ */

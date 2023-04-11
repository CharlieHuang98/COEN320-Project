/*
 * communication_system.h
 *
 *  Created on: Mar. 12, 2023
 *      Author: Huang
 */

#include <iostream>
#include <string.h>
#include <pthread.h>
using namespace std;

#ifndef SRC_COMMUNICATION_SYSTEM_H_
#define SRC_COMMUNICATION_SYSTEM_H_

pthread_t InitializeCommunicationSystemThread();

void* communication_system_main(void* arg);



#endif /* SRC_COMMUNICATION_SYSTEM_H_ */

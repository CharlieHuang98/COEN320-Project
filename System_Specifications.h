/*
 * System_Specifications.h
 *
 *  Created on: Mar. 23, 2023
 *      Author: Charlie
 */

#ifndef SYSTEM_SPECIFICATIONS_H
#define SYSTEM_SPECIFICATIONS_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <sys/syspage.h>
#include <inttypes.h>
#include <stdint.h>

#include <unistd.h>

#include <pthread.h>

#define FILENAME "input.txt"

/*
 * Airspace Boundaries
 */
#define X_VALUE_MINIMUM 0
#define X_VALUE_MAXIMUM 100000
#define Y_VALUE_MINIMUM 0
#define Y_VALUE_MAXIMUM 100000
#define Z_VALUE_MINIMUM 0
#define Z_VALUE_MAXIMUM 25000
#define VALUE_ELEVATION 15000


/*
 * Radar Specifications
 */


/*
 * Message format and type
 */


typedef struct _pulse msg_header_t;

// Message Type that can be sent to the computer system

typedef enum {ChangeAirplaneSpeed, RequestAirplaneData, RadarScan } AirplaneMsgType;

typedef enum {MsgToComputerSystemType1, MsgToComputerSystemType2, MsgToComputerSystemType3 } MsgTypeToComputerSystem;

typedef enum {MsgToCommSysType1, MsgToCommSysType2, MsgToCommSysType3 } MsgTypeToCommunicationSystem;

typedef enum {DisplayAirplaneInfo, DisplayWarnings, UpdateTerminal} MsgTypeToDataDisplay;

typedef struct {
    msg_header_t hdr;
    MsgTypeToComputerSystem type;
    int threadid;
    int calculationInterval;
    int id;
    double PosX, PosY, PosZ;
    double SpeedX, SpeedY, SpeedZ;
} MsgToCompSystem;

typedef struct  {
    msg_header_t hdr;
    MsgTypeToCommunicationSystem type;
    int id;
    double PosX, PosY, PosZ;
    double SpeedX, SpeedY, SpeedZ;
} MsgToCommunicationSystem;

typedef struct  {
    msg_header_t hdr;
    AirplaneMsgType type;
    int id;
    double PosX, PosY, PosZ;
    double SpeedX, SpeedY, SpeedZ;
} MsgToAirplane;

typedef struct  {
    msg_header_t hdr;
    MsgTypeToCommunicationSystem type;
    int id;
    double PosX, PosY, PosZ;
    double SpeedX, SpeedY, SpeedZ;
} MsgToDataDisplay;


void ERROR(const char * format, ...);

void INFO(const char * format, ...);


#endif /* SYSTEM_SPECIFICATIONS_H */

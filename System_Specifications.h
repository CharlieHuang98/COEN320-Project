/*
 * System_Specifications.h
 *
 *  Created on: Mar. 23, 2023
 *      Author: Charlie Huang
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


#define COMPUTER_CHANNEL "ComputerChannel"
#define COMMUNICATION_CHANNEL "CommunicationChannel"
#define DISPLAY_CHANNEL "DisplayChannel"

#define SIZE 1024

/*
 * Radar Specifications
 */


/*
 * Message format and type
 */


typedef struct _pulse msg_header_t;

// Message Type that can be sent to the computer system

enum AirplaneMsgType{ChangeAirplaneSpeed, RequestAirplaneData, RadarScan };

enum MsgTypeToComputerSystem{MsgToComputerSystemType1, MsgToComputerSystemType2, MsgToComputerSystemType3 };

enum MsgTypeToCommunicationSystem{MsgToCommSysType1, MsgToCommSysType2, MsgToCommSysType3 };

enum MsgTypeToDataDisplay{DisplayAirplaneInfo, DisplayWarnings, UpdateTerminal};

enum MessageType : _Uint16t{Command};

enum MessageSubType : _Uint16t{Change_Speed, Change_Position, Change_Altitude, Request_Plane_Info, Response_Plane_Info};

struct CommonMessage{
	msg_header_t hdr;
	double data;
	int plane_id;
	double Position[3];
	double Speed[3];
};

//typedef struct {
//    msg_header_t hdr;
//    MsgTypeToComputerSystem type;
//    int threadid;
//    int calculationInterval;
//    int id;
//    double PosX, PosY, PosZ;
//    double SpeedX, SpeedY, SpeedZ;
//} MsgToCompSystem;
//
//typedef struct  {
//    msg_header_t hdr;
//    MsgTypeToCommunicationSystem type;
//    int id;
//    double PosX, PosY, PosZ;
//    double SpeedX, SpeedY, SpeedZ;
//} MsgToCommunicationSystem;
//
//typedef struct  {
//    msg_header_t hdr;
//    AirplaneMsgType type;
//    int id;
//    double PosX, PosY, PosZ;
//    double SpeedX, SpeedY, SpeedZ;
//} MsgToAirplane;
//
//typedef struct  {
//    msg_header_t hdr;
//    MsgTypeToCommunicationSystem type;
//    int id;
//    double PosX, PosY, PosZ;
//    double SpeedX, SpeedY, SpeedZ;
//} MsgToDataDisplay;


void ERROR(const char * format, ...);

void INFO(const char * format, ...);

#endif /* SYSTEM_SPECIFICATIONS_H */

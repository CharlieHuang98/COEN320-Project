//airplane.cpp

#include "airplane.h"
using namespace std;

//constructor
airplane::airplane(int id, int arrival_t, int departure_t, int position[3], int speed[3]){
	Id = id;
	Arrival_t = arrival_t;
	Departure_t = departure_t;
	for (int i=0; i<3; i++){
		Position[i] = position[i];
		Speed[i] = speed[i];
	}

}

//destructor
airplane::~airplane(){

}

//Getters
int airplane::getId() const{
	return Id;
}
int airplane::getArrival(){
	return Arrival_t;
}
int airplane::getDeparture(){
	return Departure_t;
}
int* airplane::getPosition(){
	return Position;
}
int* airplane::getSpeed(){
	return Speed;
}
//Setters
void airplane::setPosition(int x,int y, int z){
	Position[0]=x;
	Position[1]=y;
	Position[2]=z;
}
void airplane::setSpeed(int x, int y, int z){
	Speed[0]=x;
	Speed[1]=y;
	Speed[2]=z;
}

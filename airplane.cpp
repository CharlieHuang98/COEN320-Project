//airplane.cpp

#include "airplane.h"
using namespace std;

int airplane::threadCounter = 0;

//constructor
airplane::airplane(int arrival_t, int position[3], int speed[3]){
	Departure_t = 0;
	Arrival_t = arrival_t;
	for (int i=0; i<3; i++){
		Position[i] = position[i];
		Speed[i] = speed[i];
	}
	threadCounter++;
	this->ThreadID = threadCounter;
	ThreadID = 0;
	//initialize the mutex
	pthread_mutex_init(&plane_mutex,NULL);


}

//destructor
airplane::~airplane(){
	pthread_attr_destroy(&attr); //Clean up attribute object
	pthread_mutex_destroy(&plane_mutex);
}

//Getters
int airplane::getDeparture(){
	return Departure_t;
}
int* airplane::getPosition(){
	return Position;
}
int* airplane::getSpeed(){
	return Speed;
}
//Create a plane thread
void airplane::MakeThread(){
	//Initialize thread and set ID
    int err_no = pthread_attr_init(&attr);
    if (err_no != 0){
    	cout<<"Error in airplane.cpp, MakeThread(), pthread_attr_init"<<endl;
    }
    //set detached
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    err_no = pthread_create(&ThreadID, &attr, &airplane::PlaneStart, this);
    if (err_no != 0) { // return non-zero if error
            // Handle error
    	cout<<"Error in airplane.cpp, MakeThread(), pthread_create"<<endl;
    }

}
void* airplane::PlaneStart(void *arg){ //What the function will do
	airplane* plane = static_cast<airplane*>(arg);
	//Thread code here
	pthread_mutex_lock(&plane->plane_mutex);
	//Print to understand the outputs
	cout<<"pthread_self() during execution = "<<pthread_self()<<" of ThreadID: "<<gettid()<< endl;
	//Updating position, ensuring within bounds;
	plane->UpdatePosition();
	plane->CheckAirspace();
	pthread_mutex_unlock(&plane->plane_mutex);

	return NULL;
}
void* airplane::setPosition(int x,int y, int z){
	Position[0]=x;
	Position[1]=y;
	Position[2]=z;
	return NULL;
}
void* airplane::setSpeed(int x, int y, int z){
	Speed[0]=x;
	Speed[1]=y;
	Speed[2]=z;
	return NULL;
}
void airplane::UpdatePosition(){
	for(int i = 0;i<3;i++){
		Position[i] += Speed[i];
	}
}
void airplane::CheckAirspace(){
	if((Position[0]< X_VALUE_MINIMUM) | (Position[0]> X_VALUE_MAXIMUM)){
		//pthread_cancel(ThreadID);
		cout<<"Plane "<<pthread_self()<<" out of X bounds"<<endl;
	}
	else if((Position[1]< Y_VALUE_MINIMUM) | (Position[1]> Y_VALUE_MAXIMUM)){
		//pthread_cancel(ThreadID);
		cout<<"Plane "<<pthread_self()<<" out of Y bounds"<<endl;
	}
	else if((Position[2]< Z_VALUE_MINIMUM) | (Position[2]> Z_VALUE_MAXIMUM)){
		//pthread_cancel(ThreadID);
		cout<<"Plane "<<pthread_self()<<" out of z bounds"<<endl;
	}
}

//airplane.cpp

#include "airplane.h"
using namespace std;

int airplane::threadCounter = 0;

//constructor
airplane::airplane(int departure_t, int position[3], int speed[3]){
	Departure_t = departure_t;
	for (int i=0; i<3; i++){
		Position[i] = position[i];
		Speed[i] = speed[i];
	}
	//threadCounter++;
	this->ThreadID = threadCounter;

}

//destructor
airplane::~airplane(){
	pthread_attr_destroy(&attr); //Clean up attribute object
}

//Getters
//int airplane::getId() const{
//	return Id;
//}
//int airplane::getArrival(){
//	return Arrival_t;
//}
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

//Create a plane thread
void airplane::MakeThread(){
	cout<<"ThreadID before creating thread: "<<ThreadID<<endl;
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
    else{
    	sleep(1);
    	cout<<"\nThread created with ThreadID: "<< ThreadID <<endl;
    	cout<<"pthread_self() output after execution: "<< pthread_self()<<endl; //Always using 1 thread, maybe it's
    }
	cout<<"ThreadID after creating thread: "<<ThreadID<<endl;
}
void* airplane::PlaneStart(void *arg){ //What the function will do
	airplane* plane = static_cast<airplane*>(arg);
	//Thread code here
	//Print to understand the outputs
	cout<<"pthread_self() during execution = "<<pthread_self()<< endl;
	cout<<"ThreadID during execution = "<<plane->ThreadID<<endl;
	//Updating position, ensuring within bounds;
	plane->UpdatePosition();
	plane->CheckAirspace();

//	}

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
		pthread_cancel(ThreadID);
		cout<<"Plane "<<pthread_self()<<" out of X bounds"<<endl;
	}
	else if((Position[1]< Y_VALUE_MINIMUM) | (Position[1]> Y_VALUE_MAXIMUM)){
		pthread_cancel(ThreadID);
		cout<<"Plane "<<pthread_self()<<" out of Y bounds"<<endl;
	}
	else if((Position[2]< Z_VALUE_MINIMUM) | (Position[2]> Z_VALUE_MAXIMUM)){
		pthread_cancel(ThreadID);
		cout<<"Plane "<<pthread_self()<<" out of z bounds"<<endl;
	}
}

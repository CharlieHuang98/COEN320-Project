//airplane.cpp

#include "airplane.h"
using namespace std;

//int airplane::threadCounter = 0;


//constructor
airplane::airplane(int arrival_t, int position[3], int speed[3]){ //Should not be initialized in the contructor, instead should be passed by the comm system
	Departure_t = 0;
	Arrival_t = 0;
	for (int i=0; i<3; i++){
		Position[i] = position[i];
		Speed[i] = speed[i];
	}
	//threadCounter++;
	//this->ThreadID = threadCounter;
	ThreadID = 0;
	//initialize the mutex
	pthread_mutex_init(&plane_mutex,NULL);

	initialize_airplane();

}

//destructor
airplane::~airplane(){
	pthread_attr_destroy(&attr); //Clean up attribute object
	pthread_mutex_destroy(&plane_mutex);
	shm_unlink(name);
	ThreadID = 0;
}
void airplane::pthreadJoin(){
	pthread_join(ThreadID, NULL);
}
void airplane::initialize_airplane(){
	//Initialize thread and set state
	    int err_no = pthread_attr_init(&attr);
	    if (err_no != 0){
	    	cout<<"Error in airplane.cpp, initialize_airplane(), pthread_attr_init"<<endl;
	    }
	    //resources will not be automatically released when it terminates and another
	    //thread can wait for it to terminate and release its resources using pthread_join()
	    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	/* Create a new memory object */
	// create the shared memory segment
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	if (shm_fd == -1){
		perror("In shm_open() writer");
		exit(1);
	}

	// configure the size of the shared memory segment
	ftruncate(shm_fd, SIZE);

	// now map the shared memory segment in the address space of the process
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
	}
	//initialize the shm for our values

	sprintf((char *)ptr, "%d,%d,%d,%d,%d,%d,%d,%d,%d;", ThreadID, Position[0], Position[1], Position[2], Speed[0], Speed[1], Speed[2], 0, 0);
}
//Getters
int airplane::getDeparture(){
	return Departure_t;
}
int* airplane::getPosition(){ //Needs to get initial position from comm systen
	return Position;
}
int* airplane::getSpeed(){ //Needs to get initial speed from comm system
	return Speed;
}
//start routine
void* plane_start_routine(void *arg){
	airplane& Airplane = *(airplane*) arg;
	Airplane.PlaneStart();
	return NULL;
}
//Create a plane thread
void airplane::MakeThread(){

    if (pthread_create(&ThreadID, &attr, plane_start_routine, (void *)this)!=EOK) { // return non-zero if error
    	ThreadID = 0;
    }

}
void *airplane::PlaneStart(void){ //What the function will do
//	airplane* plane = static_cast<airplane*>(arg);
	//timer created setting offset and period

	//int period_sec=1;
	//time timer(period_sec,0);

	//Thread code here
	while(true){
		pthread_mutex_lock(&plane_mutex);
			//Print to understand the outputs
			cout<<"pthread_self() during execution = "<<pthread_self()<<" of ThreadID: "<<gettid()<< endl;
			//Updating position, ensuring within bounds;
			GetCommand();
			UpdatePosition();
			OutputPosition();
			CheckAirspace();
			WriteToSHM();
			pthread_mutex_unlock(&plane_mutex);
			//time.waitTimer();
	//needs to have a timer wait till next pulse so that other planes get their turn
	}


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
void airplane::UpdatePosition(){ //Currently changing position for all threads, needs to change position then write to file
	for(int i = 0;i<3;i++){
		Position[i] += Speed[i];
	}

}
void airplane::CheckAirspace(){
	if((Position[0]< X_VALUE_MINIMUM) | (Position[0]> X_VALUE_MAXIMUM)){
		pthread_cancel(this->ThreadID);
		cout<<"Plane "<<pthread_self()<<" out of X bounds"<<endl;
	}
	else if((Position[1]< Y_VALUE_MINIMUM) | (Position[1]> Y_VALUE_MAXIMUM)){
		pthread_cancel(this->ThreadID);
		cout<<"Plane "<<pthread_self()<<" out of Y bounds"<<endl;
	}
	else if((Position[2]< Z_VALUE_MINIMUM) | (Position[2]> Z_VALUE_MAXIMUM)){
		pthread_cancel(this->ThreadID);
		cout<<"Plane "<<pthread_self()<<" out of Z bounds"<<endl;
	}
}
void airplane::OutputPosition(){
	cout<<"X: "<<Position[0]<<" Y: "<<Position[1]<<" Z:"<<Position[2]<<endl;
}
void airplane::WriteToSHM(){
	string space = " ";
	string planeStr = to_string(ThreadID) + space + to_string(Position[0]) + space + to_string(Position[1]) + space + to_string(Position[2]) + space + to_string(Speed[0]) + space + to_string(Speed[1]) + space + to_string(Speed [2]) + space + to_string(Departure_t);
	sprintf((char *)ptr, "%s0;", planeStr); //Write the string with the info to shared memory
}
void airplane::GetCommand(){
	//gets a command to create a thread, and set the airplane position and velocity
}

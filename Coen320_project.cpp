//#include <iostream>
//#include "airplane.h"
//using namespace std;
//
//int main() {
//	cout << "Hello World!!!" << endl; // prints Hello World!!!
//	cout<<"pthread_self() of main: "<<pthread_self()<< " with ThreadID: "<<gettid()<<"\n"<<endl;
//	int position[3];
//	int speed[3];
//	for (int i = 0; i<3;i++){
//		position[i]=24980;
//		speed[i]=10;
//	}
//	airplane plane(0,position,speed);
//	for(int p=0;p<2;p++){
//		//cout<<"for p: "<<p<<" making thread"<<endl;
//		plane.MakeThread();
//		sleep(0);
//		//cout<<"\nfor p: "<<p<<" made thread\n"<<endl;
//	}
//	plane.pthreadJoin();
//	return 0;
//}

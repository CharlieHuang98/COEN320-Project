#include <iostream>
#include "airplane.h"
using namespace std;

int main() {
	cout << "Hello World!!!\n" << endl; // prints Hello World!!!
	int position[3];
	int speed[3];
	for (int i = 0; i<3;i++){
		position[i]=0;
		speed[1]=1;
	}


	airplane plane1(0,position,speed);
	plane1.MakeThread();
	cout<<"\nobject 1 thread made, now for 2nd object thread"<<endl;
	sleep(1);
	airplane plane2(0,position,speed);
	plane2.MakeThread();
	cout<<"\nobject 2 thread made, now for 3rd object thread"<<endl;
	sleep(1);
	airplane plane3(0,position,speed);
	plane3.MakeThread();

	return 0;
}

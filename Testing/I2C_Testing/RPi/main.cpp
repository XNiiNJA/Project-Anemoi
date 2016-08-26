#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
#include <errno.h>
extern int errno;

using namespace std;

int main()
{
	
	cout << "Initializing I2C..." << endl;

	if(wiringPiI2CSetup(5) == -1)
	{
		cout << "I2C init failed!" << endl;
		cout << errno << endl;
		return -1;
	}
	else
		cout << "I2C init SUCCESS!" << endl;

		
	wiringPiI2CWrite (43, 3);

}

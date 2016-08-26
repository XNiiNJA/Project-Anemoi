#include <iostream>
#include <ctime>
#include <cmath>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Receiver.h"

using namespace std;

int main (void)
{
	
	Receiver::init();
	
	cout << "Receiver Initialized" << endl;
	
	for(;;)
	{
		cout << "post" << endl;
		
		Receiver::update();
		
		cout << turn_amount << endl;
		
	}
	return 0 ;
}

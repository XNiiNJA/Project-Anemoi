#include <iostream>
#include <ctime>
#include <cmath>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Receiver.h"
#include "ControlsProcessing.h"
#include "CommandProcessing.h"

using namespace std;

int main (void)
{
	
	Receiver::init();
	
	ControlsProcessing::init();
	
	CommandProcessing::init();
	
	cout << "Receiver Initialized" << endl;
	
	for(;;)
	{
		
		
		Receiver::update();
		
		ControlsProcessing::update();
		
		CommandProcessing::update();
		
	}
	return 0 ;
}

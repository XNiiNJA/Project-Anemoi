#include "ControlsProcessing.h"
#include "CommandProcessing.h"
#include "Receiver.h"
#include <iostream>

using namespace std;

#define Y_AXIS_CHANNEL 	0
#define X_AXIS_CHANNEL	1

#define ARM_CHANNEL	    6	
#define DISARM_CHANNEL  7

#define THROTTLE_CHANNEL  10

#define THROTTLE_DOWN_CHANNEL 11

/*Defined if the throttle is to be moved up one step every time throttle channel is active*/
#define DISCRETE_THROTTLE 

#define THROTTLE_STEP 10

#define THROTTLE_MAX 100

#define THROTTLE_MIN 0

int8_t ControlsProcessing::throttleUpLast = 0;

int8_t ControlsProcessing::throttleDownLast = 0;

int8_t ControlsProcessing::throttleControl = 0;

void ControlsProcessing::init()
{

	
	
}


void ControlsProcessing::update()
{
	
	int8_t yAxisControl = Receiver::getChannel(Y_AXIS_CHANNEL);
	
	int8_t xAxisControl = Receiver::getChannel(X_AXIS_CHANNEL);

	int8_t armControl = Receiver::getChannel(ARM_CHANNEL);
	
	int8_t disarmControl = Receiver::getChannel(DISARM_CHANNEL);
	
	
	/*Y Axis control needs to be inversed before sending to the controller*/
	yAxisControl = yAxisControl * -1;
	
	
#ifdef DISCRETE_THROTTLE
	
	int8_t throttleUp = Receiver::getChannel(THROTTLE_CHANNEL);
	
	int8_t throttleDown = Receiver::getChannel(THROTTLE_DOWN_CHANNEL);
	
	/*When the throttle moves from 0 to 1, add to throttle*/
	if(throttleUpLast == 0 && throttleUp == 1)
	{
		throttleControl += THROTTLE_STEP;
		std::cout << "Throttle Up! " << throttleControl << std::endl;
	}
	
	/*Same with going down in throttle */
	if(throttleDownLast == 0 && throttleDown == 1)
	{
		throttleControl -= THROTTLE_STEP;
		std::cout << "Throttle Down! " << throttleControl << endl;
	}
	if(throttleControl > THROTTLE_MAX)
		throttleControl = THROTTLE_MAX;
	else if(throttleControl < THROTTLE_MIN)
		throttleControl = THROTTLE_MIN;
	else
	{}
		//Do nothing.

	throttleDownLast = throttleDown;
	throttleUpLast = throttleUp;

#else

	//TODO: implement

#endif
	
	if(armControl == 1)
	{
		throttleControl = 0;
		CommandProcessing::arm();
		std::cout << "ARM! " << endl;
	}
	else
	{
		CommandProcessing::resetArm();

	}
	
	if(disarmControl == 1)
	{
		throttleControl = 0;
		CommandProcessing::disarm();
	}
	else
	{ 
		CommandProcessing::resetDisarm();
	}
	
	CommandProcessing::setBasePower((float)throttleControl);

	CommandProcessing::setOrientationVector(xAxisControl, yAxisControl, 100);	
	
}

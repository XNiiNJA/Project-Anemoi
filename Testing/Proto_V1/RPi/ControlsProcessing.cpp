#include "ControlsProcessing.h"
#include "CommandProcessing.h"
#include "Receiver.h"
#include <iostream>

using namespace std;


/*Defined if the throttle is to be moved up one step every time throttle channel is active*/
//#define DISCRETE_THROTTLE 

#define THROTTLE_STEP 10

#define THROTTLE_MAX 100

#define THROTTLE_MIN 0

int8_t ControlsProcessing::throttleUpLast = 0;

int8_t ControlsProcessing::throttleDownLast = 0;

float ControlsProcessing::throttleControl = 0;

int8_t ControlsProcessing::xAxisControl = 0;
int8_t ControlsProcessing::yAxisControl = 0;
int8_t ControlsProcessing::zAxisControl = 0;
int8_t ControlsProcessing::armControl = 0;
int8_t ControlsProcessing::disarmControl = 0;

void ControlsProcessing::init()
{

	
	
}

void ControlsProcessing::setParam(std::string tag, uint64_t val)
{
	

	
	if (tag == X_VECTOR_TAG)
	{
		float temp_float = reinterpret_cast<float &>(val);
		xAxisControl = (MAX_CONTROL * temp_float);
	}
	else if (tag == Y_VECTOR_TAG)
	{
		float temp_float = reinterpret_cast<float &>(val);
		yAxisControl = ((float)MAX_CONTROL * temp_float);
	}
	else if (tag == Z_VECTOR_TAG)
	{	
		float temp_float = reinterpret_cast<float &>(val);
		zAxisControl = ((float)MAX_CONTROL * temp_float);
	}
	else if (tag == MAG_VECTOR_TAG)
	{	
		float temp_float = reinterpret_cast<float &>(val);
		throttleControl = (temp_float/255) * 100.0f;//((float)MAX_CONTROL * temp_float);
		cout << "Throttle: " << (float)throttleControl << endl;
	}
	else if (tag == VECTOR_TYPE_TAG)
	{
		//Unused.. for now.
	}
	else if (tag == ARM_TAG)
	{
		bool temp = reinterpret_cast<bool &>(val);
		
		armControl = temp ? 1 : 0;
		
	}
	else if (tag == DISARM_TAG)
	{
		bool temp = reinterpret_cast<bool &>(val);
		
		disarmControl = temp ? 1 : 0;		
		
	}
	
	
}

void ControlsProcessing::update()
{
	
	/*int8_t yAxisControl = Receiver::getChannel(Y_AXIS_CHANNEL);
	
	int8_t xAxisControl = Receiver::getChannel(X_AXIS_CHANNEL);

	int8_t armControl = Receiver::getChannel(ARM_CHANNEL);
	
	int8_t disarmControl = Receiver::getChannel(DISARM_CHANNEL);*/
	
	
	
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

	CommandProcessing::setOrientationVector(xAxisControl, yAxisControl, zAxisControl);	
	
	cout << "<" << (int)xAxisControl << " , " << (int)yAxisControl << " , " << (int)zAxisControl << ">" << endl;
	
}

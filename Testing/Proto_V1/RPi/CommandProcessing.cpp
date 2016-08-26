#include "CommandProcessing.h"



long int lastRemainTime = 0;

void CommandProcessing::init()
{

	//initialize control parameters.
	disarm();
	
	setBasePower(0.0f); 
	
	setOrientationVector(0, 0, 1);

	 

}

void CommandProcessing::update()
{
	//Send a new message every 10 ms in order to let the arduino
	//have time to run the control loop before giving it more commands.
	
	long int remainTime = 0;
	struct timespec curTime;
	
	clock_gettime(CLOCK_REALTIME, &curTime);
	
	remainTime = curTime.tv_nsec % MESSAGE_SEND_TIME_NSEC;
	
	if(remainTime < lastRemainTime)
	{
		/*Send the next message*/
			
	}
	
	lastRemainTime = remainTime;
	
}

void CommandProcessing::setCommand(uint8_t command, uint8_t * data)
{
	
	int8_t commandIndex = command - 1;
	
	commandsBuffer[commandIndex][0] = command;
	
	for(int i = 1; i < MAX_COMMAND_SIZE; i++)
	{
		commandsBuffer[commandIndex][i] = data[i - 1];
	}
	
}

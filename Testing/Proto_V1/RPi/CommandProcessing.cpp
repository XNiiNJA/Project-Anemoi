#include "CommandProcessing.h"
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>


#define MESSAGE_SEND_TIME_NSEC 100000000

using namespace std;

// The address of the Arduino
#define ADDRESS 0x04

long int lastRemainTime = 0;

uint8_t CommandProcessing::commandsBuffer[NUM_COMMANDS][MAX_COMMAND_SIZE];

uint8_t CommandProcessing::prevBuffer[NUM_COMMANDS][MAX_COMMAND_SIZE];

int CommandProcessing::i2c_file;

int CommandProcessing::currentCommand = 0;
 
// The I2C bus: This is for V2 pi's. For V1 Model B you need i2c-0
static const char *devName = "/dev/i2c-1";

void CommandProcessing::init()
{

	//initialize control parameters.
	disarm();
	
	setBasePower(0.0f); 
	
	setOrientationVector(0, 0, 1);

	i2c_file = open(devName, O_RDWR);
	
	ioctl(i2c_file, I2C_SLAVE, ADDRESS);


	for(int i = 0; i < NUM_COMMANDS; i++)
		for(int j = 0; j < MAX_COMMAND_SIZE; j++)
			commandsBuffer[i][j] = 0;

}

void CommandProcessing::update()
{
	//Send a new message every 10 ms in order to let the arduino
	//have time to run the control loop before giving it more commands.
	
	long int thisTime = 0;
	struct timespec curTime;
	bool sentBefore = true;
	
	clock_gettime(CLOCK_REALTIME, &curTime);
	
	thisTime = curTime.tv_nsec;
	
	if(thisTime - lastRemainTime >= MESSAGE_SEND_TIME_NSEC || thisTime < lastRemainTime)
	{	  
		
		//Check that we didn't send this last time
		for(int i = 0; i < MAX_COMMAND_SIZE; i++)
			if(prevBuffer[currentCommand][i] != commandsBuffer[currentCommand][i])
			{
				sentBefore = false;
				break;
			}
		
		//If this was already sent, skip it. We'll pick the next one up on the next go around.
		if(!sentBefore)
		{
			
			write(i2c_file, commandsBuffer[currentCommand], MAX_COMMAND_SIZE);
			
			for(int i = 0; i < MAX_COMMAND_SIZE; i++)
				prevBuffer[currentCommand][i] = commandsBuffer[currentCommand][i];
				
			
			cout << "sent: " << currentCommand << endl;
			
			for(int i = 0; i < MAX_COMMAND_SIZE; i++)
				cout << (int)commandsBuffer[currentCommand][i] << " , ";
			
			cout << endl;
			lastRemainTime = thisTime;
		}
		else
		{
			//Do nothing.
		}
		
		currentCommand++;
		
	}
	
	if(currentCommand >= NUM_COMMANDS)
		currentCommand = 0;
	
	
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

/*Set the orientation of the multicopter*/
void CommandProcessing::setOrientationVector(int8_t x, int8_t y, int8_t z)
{
	
	uint8_t oriData[MAX_COMMAND_SIZE - 1];
	
	oriData[0] = reinterpret_cast<uint8_t&>(x);
	oriData[1] = reinterpret_cast<uint8_t&>(y);
	oriData[2] = reinterpret_cast<uint8_t&>(z);
	
	setCommand(ORIENT_COMMAND, oriData);
	
	
	
}

/*Set the base power of the multicopter (Takes a value 0.0 - 100.0)*/
void CommandProcessing::setBasePower(float power)
{	
	uint8_t powerData[MAX_COMMAND_SIZE - 1];
	
	uint32_t powerBytes = reinterpret_cast<uint32_t&>(power);
	
	powerData[0] = powerBytes & 0xFF;
	powerData[1] = (powerBytes >> 8) & 0xFF;
	powerData[2] = (powerBytes >> 16) & 0xFF;
	powerData[3] = (powerBytes >> 24) & 0xFF;
	
	
	setCommand(COMMAND_THROTTLE, powerData);
	
}
	
void CommandProcessing::disarm()
{
	uint8_t armData[MAX_COMMAND_SIZE - 1];
	
	armData[0] = 1;
	
	setCommand(COMMAND_DISARM, armData);
}
	
void CommandProcessing::arm()
{
	uint8_t armData[MAX_COMMAND_SIZE - 1];
	
	armData[0] = 1;
	
	setCommand(COMMAND_ARM, armData);
}

void CommandProcessing::resetDisarm()
{
	uint8_t armData[MAX_COMMAND_SIZE - 1];
	
	armData[0] = 0;
	
	setCommand(COMMAND_DISARM, armData);
	
}

void CommandProcessing::resetArm()
{
	uint8_t armData[MAX_COMMAND_SIZE - 1];
	
	armData[0] = 0;
	
	setCommand(COMMAND_ARM, armData);
	
}

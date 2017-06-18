#include "CommandProcessing.h"
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

//Add a zero if this breaks
#define MESSAGE_SEND_TIME_NSEC 0//1000000//10000000

using namespace std;

// The address of the Arduino
#define ADDRESS 0x04

long int lastRemainTime = 0;

uint8_t CommandProcessing::commandsBuffer[NUM_COMMANDS][COMMAND_CODE_SIZE + MAX_COMMAND_SIZE + ERROR_CORRECTION_SIZE];

uint8_t CommandProcessing::loopbackBuffer[NUM_COMMANDS][COMMAND_CODE_SIZE + MAX_COMMAND_SIZE + ERROR_CORRECTION_SIZE];

uint8_t CommandProcessing::prevBuffer[NUM_COMMANDS][COMMAND_CODE_SIZE + MAX_COMMAND_SIZE + ERROR_CORRECTION_SIZE];

uint8_t CommandProcessing::readyBuffer[NUM_COMMANDS];

int CommandProcessing::i2c_file;
int CommandProcessing::spi_file;

int CommandProcessing::currentCommand = 0;

int error_correction_start = COMMAND_CODE_SIZE + MAX_COMMAND_SIZE;
 
// The I2C bus: This is for V2 pi's. For V1 Model B you need i2c-0
static const char *devName = "/dev/i2c-1";

static const char *spi_devName = "/dev/spidev0.0";

void CommandProcessing::init()
{

	//initialize control parameters.
	disarm();
	
	setBasePower(0.0f); 
	
	setOrientationVector(0, 0, 1);

//  USING SPI NOW. This is for I2C
//
//	i2c_file = open(devName , O_RDWR);
//	
//	ioctl(i2c_file, I2C_SLAVE, ADDRESS);

	spi_file = open(spi_devName, O_RDWR);

	for(int i = 0; i < NUM_COMMANDS; i++)
		for(int j = 0; j < MAX_COMMAND_SIZE; j++)
			commandsBuffer[i][j] = 0;

	memset(readyBuffer, 0, sizeof(readyBuffer));

}

void CommandProcessing::update()
{
	//Send a new message every 10 ms in order to let the arduino
	//have time to run the control loop before giving it more commands.
	
	long int thisTime = 0;
	struct timespec curTime;
	bool sentBefore = true;
	struct spi_ioc_transfer xfer[NUM_COMMANDS];
	
	memset(xfer, 0, sizeof(xfer));
	
	clock_gettime(CLOCK_REALTIME, &curTime);
	
	thisTime = curTime.tv_nsec;
	
	if(thisTime - lastRemainTime >= MESSAGE_SEND_TIME_NSEC || thisTime < lastRemainTime)
	{	  
		
		//Check that we didn't send this last time
		//for(int i = 0; i < MAX_COMMAND_SIZE; i++)
		//	if(prevBuffer[currentCommand][i] != commandsBuffer[currentCommand][i])
		//	{
		//		sentBefore = false;
		//		break;
		//	}
		
		uint8_t transfer_count = 0;
		
		for(int i = 0; i <= NUM_COMMANDS; i++)
		{
			
			if(currentCommand >= NUM_COMMANDS)
				currentCommand = 0;
			
			cout << "..................................................testing: " << currentCommand << endl;
			
			if(readyBuffer[currentCommand])
			{
				cout << "..................................................passed: " << currentCommand << endl;
			
				cout << (int)*(&commandsBuffer[currentCommand])[0] << endl;
			
				sentBefore = false;
				
				xfer[transfer_count].tx_buf = (unsigned long)commandsBuffer[currentCommand];
				xfer[transfer_count].rx_buf = (unsigned long)loopbackBuffer[currentCommand];
				xfer[transfer_count].len = COMMAND_CODE_SIZE + MAX_COMMAND_SIZE + ERROR_CORRECTION_SIZE;
				xfer[transfer_count].speed_hz = 57600;
				xfer[transfer_count].delay_usecs = 1000;			
				xfer[transfer_count].cs_change = true;
				
				transfer_count++;
				
				readyBuffer[currentCommand] = 0;
				
				//break;
			}
			
			currentCommand++;
						
		}
		
		struct spi_ioc_transfer xfer_final[transfer_count];
		
		memset(xfer_final, 0, sizeof(xfer_final));
		
		for(int i = 0; i < transfer_count; i++)
			xfer_final[i] = xfer[i];
		
		
		//If this was already sent, skip it. We'll pick the next one up on the next go around.
		if(transfer_count > 0)//!sentBefore)
		{
			
			
			
			/*for(int i = 0; i < NUM_COMMANDS; i++)
			{

				uint8_t tempbuff[MAX_COMMAND_SIZE];

			
				for(int j = 0; j < MAX_COMMAND_SIZE; j++)
					tempbuff[j] = commandsBuffer[i][j];
 
				xfer[i].tx_buf = (unsigned long)commandsBuffer[i];
				xfer[i].len = MAX_COMMAND_SIZE;
				xfer[i].speed_hz = 250000;
				xfer[i].delay_usecs = 1000;			
				xfer[i].cs_change = true;
			}*/
			 
			//write(i2c_file, tempbuff, MAX_COMMAND_SIZE);
			int status = ioctl(spi_file, SPI_IOC_MESSAGE(transfer_count), xfer);
			
			if(status < 0)
			{
				perror("SPI_IOC_MESSAGE");
			}
			
			cout << "sent: " << currentCommand << endl;
			
			for(uint8_t i = 0 ; i < sizeof(readyBuffer); i++)
			{
				cout << "LOOPBACK...." << endl;
				if(readyBuffer[i])
				{	
					cout << "LOOPBACK: ";
					for(uint8_t j = 0; j < sizeof(loopbackBuffer[i]); j++)
					{
						cout << (int)loopbackBuffer[i][j] << " , ";
					}
					cout << endl;
				}
			}
			
			/*
			for(int i = 0; i < MAX_COMMAND_SIZE; i++)
				cout << (int)commandsBuffer[currentCommand][i] << " , ";
			*/
			cout << endl;
			lastRemainTime = thisTime;
			
			
			readyBuffer[currentCommand] = 0;
			
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

void CommandProcessing::setCommand(uint8_t command, uint8_t * data, int length)
{
	
	int8_t commandIndex = command - 1;
	
	bool changed = false;
	
	//check if any of the data actually changed.
	for(int i = 0; i < length; i++)
	{
		if(commandsBuffer[commandIndex][i + 1] != data[i])
		{
			changed = true;
			break;
		}
	}
	
	if(changed)
	{
		

		
		uint8_t parity = commandsBuffer[commandIndex][0] = command;

		for(int i = 0; i < MAX_COMMAND_SIZE - 1; i++)
		{
			//XOR for parity
			parity ^= data[i];
			
			if(i < length)
				commandsBuffer[commandIndex][i + 1] = data[i];
			else
				commandsBuffer[commandIndex][i + 1] = 0;
			
			if(command == COMMAND_THROTTLE)
				printf("%#04x\n", commandsBuffer[commandIndex][i]);

		}
	
		commandsBuffer[commandIndex][error_correction_start] = parity;
	
		cout << "Parity (" << (int)commandIndex << ") " << (int)parity << endl;
	
		readyBuffer[commandIndex] = 1;
		
	}
}

/*Set the orientation of the multicopter*/
void CommandProcessing::setOrientationVector(int8_t x, int8_t y, int8_t z)
{
	
	uint8_t oriData[MAX_COMMAND_SIZE - 1];
	
	oriData[0] = reinterpret_cast<uint8_t&>(x);
	oriData[1] = reinterpret_cast<uint8_t&>(y);
	oriData[2] = reinterpret_cast<uint8_t&>(z);
	
	setCommand(ORIENT_COMMAND, oriData, 3);
	
	
	
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
	
	
	setCommand(COMMAND_THROTTLE, powerData, 4);
	
}
	
void CommandProcessing::disarm()
{
	uint8_t armData[MAX_COMMAND_SIZE - 1];
	
	armData[0] = 1;
	
	setCommand(COMMAND_DISARM, armData, 1);
}
	
void CommandProcessing::arm()
{
	uint8_t armData[MAX_COMMAND_SIZE - 1];
	
	armData[0] = 1;
	
	setCommand(COMMAND_ARM, armData, 1);
}

void CommandProcessing::resetDisarm()
{
	uint8_t armData[MAX_COMMAND_SIZE - 1];
	
	armData[0] = 0;
	
	setCommand(COMMAND_DISARM, armData, 1);
	
}

void CommandProcessing::resetArm()
{
	uint8_t armData[MAX_COMMAND_SIZE - 1];
	
	armData[0] = 0;
	
	setCommand(COMMAND_ARM, armData, 1);
	
}

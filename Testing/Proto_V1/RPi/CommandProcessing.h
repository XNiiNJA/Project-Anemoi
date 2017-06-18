#include <ctime>
#include "types.h"


#define COMMAND_ARM       0x1u
#define COMMAND_DISARM    0x2u
#define COMMAND_THROTTLE  0x3u

#define ORIENT_COMMAND    0x4u
#define ACCEL_COMMAND     0x5u
#define VELOCITY_COMMAND  0x6u
#define POSITION_COMMAND  0x7u

//This message is used to resync SPI with the arduino.
//Shouldn't be needed, but I'm reserving it now.
#define SYNC_MESSAGE 0xFFu

#define ORIENT_COMMAND_X  0
#define ORIENT_COMMAND_Y  1
#define ORIENT_COMMAND_Z  2 


#define NUM_COMMANDS      4
#define MAX_COMMAND_SIZE  6
#define COMMAND_CODE_SIZE 1
#define ERROR_CORRECTION_SIZE 1

#define MAX_COMMAND_INDEX 2


class CommandProcessing
{

public:

	static void init();
		
	static void update();
	
		
	/*Set the data that should be sent for a specified command*/
	static void setCommand(uint8_t command, uint8_t * data, int length);

	/*Set the orientation of the multicopter*/
	static void setOrientationVector(int8_t x, int8_t y, int8_t z);

	/*Set the base power of the multicopter (Takes a value 0.0 - 100.0)*/
	static void setBasePower(float power);
	
	static void disarm();
	
	static void arm();
	
	static void resetArm();
	
	static void resetDisarm();
	
	/*TODO: 
	 * Add setAccelerationVector()
	 * Add setAccelerationMagnitude()
	 * Add setVelocityVector()
	 * Add setVelocityMagnitude()
	 * Add setPosition()
	 */

private: 

	static long nextUpdateMillis;

	static uint8_t commandsBuffer[NUM_COMMANDS][COMMAND_CODE_SIZE + MAX_COMMAND_SIZE + ERROR_CORRECTION_SIZE];
	
	static uint8_t loopbackBuffer[NUM_COMMANDS][COMMAND_CODE_SIZE + MAX_COMMAND_SIZE + ERROR_CORRECTION_SIZE];

	static uint8_t prevBuffer[NUM_COMMANDS][COMMAND_CODE_SIZE + MAX_COMMAND_SIZE + ERROR_CORRECTION_SIZE];	

	static uint8_t readyBuffer[NUM_COMMANDS];

	static int currentCommand;
	
	//The i2c file
	static int i2c_file;
	
	static int spi_file;
	
};

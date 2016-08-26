#include <time.h>



#define COMMAND_ARM       0x1u
#define COMMAND_DISARM    0x2u
#define ORIENT_COMMAND    0x3u
#define ACCEL_COMMAND     0x4u
#define VELOCITY_COMMAND  0x5u
#define POSITION_COMMAND  0x6u


#define ORIENT_COMMAND_X  0
#define ORIENT_COMMAND_Y  1
#define ORIENT_COMMAND_Z  2 

#define MESSAGE_SEND_TIME_NSEC 10000

#define NUM_COMMANDS      3
#define MAX_COMMAND_SIZE  5

#define MAX_COMMAND_INDEX 2


class CommandProcessing
{

public:

	static void init();
		
	static void update();
	
		
	/*Set the data that should be sent for a specified command*/
	static void setCommand(uint8_t command, uint8_t * data);

	/*Set the orientation of the multicopter*/
	static void setOrientationVector(int8_t x, int8_t y, int8_t z);

	/*Set the base power of the multicopter (Takes a value 0.0 - 100.0)*/
	static void setBasePower(float power);
	
	static void disarm();
	
	static void arm();
	
	/*TODO: 
	 * Add setAccelerationVector()
	 * Add setAccelerationMagnitude()
	 * Add setVelocityVector()
	 * Add setVelocityMagnitude()
	 * Add setPosition()
	 */

private: 

	static long nextUpdateMillis;

	static uint8_t commandsBuffer[NUM_COMMANDS][MAX_COMMAND_SIZE];
	
}

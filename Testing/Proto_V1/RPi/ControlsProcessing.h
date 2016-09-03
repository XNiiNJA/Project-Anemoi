/**
 * 
 * This class acts as a middleman between receiving commands, and formatting them into commands
 * 
 */
 
#include "types.h"

class ControlsProcessing
{

public:

	static void init();
	
	static void update();
	
private:

	static int8_t throttleUpLast;

	static int8_t throttleDownLast;

	static int8_t throttleControl;

};

/**
 * 
 * This class acts as a middleman between receiving commands, and formatting them into commands
 * 
 */
 
#include "types.h"
#include <string>

#ifndef CONTROLSPROCESSING_H
#define CONTROLSPROCESSING_H

#define MAX_CONTROL 0x7f

class ControlsProcessing
{

public:

	static void init();
	
	static void update();
	
	static void setParam(std::string tag, uint64_t val);
	
private:

	static int8_t throttleUpLast;

	static int8_t throttleDownLast;

	static float throttleControl;

	static int8_t yAxisControl;
	
	static int8_t xAxisControl;

	static int8_t zAxisControl;
	
	static int8_t armControl;

	static int8_t disarmControl;

};

#endif

#pragma once

/*
 * This class maintains the current known state of the target.
*/

#include "Dimensionals.h"

struct State {
	quaternion orient;
	long sequence; //This is the last sequence number recieved from the target
};

class StateManager
{



private:

	State lastState;



};
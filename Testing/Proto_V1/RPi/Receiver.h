#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <iostream>
#include <cassert>
#include "types.h"

#ifndef RECEIVER_H
#define RECIEVER_H

#define NUM_CHANNELS 6

#define X_VECTOR_TAG "v_x"
#define Y_VECTOR_TAG "v_y"
#define Z_VECTOR_TAG "v_z"

#define MAG_VECTOR_TAG "v_mag"

#define VECTOR_TYPE_TAG "t_vect"

#define ARM_TAG "c_arm"
#define DISARM_TAG "c_disarm"

#define Y_AXIS_CHANNEL 	0
#define X_AXIS_CHANNEL	1

#define ARM_CHANNEL	    6	
#define DISARM_CHANNEL  7

#define THROTTLE_CHANNEL  10

#define THROTTLE_DOWN_CHANNEL 11


/**
 * Class to recieve data for control purposes. 
 * Splits data into n channels for control. 
 * Each channel controls a different controlable part.
 */
class Receiver
{

public:


	static void init();

	static void update();
	
	static int getTurn();
	
	static int getPower();
	
	static int8_t getChannel(int id);
	
	
private:


};

#endif

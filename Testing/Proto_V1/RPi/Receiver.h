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

#define NUM_CHANNELS 6

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
	
	
	
private:


};

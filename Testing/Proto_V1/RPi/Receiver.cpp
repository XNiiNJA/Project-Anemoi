#include "Receiver.h"

#define TURN_CHANNEL  1
#define POWER_CHANNEL 0

#define POWER_CORRECTION 127

using namespace std;

	static uint8_t channels[2];
	static int port = 6000; 
	static int broadcast = 1;
	static int s;
	static sockaddr_in si_me, si_other;

void Receiver::init()
{
		
	s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	setsockopt(s, SOL_SOCKET, SO_BROADCAST,
				&broadcast, sizeof broadcast);

	memset(&si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(port);
	si_me.sin_addr.s_addr = INADDR_ANY;

	bind(s, (sockaddr *)&si_me, sizeof(sockaddr));

}

void Receiver::update()
{

    char buf[10000];
    unsigned slen=sizeof(sockaddr);
    recvfrom(s, buf, sizeof(buf)-1, 0, (sockaddr *)&si_other, &slen);

	channels[TURN_CHANNEL] = buf[TURN_CHANNEL];
	channels[POWER_CHANNEL] = buf[POWER_CHANNEL];
	
}



int Receiver::getTurn()
{
	
	return channels[TURN_CHANNEL]  - 127;
	
}

int Receiver::getPower()
{
	
	return channels[POWER_CHANNEL];
	
}

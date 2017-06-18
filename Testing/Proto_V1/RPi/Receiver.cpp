#include "Receiver.h"
#include "ControlsProcessing.h"
#include <msgpack.hpp>
#include <unordered_map>

#define TURN_CHANNEL  1
#define POWER_CHANNEL 0

#define POWER_CORRECTION 127



using namespace std;

	static int8_t channels[17];
	static int port = 6000; 
	static int broadcast = 1;
	static int s;
	static sockaddr_in si_me, si_other;

	static long lastrec = -1;

void Receiver::init()
{
	
	s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	setsockopt(s, SOL_SOCKET, SO_BROADCAST,
				&broadcast, sizeof broadcast);
   
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 100000;
	if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
		perror("Error");
	}
   
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
    //ssize_t len_rec = recvfrom(s, buf, sizeof(buf)-1, 0, (sockaddr *)&si_other, &slen);
	ssize_t len_rec = recvfrom(s, buf, sizeof(buf)-1, 0, NULL, 0);

	if(len_rec < 0)
	{
		cout << "fail " << lastrec << endl;
		return;
	}
	
	cout << "no fail " << lastrec << endl;
	
	//for(int i = 0; i < len_rec; i++)
	//	cout << (int)buf[i] << endl;
		
	
	msgpack::object_handle oh = msgpack::unpack(buf, len_rec);

	msgpack::object obj = oh.get();
	
	std::unordered_map<std::string, uint64_t> a = obj.as<std::unordered_map<std::string, uint64_t>>();
	
	for(std::unordered_map<std::string, uint64_t>::iterator itr = a.begin(); itr != a.end(); itr++)
	{
		std::cout << itr->first << " : " << itr->second << std::endl;
	
		ControlsProcessing::setParam(itr->first, itr->second);
		
		lastrec ++;
	}
	
	cout << "update" << endl;

	//for(int i = 0; i < 17; i++)
	//	channels[i] = buf[i];
//	channels[TURN_CHANNEL] = buf[TURN_CHANNEL];
//	channels[POWER_CHANNEL] = buf[POWER_CHANNEL];
	
}


int8_t Receiver::getChannel(int id)
{

	return channels[id];
	
}

int Receiver::getTurn()
{
	
	return channels[TURN_CHANNEL]  - 127;
	
}

int Receiver::getPower()
{
	
	return channels[POWER_CHANNEL];
	
}

#include "stdafx.h"
#include "ControlsHandler.h"

void ControlsHandler::setVector(float x, float y, float z, float mag, VECTOR_TYPE v)
{
	current.x = x;
	current.y = y;
	current.z = z;
	current.mag = mag;
	current.v = v;

}

void ControlsHandler::setVector(Vector v)
{

	current = v;

}

Vector ControlsHandler::getVector()
{
	return current;
}

void ControlsHandler::setArmState(bool state)
{
	arm_state = state;
}

void ControlsHandler::setDisarmState(bool state)
{
	disarm_state = state;
}


char* ControlsHandler::getMessagePackBuffer(size_t * size)
{
	//Start packing boys!

	msgpack::sbuffer buffer;

	msgpack::packer<msgpack::sbuffer> pk(&buffer);

	pk.pack_map(7);

	pk.pack(std::string(X_VECTOR_TAG));
	pk.pack(reinterpret_cast<uint32_t &>(current.x));

	pk.pack(std::string(Y_VECTOR_TAG));
	pk.pack(reinterpret_cast<uint32_t &>(current.y));

	pk.pack(std::string(Z_VECTOR_TAG));
	pk.pack(reinterpret_cast<uint32_t &>(current.z));

	pk.pack(std::string(MAG_VECTOR_TAG));
	pk.pack(reinterpret_cast<uint32_t &>(current.mag));

	pk.pack(std::string(VECTOR_TYPE_TAG));
	pk.pack(reinterpret_cast<uint8_t &>(current.v));

	pk.pack(std::string(ARM_TAG));
	pk.pack(reinterpret_cast<uint8_t &>(arm_state));

	pk.pack(std::string(DISARM_TAG));
	pk.pack(reinterpret_cast<uint8_t &>(disarm_state));

	//Save the data to a new buffer.
	
	char* retBuff = new char[buffer.size()];

	size_t s = (buffer.size());

	*size = s;

	int i = 0;

	for (; i < buffer.size(); i++)
		retBuff[i] = buffer.data()[i];

	return retBuff;

	//return (char *)ss.str().data();
	
}

void ControlsHandler::putMessagePackBuffer(char* buffer, size_t s)
{
	
	msgpack::object_handle oh =
		msgpack::unpack(buffer, s);

	msgpack::object obj = oh.get();
	std::cout << obj << std::endl;  

	std::unordered_map<std::string, uint64_t> a = obj.as<std::unordered_map<std::string, uint64_t>>();

 	for (std::unordered_map<std::string, uint64_t>::iterator itr = a.begin(); itr != a.end(); itr++)
	{
		//std::cout << itr->first << " : " << itr->second << std::endl;

		setParam(itr->first, itr->second);

	}

	std::cout << "<" << current.x << " , " << current.y << " , " << current.z << ">" << std::endl;

	std::cout << "Magnitude: " << current.mag << std::endl;

	std::cout << "Type: " << current.v << std::endl;

}

void ControlsHandler::setParam(std::string tag, uint64_t val)
{

	if (tag == X_VECTOR_TAG)
	{
		current.x = reinterpret_cast<float &>(val);
	}
	else if (tag == Y_VECTOR_TAG)
	{
		current.y = reinterpret_cast<float &>(val);
	}
	else if (tag == Z_VECTOR_TAG)
	{
		current.z = reinterpret_cast<float &>(val);
	}
	else if (tag == MAG_VECTOR_TAG)
	{
		current.mag = reinterpret_cast<float &>(val);
	}
	else if (tag == VECTOR_TYPE_TAG)
	{
		current.v = reinterpret_cast<VECTOR_TYPE &>(val);
	}
	else
	{
		//Because we're safe.
	}
}
#pragma once
#include <iostream>
#include <sstream>
#include <msgpack.hpp>
#include <string.h>

// Vector tags
#define X_VECTOR_TAG "v_x"
#define Y_VECTOR_TAG "v_y"
#define Z_VECTOR_TAG "v_z"
#define MAG_VECTOR_TAG "v_mag"

// Type tags
#define VECTOR_TYPE_TAG "t_vect"

// Control tags
#define ARM_TAG "c_arm"
#define DISARM_TAG "c_disarm"


enum VECTOR_TYPE { Direction, Acceleration, Velocity, Position };

MSGPACK_ADD_ENUM(VECTOR_TYPE)

struct Vector {
	float x;
	float y;
	float z;
	float mag;
	VECTOR_TYPE v;
};

class ControlsHandler
{
public:

	void setVector(float x, float y, float z, float mag, VECTOR_TYPE v);
	
	void setVector(Vector v);

	Vector getVector();

	void setArmState(bool state);

	bool getArmState();

	void setDisarmState(bool state);

	bool getDisarmState();

	/*Get the messagepack buffer to send*/
	char* getMessagePackBuffer(size_t * size);

	/*Recieve a messagepack buffer to parse*/
	void putMessagePackBuffer(char * buffer, size_t s);

	void setParam(std::string tag, uint64_t val);

	void setParam(std::string tag, VECTOR_TYPE type);

private:

	Vector current;
	bool arm_state = false;
	bool disarm_state = false;

};
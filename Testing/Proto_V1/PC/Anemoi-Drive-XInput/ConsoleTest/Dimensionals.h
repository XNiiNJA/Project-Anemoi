/**
*	dimensionals.h
*	This header file defines data structures to organize three dimensional concepts
*	Author: Grant Oberhauser
*/

#pragma once
#ifndef DIMENSIONALS_H
#define DIMENSIONALS_H

struct quaternion
{
	float a;
	float b;
	float c;
	float d;
};

struct point
{
	float x;
	float y;
	float z;
};

//A more "earth-minded" way of thinking about location
struct location
{
	double latitude;
	double longitude;
	double altitude;
};

#endif

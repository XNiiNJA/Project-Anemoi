/**
*	orientationController
*	This header defines the implementation of the orientation controller. The orientation controller is responsible for 
*   figuring out individual motor outputs for the multicopter. This motor outputs are simply corrections applied on top of existing outputs.
*	Author: Grant Oberhauser 
*/
#ifndef ORIENTATIONCONTROLLER_H
#define ORIENTATIONCONTROLLER_H

#include "dimensionals.h"
#include "AbstractServo.h"
#include "MotorManager.h"
#include "controller.h"
#include "sensorhub.h"

class OrientationController
{

public:

	/*Initializes the controller with the number of motors (currently only supports an even number of motors)*/
	static void init();

	static void update();

	/*Gets the output of the motor in the specified position*/
	static float getMotorOutput(int motor);

	/*Set the orientation for the multicopter to control to in the form of a vector*/
	static void setOrientation(point o);

	/*Set the heading the multicopter should try to aim at.*/
	static void setHeading(float h);

private:

  static Controller * motorControllers;

};


#endif

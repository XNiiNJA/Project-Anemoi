/**
*	MotorManager.h
*	Manages the motors on the multicopter. Apply power to individual motors, or to all motors.
*	Author: Grant Oberhauser
*/
#ifndef MOTORMANAGER_H
#define MOTORMANAGER_H


#include "AbstractServo.h"
#include "OrientationController.h"

#define MOTOR_COUNT 4

class MotorManager
{

public:

	/*Initializes the manager*/
	static void init();

	/*Register a new motor for control with an id*/
	//void registerMotor(int id, AbstractServo * s);

  static void update();

	/*Arms all of the motors and allows them all to be controlled*/
	static void armAll();

	/*Disarms all of the motors and does not allow them to be controlled*/
	static void disarmAll();

	/*Sets the power in the form of a float from 0 to 100*/
	static void setBasePower(float p);

	/*Set a trim relative to the base power. Will take the base power and add the trim*/
	static void setMotorTrim(int id, float trim);

	static point getMotorLocation(int motor);

	static int getMotorCount();

	//static void setMotorTrim(AbstractServo * s, float trim);


private:


	//static AbstractServo * servoList[MOTOR_COUNT];

	//static float basePower;


};


#endif

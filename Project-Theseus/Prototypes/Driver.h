#ifndef DRIVER_H
#define DRIVER_H

#include <wiringPi.h>
#include <wiringPiI2C.h>
//#include "../../../wiringPi/wiringPi/wiringPi.h"
//#include <wiringPi/softServo.h>
//#include "../../../wiringPi/wiringPi/softServo.h"
#include <softPwm.h>
#include <iostream>
#include <stdint.h>

#define TURN_RIGHT_PIN 	4
#define TURN_LEFT_PIN 	3

#define MAX_TURN 		65
#define TURN_PWM_RANGE 	100

#define MAX_DRIVE 1024
#define MAX_SERVO 1250


#define DRIVE_PIN 		1

using namespace std;

class Driver
{

public:

	static void init();

	static void update();

	static void setDriveSpeed(int drivePercent);

	static void turnAmount(int8_t amount);

	static void turnRight();
	
	static void turnLeft();
	
	static void turnStraight();

private:

	static int min(int a, int b){if(a < b) return a; else return b;}

	static int rightPWM;
	static int leftPWM;
	static int drivePWM;
	
};

#endif

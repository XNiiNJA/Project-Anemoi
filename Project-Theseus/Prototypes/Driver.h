#ifndef DRIVER_H
#define DRIVER_H

#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>

#define TURN_RIGHT_PIN 	4
#define TURN_LEFT_PIN 	3

#define MAX_TURN 		65
#define TURN_PWM_RANGE 	100

#define MAX_DRIVE 1024


#define DRIVE_PIN 		1

using namespace std;

class Driver
{

public:

	static void init();

	static void update();

	static void setDriveSpeed(int drivePercent){drivePWM = min(MAX_DRIVE, (drivePercent/100.0f) * MAX_DRIVE);}

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

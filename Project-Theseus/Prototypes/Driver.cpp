#include "Driver.h"

int Driver::rightPWM = 0;
int Driver::leftPWM = 0;
int Driver::drivePWM = 0;

void Driver::init()
{
	
	wiringPiSetup () ;
		
	softPwmCreate (TURN_RIGHT_PIN, 0, 100) ;
	softPwmCreate (TURN_LEFT_PIN, 0, 100) ;
	
	pinMode (1, PWM_OUTPUT);
	
	rightPWM = leftPWM = drivePWM = 0;
	
}

void Driver::update()
{
		
	softPwmWrite(TURN_LEFT_PIN, leftPWM);
	
	softPwmWrite(TURN_RIGHT_PIN, rightPWM);
	
	pwmWrite(DRIVE_PIN, drivePWM);
	
}

void Driver::turnStraight()
{
	rightPWM = 0;
	leftPWM = 0;
}

void Driver::turnRight()
{
	rightPWM = MAX_TURN;
	leftPWM = 0;
}

void Driver::turnLeft()
{
	rightPWM = 0;
	leftPWM = MAX_TURN;
}


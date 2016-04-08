#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include <ctime>
#include <cmath>
#include "Driver.h"

#define TURN_RIGHT_PIN 	4
#define TURN_LEFT_PIN 	3

#define DRIVE_PIN 		1

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

#define MAX_TURN 65


int right_pin = 0;

int left_pin = 0;

int drive_pin = 0;


using namespace std;

void turnStraight();

void turnRight();

void turnLeft();

int main (void)
{
	
	Driver::init();
	
	cout << "Driver Initialized";
	
	for(;;)
	{
		
		if(millis() % 10000 < 3333)
			Driver::turnRight();
		else if(millis() % 10000 < 6666)
			Driver::turnLeft();
		else
			Driver::turnStraight();
		
		Driver::update();
		
	}
	return 0 ;
}

void turnStraight()
{
	cout << "Straight";
	//right_pin = LOW;
	//left_pin = LOW;
	right_pin = 0;
	left_pin = 0;
	
	
}

void turnRight()
{
	cout << "Right";
	//right_pin = HIGH;
	//left_pin = LOW;
	right_pin = MAX_TURN;
	left_pin = 0;
}

void turnLeft() 
{
	cout << "Left";
	//right_pin = LOW;
	//left_pin = HIGH;
	right_pin = 0;
	left_pin = MAX_TURN;
	
}


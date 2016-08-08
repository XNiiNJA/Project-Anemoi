#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include <ctime>
#include <cmath>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Driver.h"
#include "Receiver.h"

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

void exit_handler(int s){
	
	Driver::setDriveSpeed(0);
	Driver::turnStraight();
	
	Driver::update();
	
	exit(0);
	
}

int main (void)
{
	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = exit_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;	

	sigaction(SIGINT, &sigIntHandler, NULL);
	
	cout << "Initilized Handler";
	
	Driver::init();
	
	Driver::turnStraight();
	
	Driver::setDriveSpeed(0);
	
	cout << "Driver Initialized";
	
	Receiver::init();
	
	cout << "Receiver Initialized";
	
	cout << "Starting in 5 seconds...";
	
	int startTime = millis();
	
	while(millis() - startTime < 5000);
	
	
	for(;;)
	{
		
		Driver::update();
		
		Receiver::update();
		
		int turn_amount = Receiver::getTurn();
		
		int power_amount = Receiver::getPower();
		
		Driver::turnAmount(turn_amount);
		
		cout << endl << turn_amount << endl;
		
		Driver::setDriveSpeed(((float)power_amount/(float)255) * 100);
		
		
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
	right_pin = 0;
	left_pin = MAX_TURN;	
}


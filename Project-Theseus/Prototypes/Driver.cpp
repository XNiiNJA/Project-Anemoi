#include "Driver.h"

int Driver::rightPWM = 0;
int Driver::leftPWM = 0;
int Driver::drivePWM = 0;

void Driver::init()
{
	
	wiringPiSetup () ;
	
#ifdef DC_DRIVE
	
	softPwmCreate (TURN_RIGHT_PIN, 0, 100) ;
	softPwmCreate (TURN_LEFT_PIN, 0, 100) ;
	
	pinMode (1, PWM_OUTPUT);
	
	rightPWM = leftPWM = drivePWM = 0;

#else

	softServoSetup(DRIVE_PIN, 0, 0, 0, 0, 0, 0, 0);
	

#endif
	
}

void Driver::update()
{
		
	//softPwmWrite(TURN_LEFT_PIN, leftPWM);
	
	//softPwmWrite(TURN_RIGHT_PIN, rightPWM);
	
	//pwmWrite(DRIVE_PIN, drivePWM);
	
	softServoWrite(DRIVE_PIN, drivePWM);
	
}

/**
 * Positive amount means turn right, negative means turn left
 */
void Driver::turnAmount(int8_t amount)
{
	
	if(amount > 0)
	{
		rightPWM = min(MAX_TURN, amount);
		leftPWM = 0;
		
	}
	else if(amount < 0)
	{
		rightPWM = 0;
		leftPWM = -min(MAX_TURN, amount);;
	}
	else
	{
		rightPWM = 0;
		leftPWM = 0;
	}
	
}

void Driver::setDriveSpeed(int drivePercent){
	drivePWM = min(MAX_DRIVE, ((float)drivePercent/100.0f) * MAX_DRIVE); 
	cout << "current power" << drivePWM << endl;
}

void Driver::turnStraight()
{
	rightPWM = 0;
	leftPWM = 0;
	cout << "turn straight\n";
}

void Driver::turnRight()
{
	rightPWM = MAX_TURN;
	leftPWM = 0;
	cout << "turn right\n";
}

void Driver::turnLeft()
{
	rightPWM = 0;
	leftPWM = MAX_TURN;
	cout << "turn left\n";
}


/**
*	AbstractServo.c
*	Abstracts servo concepts to make controlling them more intuitive. 
*	Author: Grant Oberhauser
*/

#include "AbstractServo.h"
#include <Servo.h>

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

/*Initialize the servo, giving maximum and minimum operational values, and more simple external values. Along with the pin the servo is on*/
AbstractServo::AbstractServo(int low, int high, int pin, float rangeMin, float rangeMax)
{

	lowMicroseconds = low;

	highMicroseconds = high;

	motorPin = pin;

	lowRange = rangeMin;

	highRange = rangeMax;

	enabled = false;

  started = false;

  servoObj = new Servo();

  servoObj->attach(pin);

  arming = false;

  armTime = 0;

  currentPower = 0;
  
}

void AbstractServo::update()
{

  if(!(arming && millis() - armTime < 1000))
  {
    arming = false;
    
    int range = highMicroseconds - lowMicroseconds;
    servoObj->writeMicroseconds((int)(lowMicroseconds + range * ((float)currentPower/(float)highRange)));
    
  }
  else
  {
    servoObj->writeMicroseconds(0);
  }
}

/*Allow the servo to take commands*/
void AbstractServo::enable()
{

	servoObj->attach(motorPin);
  servoObj->writeMicroseconds(0);

  arming = true;

  armTime = millis();

	enabled = true;

}

/*Set to 0 and disable the servo*/
void AbstractServo::disable()
{

  servoObj->detach();
	servoObj->writeMicroseconds(0);

	enabled = false;

}

/*Set the power using a float between the min and max values.*/
void AbstractServo::setPower(float p)
{

  	/*int range = highMicroseconds - lowMicroseconds;
   
    Serial.println((int)(lowMicroseconds + range * ((float)p/(float)highRange)));
    Serial.println(servoObj->attached());
    Serial.println(motorPin);*/

    //lastTime = millis();
	  //servoObj->writeMicroseconds((int)(lowMicroseconds + range * ((float)p/(float)highRange)));
    currentPower = p;
    started = true;
    //Serial.println(currentPower);
 
  
}

/*Write the raw milliseconds value to the servo*/
/*void AbstractServo::writeMicroseconds(int m)
{
  started = true;
	servoObj->writeMicroseconds(m);

}*/


void AbstractServo::setLocation(point p)
{


	location = p;

}

point AbstractServo::getLocation()
{

	return location;

}

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

#define LOW_RESPONSE 637

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

  servoObj = Servo();

  arming = false;

  armTime = 0;

  currentPower = 0;
  
}

void AbstractServo::update()
{

  if(enabled)
  {
    arming = false;
    
    int range = highMicroseconds - lowMicroseconds;
    int totPower = (int)(lowMicroseconds + range * ((float)currentPower/(float)highRange));
    
    totPower = max(LOW_RESPONSE, totPower);//Keep it just a hair above the low.
    
    totPower = min(highMicroseconds, totPower); //Make sure to keep it low enough.
          
    if(newVal && totPower != oldPower)
    {
      noInterrupts();
      servoObj.writeMicroseconds(totPower);
      interrupts();
      oldPower = totPower;
      newVal = false;
    }
    
  }
  else
  {
   //Do nothing. 
  }
  

  
  
}

/*Allow the servo to take commands*/
void AbstractServo::enable()
{
 
  servoObj.attach(motorPin);
  servoObj.writeMicroseconds(LOW_RESPONSE);

  arming = true;

  armTime = millis();

  enabled = true;

}

/*Set to 0 and disable the servo*/
void AbstractServo::disable()
{

  servoObj.writeMicroseconds(0);
  servoObj.detach();
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
    if(p < lowRange)
      p = lowRange;
    else if(p > highRange)
      p = highRange;
    
    currentPower = p;
    started = true;
    newVal = true;
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

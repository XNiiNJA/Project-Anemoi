/**
*	MotorManager.c
*	Manages the motors on the multicopter
*	Author: Grant Oberhauser
*/

#include "MotorManager.h"

#define MIN_MICROSECS 600

#define MAX_MICROSECS 2000

int servoPins[] = {2, 3, 4, 5, 6, 7};

static AbstractServo * servoList[MOTOR_COUNT];

static float basePower;

/*Initializes the motors.
  Currently the motors are all hard coded in. 
  Later they will be read from an external source so they can be modified
*/
void MotorManager::init()
{

	//servoList = new AbstractServo[MOTOR_COUNT];

	for(int i = 0; i < MOTOR_COUNT; i++)
	{

		servoList[i] = new AbstractServo(MIN_MICROSECS, MAX_MICROSECS, servoPins[i], 0.0, 100.0);

	}
 
  OrientationController::init();

  point orient;
  orient.x = 0;
  orient.y = 0;
  orient.z = 1;

  OrientationController::setOrientation(orient);

  point location; 

  location.z = 0;
  location.x = -24;
  location.y = 20;

  servoList[0]->setLocation(location);

  location.x = 24;
  location.y = 20;
  
  servoList[1]->setLocation(location);
  
  location.x = 24;
  location.y = -20;
  
  servoList[2]->setLocation(location);
  
  location.x = -24;
  location.y = -20;
  
  servoList[3]->setLocation(location);

  basePower = 0.0f;
  


}

void MotorManager::update()
{


  OrientationController::update();

  //Serial.println("Updated Orientation");

  float or1 = OrientationController::getMotorOutput(0);
  float or2 = OrientationController::getMotorOutput(1);
  float or3 = OrientationController::getMotorOutput(2);
  float or4 = OrientationController::getMotorOutput(3);

  Serial.print(or1);
  Serial.print(" , ");
  Serial.print(or2);
  Serial.print(" , ");
  Serial.print(or3);
  Serial.print(" , ");
  Serial.print(or4);
  Serial.println(" , ");
  
  for(int i = 0; i < MOTOR_COUNT; i++)
  {
    float power1 = (float)((sin((float)(millis() / 4000.0f)) + 1) / 2) * 100 + 5;
    float power2 = (float)((sin((float)((millis() + 1000)/ 4000.0f)) + 1) / 2) * 100 + 5;
    float power3 = (float)((sin((float)((millis() + 2000) / 4000.0f)) + 1) / 2) * 100 + 5;
    float power4 = (float)((sin((float)((millis() + 3000) / 4000.0f)) + 1) / 2) * 100 + 5;       

    setMotorTrim(0, or1);
    setMotorTrim(1, or2);
    setMotorTrim(2, or3);
    setMotorTrim(3, or4);
        
    //setMotorTrim(i, OrientationController::getMotorOutput(i));
    servoList[i]->update();
  }

  
}


/*Arms all of the motors and allows them all to be controlled*/
void MotorManager::armAll()
{

	for(int i = 0; i < MOTOR_COUNT; i++)
	{

		servoList[i]->enable();

	}

}

/*Disarms all of the motors and does not allow them to be controlled*/
void MotorManager::disarmAll()
{

	for(int i = 0; i < MOTOR_COUNT; i++)
	{

		servoList[i]->disable();

	}


}



/*Sets the power in the form of a float from 0 to 100*/
void MotorManager::setBasePower(float p)
{

	basePower = p;

	for(int i = 0; i < MOTOR_COUNT; i++)
	{
		servoList[i]->setPower(p);
	}

}


/*Set a trim relative to the base power. Will take the base power and add the trim*/
void MotorManager::setMotorTrim(int id, float trim)
{

	if(id >= 0 && id < MOTOR_COUNT)
		servoList[id]->setPower(basePower + trim);

}



point MotorManager::getMotorLocation(int motor)
{

	if(motor >= 0 && motor < MOTOR_COUNT)
		return servoList[motor]->getLocation();

}


int MotorManager::getMotorCount()
{
  return MOTOR_COUNT;
}



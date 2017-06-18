/**
*	MotorManager.c
*	Manages the motors on the multicopter
*	Author: Grant Oberhauser
*/

#include "MotorManager.h"
#include "logging.h"

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

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

  //0-----1
  //   |
  //   |
  //3-----2


  OrientationController::setOrientation(orient);

  point location; 

  location.z = 0.0;
  location.x = -0.24;
  location.y = 0.20;

  servoList[0]->setLocation(location);

  location.x = 0.24;
  location.y = 0.20;
  
  servoList[1]->setLocation(location);
  
  location.x = 0.24;
  location.y = -0.20;
  
  servoList[2]->setLocation(location);
  
  location.x = -0.24;
  location.y = -0.20;
  
  servoList[3]->setLocation(location);

  basePower = 0.0f;
  


}

void MotorManager::update()
{


  OrientationController::update();

  //Logging::logln("Updated Orientation");

  float or1 = OrientationController::getMotorOutput(0);
  float or2 = OrientationController::getMotorOutput(1);
  float or3 = OrientationController::getMotorOutput(2);
  float or4 = OrientationController::getMotorOutput(3);

//  setMotorTrim(0, power1);
//  setMotorTrim(1, power2);
//  setMotorTrim(2, power3);
//  setMotorTrim(3, power4);

/*  Logging::log(or1);
  Logging::log(" , ");
  Logging::log(or2);
  Logging::log(" , ");
  Logging::log(or3); 
  Logging::log(" , ");
  Logging::logln(or4);*/


    setMotorTrim(0, or1);
    setMotorTrim(1, or2);
    setMotorTrim(2, or3);
    setMotorTrim(3, or4);
/*
  Logging::log(".................................");
  Logging::log(or1);
  Logging::log(".....");
  Logging::logln(or2);

  Logging::log(".................................");
  Logging::log(or4);
  Logging::log(".....");
  Logging::logln(or3);*/
  
/*  Serial.print("(");
  Serial.print(or1);
  Serial.print(")   (");
  Serial.print(or2);
  Serial.print(")\n\n\n");
  Serial.print("(");
  Serial.print(or4);
  Serial.print(")   (");
  Serial.print(or3);  
  Serial.print(")\n\n----------------\n\n");*/
  
  for(int i = 0; i < MOTOR_COUNT; i++)
  {     
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

	/*for(int i = 0; i < MOTOR_COUNT; i++)
	{
		servoList[i]->setPower(p);
	}*/

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



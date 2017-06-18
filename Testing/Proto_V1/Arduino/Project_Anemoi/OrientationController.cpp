/**
*	OrientationController.cpp
*   The implementation of the orientation controller
*	Author: Grant Oberhauser
*/

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "logging.h"
#include "OrientationController.h"

//Oscillation period (with P=700 half props): 2 Seconds
//Consistant oscillation: 700 w/ half props


#define MOTOR_P 1150//411//205
//#define MOTOR_P 0
//#define MOTOR_I 205
#define MOTOR_I 0

#define MOTOR_D 00//51
//#define MOTOR_D 1


//The default algorithm is left-handed. To convert to right-handed just multiply by -1.
//Right-Handed: -1
//Left-Handed :  1
#define HANDEDNESS -1 


static point globalOrientationVector;

Controller * OrientationController::motorControllers;

static int motorCount;

void OrientationController::init()
{
  
	motorControllers = new Controller[MotorManager::getMotorCount()];


	for(int i = 0; i < MotorManager::getMotorCount(); i++)
	{

		motorControllers[i].init(MOTOR_P, MOTOR_I, MOTOR_D, 100.0f, -100.0f);
                motorControllers[i].forceDeltaT();
      
	}

        globalOrientationVector.x = 0; 
        globalOrientationVector.y = 0; 
        globalOrientationVector.z = 1; 

}

long last_update_time = 0;

void OrientationController::update()
{

  
	point relativeVector = SensorHub::globalToLocal(globalOrientationVector);

        float deltaT = ((float)micros() - last_update_time)/1000000.0f;
        	
        last_update_time = micros();
        
        float inverseDistance = 0;
        
        if(relativeVector.y != 0 || relativeVector.x != 0 || relativeVector.z != 0)
          inverseDistance =  KalmanFilter::invSqrt(relativeVector.x * relativeVector.x + relativeVector.y * relativeVector.y + relativeVector.z * relativeVector.z);
        
        float targetAngle = acos(relativeVector.z * inverseDistance);/// sqrt(relativeVector.x * relativeVector.x + relativeVector.y * relativeVector.y + relativeVector.z * relativeVector.z));

        float relativeXYSquare = 1/((relativeVector.x * relativeVector.x) + (relativeVector.y * relativeVector.y));

//        Serial.print("Target Angle: ");
//        Serial.print(targetAngle);
  
//        Serial.print(" ; ");
  
/*        Serial.print("<");
        Serial.print(relativeVector.x);
        Serial.print(" , ");
        Serial.print(relativeVector.y);
        Serial.print(" , ");
        Serial.print(relativeVector.z);        
        Serial.println(">");
*/      


        for(int i = 0; i < MotorManager::getMotorCount(); i++)
	{
          float error = 0;
          //if(targetAngle >= 0.01)
          //{
        	point motorLoc = MotorManager::getMotorLocation(i);

		float motorX = motorLoc.x;
		float motorY = motorLoc.y;
		float motorZ = motorLoc.z;

		float intersectionX = (relativeVector.y * (relativeVector.y * motorX - relativeVector.x * motorY)) * relativeXYSquare;

		float distX = (intersectionX - motorX);

		float intersectionY = (relativeVector.x * (relativeVector.x * motorY - motorX * relativeVector.y)) * relativeXYSquare;

		float distY = (intersectionY - motorY);

		float radius = sqrt(distX * distX + distY * distY);

		float motorAngle = acos((relativeVector.x * motorX + relativeVector.y * motorY + relativeVector.z * motorZ)
                                    /(sqrt(motorX * motorX + motorY * motorY + motorZ * motorZ)) * inverseDistance) - (M_PI_2);

		error = targetAngle * radius * motorAngle;// * HANDEDNESS;
//                Serial.print(error);
//                Serial.print(" , ");

          //}

          motorControllers[i].disableSetpointLimits();

	  motorControllers[i].setSetpoint(0);

	  motorControllers[i].setCurrentValue(-error);

          motorControllers[i].setP(MOTOR_P);

          motorControllers[i].setI(MOTOR_I);

          motorControllers[i].setD(MOTOR_D);

          motorControllers[i].setDeltaT(deltaT);

          motorControllers[i].update();
          



//          Serial.print(motorControllers[i].getOutput());
//          Serial.print(" , ");
          
	}

 // Serial.println();

/*  Serial.print(relativeVector.x);
  Serial.print(" , ");
  Serial.print(relativeVector.y);
  Serial.print(" , ");  
  Serial.print(relativeVector.z);
  Serial.print(" , ");
  Serial.println();*/

}

void OrientationController::setOrientation(point o)
{

  //convert to unit vector
  float magnitude = sqrt(o.x * o.x + o.y * o.y + o.z * o.z);
  
  globalOrientationVector.x = o.x/magnitude;
  globalOrientationVector.y = o.y/magnitude;
  globalOrientationVector.z = o.z/magnitude;

}

void OrientationController::setHeading(float h)
{

	//TODO: Implement this

}

float OrientationController::getMotorOutput(int motor)
{
	motorControllers[motor].getOutput();

}

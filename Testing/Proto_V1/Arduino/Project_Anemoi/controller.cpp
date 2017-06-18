/**
*	controller.c
*	Implementation of a generic controller
*	Author: Grant Oberhauser
*/

#include "controller.h"
#include "logging.h"


#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

long last_time = 0;

float curProp = 0.0f, curInti = 0.0f, curDir = 0.0f;

Controller::Controller()
{
  Pval = 0; 
  Ival = 0; 
  Dval = 0;
  curProp = 0;
  lastError = 0;
}

/*Basic setup*/
void Controller::init(float PParam, float IParam, float DParam)
{

  Pval = anotherVal = PParam;
  Dval = DParam;
  Ival = IParam;

  last_time = micros();

}

/*Setup that caps the setpoints to avoid runaway errors due to unrealistic requests.*/
void Controller::init(float PParam, float IParam, float DParam, float maxSetpoint, float minSetpoint)
{

  Pval = PParam;
  Dval = DParam;
  Ival = IParam;

  maxSet = maxSetpoint;
  minSet = minSetpoint;

  setpointLimitsSet = true;
  
}

/*Run a pass through the control loop*/
void Controller::update()
{

  float delta;

  last_time = micros();
  
  if(deltaTForced)
  {
    delta = deltaT; 
  }
  else
  {
    delta = ((float)micros() - last_time)/1000000.0f;
  }
  
  lastError = curProp;

  curProp = curSetpoint - curReading;

  curDir = (curProp - lastError)/delta;

  //Dis a trapezoidal rule. Come at me brah.
  curInti += (delta/2) * (lastError + curProp);//curProp;

  Logging::log(".............................................DELTA: ");
  Logging::log(curProp);
  Logging::log(" - ");
  Logging::logln(lastError);


  if(setpointLimitsSet && curInti > maxSet)
  {
    curInti = maxSet;
  }
  else if(setpointLimitsSet && curInti < minSet)
  {
    curInti = minSet;
  }

}

/*Set a new setpoint to control to.*/
void Controller::setSetpoint(float p)
{

  if(setpointLimitsSet && p < minSet)
    p = minSet;
  else if(setpointLimitsSet && p > maxSet)
    p = maxSet;

  curSetpoint = p;

}

/*Sets the maximum and minimum setpoints available.*/
void Controller::applySetpointLimits(float max, float min)
{

  maxSet = max;
  minSet = min;
  setpointLimitsSet = true;

}

/*Get the output of the control loop*/
float Controller::getOutput()
{

  float rawOut = curProp * Pval + curInti * Ival + curDir * Dval;

  Logging::log(".............................................Final Delta: ");
  Logging::logln(curDir * Dval);

/*  if(rawOut > maxSet)
    rawOut = maxSet;
  else if(rawOut < minSet)
    rawOut = minSet;*/
    
  return rawOut;

}

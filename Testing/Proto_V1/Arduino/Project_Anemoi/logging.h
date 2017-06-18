/**
*	SensorHub.h
*	Header file that takes in sensor readings and calculates orientations
*	Author: Grant Oberhauser
*/

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#ifndef LOGGING_H
#define LOGGING_H


//Comment out to disable logging.
//#define DEBUG

class Logging
{

public: 

	static void log(char * line){
#ifdef DEBUG
            Serial.print(line);
#endif
        }

	static void log(float line){
#ifdef DEBUG
            Serial.print(line);
#endif
        }
        
	static void logln(char * line){
#ifdef DEBUG
            Serial.println(line);
#endif
        }

	static void logln(float line){
#ifdef DEBUG
            Serial.println(line);
#endif
        }
        
        static void logVector(float x, float y, float z)
        {
        
          Serial.print(x);
          Serial.print(" , ");
          Serial.print(y);
          Serial.print(" , ");
          Serial.print(z);          
        }
        

};

#endif

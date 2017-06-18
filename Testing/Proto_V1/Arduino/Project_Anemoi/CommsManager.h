/**
*	CommsManager.h
*	Manages communications with a master controller.  
*	Author: Grant Oberhauser
*/

#ifndef COMMSMANAGER_H
#define COMMSMANAGER_H

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define ARM_COMMAND       0x1u
#define DISARM_COMMAND    0x2u
#define THROTTLE_COMMAND  0x3u
#define ORIENT_COMMAND    0x4u
#define ORIENT_COMMAND_X  0
#define ORIENT_COMMAND_Y  1
#define ORIENT_COMMAND_Z  2
#define ACCEL_COMMAND     0x5u
#define VELOCITY_COMMAND  0x6u
#define POSITION_COMMAND  0x7u

#define SLAVE_ADDRESS 0x4

#define NUM_COMMANDS 5 
#define MAX_COMMAND_SIZE 6
#define ERROR_CORRECTION_SIZE 1

#define INCOMING_SIZE 10

#define SPI_BUFFER_LENGTH 100

class CommsManager
{

public:

        static void init();

        /*
        * Takes all bytes recieved since the last update and processes them.
        */
        static void update();

        /*
        * Super quick function used in the SPI interupt. 
        * Only inserts the byte into a buffer and increments count and end_pos
        */
        static void addByte(uint8_t b);


private:

        static volatile uint8_t length, start_pos, end_pos;

        static uint8_t spibuf[SPI_BUFFER_LENGTH];
        
        static uint8_t commands[NUM_COMMANDS][MAX_COMMAND_SIZE + ERROR_CORRECTION_SIZE];
    
        static boolean constructing;

        static uint8_t current_command;
        static uint8_t current_command_pos;
  

};

#endif

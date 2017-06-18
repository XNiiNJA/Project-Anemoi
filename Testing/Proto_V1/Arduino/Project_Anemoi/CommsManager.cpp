
#include "CommsManager.h"

#include "OrientationController.h"
#include "MotorManager.h"


uint8_t CommsManager::spibuf[SPI_BUFFER_LENGTH];

volatile uint8_t CommsManager::length, CommsManager::start_pos, CommsManager::end_pos;

uint8_t CommsManager::commands[NUM_COMMANDS][MAX_COMMAND_SIZE + ERROR_CORRECTION_SIZE];

boolean CommsManager::constructing;

uint8_t CommsManager::current_command; 

uint8_t CommsManager::current_command_pos;

void CommsManager::init()
{
  
 for(int i = 0; i < SPI_BUFFER_LENGTH; i ++)
   spibuf[i] = 0;
   
/* for(int i = 0; i < NUM_COMMANDS; i++)
   for(int j = 0; j < MAX_COMMAND_SIZE + ERROR_CORRECTION_SIZE; j++)
     commands[i][j] = 0;
   */
 length = start_pos = end_pos = 0;
 
 constructing = false;
  
 current_command = current_command_pos = 0;
  
}

void CommsManager::update()
{
  
  if(start_pos == end_pos)//There's nothing here for us.
    return;
  
  uint8_t incoming[INCOMING_SIZE];
  
  memset(incoming, 0, sizeof(incoming));
  
  while(start_pos != end_pos)
  {
    //Get the next byte
    uint8_t curByte = spibuf[start_pos];
    
    //Maintain the length and start position.
    length--;

    start_pos++;
    
    //We reached the end of the buffer, wrap back to the front.
    if(start_pos >= SPI_BUFFER_LENGTH)
      start_pos = 0;
    
    //Are we currently constructing a command?
    if(!constructing)
    {
      //We're at the start of a new command!
      constructing = true;
      
      current_command = curByte;
      current_command_pos = 0;
      
    }
    else
    {

      //Add to the current command
      commands[current_command][current_command_pos] = curByte;
    
      current_command_pos++;
      
    }
    
    if(current_command_pos >= MAX_COMMAND_SIZE + ERROR_CORRECTION_SIZE)
    {
      //We are done with this command, clean up time.
      constructing = false;
    
//      uint8_t error_correction_loc = MAX_COMMAND_SIZE + ERROR_CORRECTION_SIZE - 1;
      
//      uint8_t parity_bit = commands[current_command][error_correction_loc];
  
      incoming[current_command] = 1;
      
    }
    
  }
  
  //Once we're done with going through the buffers, we can start commanding based on what we recieved. 
  uint32_t throttleBytes = 0;
  float throttlePower;
  
  
  for(int i = 0; i < INCOMING_SIZE; i++)
  {
    if(incoming[i] == 1)
    {
      switch(i)
      {
      
        case ORIENT_COMMAND:
          
          point cmdPoint;
          
          cmdPoint.x = reinterpret_cast<volatile int8_t&>(commands[ORIENT_COMMAND][ORIENT_COMMAND_X]);    //reinterpret_cast<int8_t&>(x);//reinterpret_cast<int8_t&>(buffer[ORIENT_COMMAND_X - 1]);
          cmdPoint.y = reinterpret_cast<volatile int8_t&>(commands[ORIENT_COMMAND][ORIENT_COMMAND_Y]);    //reinterpret_cast<int8_t&>(y);//reinterpret_cast<int8_t&>(buffer[ORIENT_COMMAND_Y - 1]);
          cmdPoint.z = reinterpret_cast<volatile int8_t&>(commands[ORIENT_COMMAND][ORIENT_COMMAND_Z]);    //reinterpret_cast<int8_t&>(z);//reinterpret_cast<int8_t&>(buffer[ORIENT_COMMAND_Z - 1]);
          
          OrientationController::setOrientation(cmdPoint);      
          
        break;
        
        case ARM_COMMAND:
          
  
          if(commands[ARM_COMMAND][0] == 1)
          {
            MotorManager::armAll();
          } 
          
        break;
        
        case DISARM_COMMAND:
        
          if(commands[DISARM_COMMAND][0] == 1)
            MotorManager::disarmAll();
        
        break;
        
        case THROTTLE_COMMAND:
          
          throttleBytes = 0;
          throttleBytes = commands[THROTTLE_COMMAND][0];
          throttleBytes |= ((uint32_t)commands[THROTTLE_COMMAND][1] << 8);
          throttleBytes |= ((uint32_t)commands[THROTTLE_COMMAND][2] << 16);
          throttleBytes |= ((uint32_t)commands[THROTTLE_COMMAND][3] << 24);
          
          throttlePower = reinterpret_cast<float&>(throttleBytes);
          
          MotorManager::setBasePower(throttlePower);
        
        break;
        
      }
      
    }
      
  }

  
    

}

void CommsManager::addByte(uint8_t b)
{
  
  spibuf[end_pos] = b;
  
  if(++end_pos >= SPI_BUFFER_LENGTH)
    end_pos = 0;
    
  if(end_pos == start_pos && length > 0)
    start_pos++;
  else
    length++;
  
}


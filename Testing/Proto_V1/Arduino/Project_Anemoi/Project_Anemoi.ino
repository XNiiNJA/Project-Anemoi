#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include "kalmanfilter.h"
#include <math.h>
#include <Servo.h>
#include <SPI.h> // Included for SFE_LSM9DS0 library
#include <WSWire.h>
#include "dimensionals.h"
#include "quatops.h"
#include "sensorhub.h"
#include "MotorManager.h"
#include "AbstractServo.h"

#include <SparkFunLSM9DS1.h>



#define NUM_MOTORS 4


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
#define MAX_COMMAND_SIZE 10

//#define VERBOSE

float relativeVectorX, relativeVectorY, relativeVectorZ;

int globalVectorX, globalVectorY, globalVectorZ;

uint16_t gyroX, gyroY, gyroZ;

uint16_t curRotX, curRotY, curRotZ;

double targetAngle;

point motorPositions[NUM_MOTORS];

float errors[NUM_MOTORS];

long loops = 0;

int command = -1;

uint8_t commands[NUM_COMMANDS][MAX_COMMAND_SIZE];

void setup() {

  Serial.begin(9600);

  SensorHub::init();

 // initialize i2c as slave
 Wire.begin(SLAVE_ADDRESS);

  delay(1000);

  //positions in nearest cm.
  //41 cm long
  //48 cm across
//       Front          |
//   0            1     |
//                      41 cm
//   2            3     |
//        BACK          |
//                      |
//   <------48 cm------->

  motorPositions[0].x = -24;
  motorPositions[0].y = 20;
  motorPositions[0].z = 0;

  motorPositions[1].x = 24;
  motorPositions[1].y = 20;
  motorPositions[1].z = 0;

  motorPositions[2].x = -24;
  motorPositions[2].y = -20;
  motorPositions[2].z = 0;
  
  motorPositions[3].x = 24;
  motorPositions[3].y = -20;
  motorPositions[3].z = 0;

  MotorManager::init();

  //MotorManager::setBasePower(10);
  
  point o;
  o.x = 0;
  o.y = 0;
  o.z = 1;
  
  
  OrientationController::setOrientation(o);
  
   Wire.onReceive(receiveData);

}



//Receive control data from Raspberry Pi here.
void receiveData(int byteCount){
  uint8_t commandByte;
  uint8_t buffer[100];
  uint8_t i = 0u;
  point cmdPoint;
  uint32_t throttleBytes;
  float throttlePower;
 
  
  commandByte = Wire.read();
  
  while(i < byteCount - 1) {
    //buffer[i] = Wire.read();
    commands[commandByte][i++] = Wire.read();
  }
  
  command = commandByte;
  
 /* command = commandByte;
  uint8_t x = Wire.read();
  ///
  uint8_t y = Wire.read();
  
  uint8_t z = Wire.read();*/
 
  switch(commandByte)
  {
  
    case ORIENT_COMMAND:
      
      cmdPoint.x = reinterpret_cast<int8_t&>(commands[ORIENT_COMMAND][ORIENT_COMMAND_X]);    //reinterpret_cast<int8_t&>(x);//reinterpret_cast<int8_t&>(buffer[ORIENT_COMMAND_X - 1]);
      cmdPoint.y = reinterpret_cast<int8_t&>(commands[ORIENT_COMMAND][ORIENT_COMMAND_Y]);    //reinterpret_cast<int8_t&>(y);//reinterpret_cast<int8_t&>(buffer[ORIENT_COMMAND_Y - 1]);
      cmdPoint.z = reinterpret_cast<int8_t&>(commands[ORIENT_COMMAND][ORIENT_COMMAND_Z]);    //reinterpret_cast<int8_t&>(z);//reinterpret_cast<int8_t&>(buffer[ORIENT_COMMAND_Z - 1]);
      
      OrientationController::setOrientation(cmdPoint);      
      
      command = 1;
      
    break;
    
    case ARM_COMMAND:
    
      if(commands[ARM_COMMAND][0] == 1)
        MotorManager::armAll();
    
    break;
    
    case DISARM_COMMAND:
    
      if(commands[DISARM_COMMAND][0] == 1)
        MotorManager::disarmAll();
    
    break;
    
    case THROTTLE_COMMAND:
      
      throttleBytes = commands[THROTTLE_COMMAND][0];
      throttleBytes |= ((uint32_t)commands[THROTTLE_COMMAND][1] << 8);
      throttleBytes |= ((uint32_t)commands[THROTTLE_COMMAND][2] << 16);
      throttleBytes |= ((uint32_t)commands[THROTTLE_COMMAND][3] << 24);
      
      throttlePower = reinterpret_cast<float&>(throttleBytes);
      
      MotorManager::setBasePower(throttlePower);
      
      
      break;
    
    default:
    
      command = commandByte;
    
    break;
      
   
  }
  
  while(Wire.available())
    Wire.read();
 

}





long loopStart = 0;

uint32_t throttBytes;

float throttFloatTest;
  
void loop() {

  Serial.println(millis() - loopStart);
  
/*  if(millis() - loopStart > 300)
    I2C_ClearBus();*/
  
  loopStart = millis();

  SensorHub::update();

//MotorManager::setBasePower((float)((sin((float)(millis() / 1000.0f)) + 1) / 2) * 10 + 5);
  
  MotorManager::update();

  if(command != -1)
  {
    Serial.print("Got ");
    //Serial.println(reinterpret_cast<int8_t&>(commands[ORIENT_COMMAND][ORIENT_COMMAND_X]));
    Serial.println(command);
/*    if(command == THROTTLE_COMMAND)
    {
      Serial.println("THROTTLE COMMANND!@@@!@#@##@@##$@#$$@#@#$@$#");
      Serial.print(commands[THROTTLE_COMMAND][0]);
      Serial.print(" , ");
      Serial.print(commands[THROTTLE_COMMAND][1]);
      Serial.print(" , ");
      Serial.print(commands[THROTTLE_COMMAND][2]);
      Serial.print(" , ");
      Serial.print(commands[THROTTLE_COMMAND][3]);
      Serial.println(" , ");
      
      throttBytes = commands[THROTTLE_COMMAND][0];
      throttBytes |= ((uint32_t)commands[THROTTLE_COMMAND][1] << 8);
      throttBytes |= ((uint32_t)commands[THROTTLE_COMMAND][2] << 16);
      throttBytes |= ((uint32_t)commands[THROTTLE_COMMAND][3] << 24);
      
      //throttFloatTest = reinterpret_cast<float&>(throttBytes);
      
      //memcpy(&throttFloatTest, &throttBytes, sizeof throttFloatTest);
      
      throttFloatTest = *(float *)&throttBytes;
      
      Serial.print(throttFloatTest, 6);
      Serial.println(" Power Level ");
      
    }*/
    command = -1;
  }
  
  
 
  
  
  /*
  point targetPoint;
  targetPoint.x = 0;
  targetPoint.y = 0;
  targetPoint.z = 1;

  point relVect = SensorHub::globalToLocal(targetPoint);


  relativeVectorX = relVect.x;
  relativeVectorY = relVect.y;
  relativeVectorZ = relVect.z;

  targetAngle = acos(relativeVectorZ / sqrt(relativeVectorX * relativeVectorX + relativeVectorY * relativeVectorY + relativeVectorZ * relativeVectorZ));

  for(int i = 0; i < NUM_MOTORS; i++)
  {

    int16_t motorX = motorPositions[i].x;
    int16_t motorY = motorPositions[i].y;
    int16_t motorZ = motorPositions[i].z;

    float intersectionX = (relativeVectorY * (relativeVectorY * motorX - relativeVectorX * motorY))/((relativeVectorX * relativeVectorX) + (relativeVectorY * relativeVectorY));

    float distX = (intersectionX - motorX);

    float intersectionY = (relativeVectorX * (relativeVectorX * motorY - motorX * relativeVectorY))/ ((relativeVectorX * relativeVectorX) + (relativeVectorY * relativeVectorY));

    float distY = (intersectionY - motorY);

    float radius = sqrt(distX * distX + distY * distY);

    float motorAngle = acos((relativeVectorX * motorX + relativeVectorY * motorY + relativeVectorZ * motorZ)/(sqrt(relativeVectorX * relativeVectorX + relativeVectorY * relativeVectorY + relativeVectorZ * relativeVectorZ) * sqrt(motorX * motorX + motorY * motorY + motorZ * motorZ))) - (M_PI_2);

    errors[i] = targetAngle * radius * motorAngle;
  }

  long dur = millis() - loopStart;*/

#ifdef VERBOSE

  Serial.print("------Errors Calculated-----\n");

  for(int i = 0; i < NUM_MOTORS; i++)
  {

    Serial.print("Motor (");
    Serial.print(i);
    Serial.print("): ");
    Serial.print(errors[i]);
    Serial.println("\n");
    
  }*/

//       Front          |
//   0            1     |
//                      y
//   2            3     |
//        BACK          |
//                      |
//   <------x------->


  Serial.print("Front:  ");


  Serial.print(errors[0]);
  Serial.print("     ");
  Serial.print(errors[1]);

  Serial.print("   Back:  ");

  Serial.print(errors[2]);
  Serial.print("     ");
  Serial.print(errors[3]);

  Serial.print("                             ");


  
  p("\r");

  

#else
    
   // loadJunk();

#endif

  //loadJunk();
  
  
}

//Loads data other than zeros for serious benchmarking
void loadJunk()
{
//       Front          |
//   0            1     |
//                      y
//   2            3     |
//        BACK          |
//                      |
//   <------x------->
 
 /*motorPositions[0][0] = -24; //X
 motorPositions[0][1] = 20; //Y
 motorPositions[0][2] = 0; //Z

 motorPositions[1][0] = 24; 
 motorPositions[1][1] = 20;
 motorPositions[1][2] = 0;

 motorPositions[2][0] = -24; 
 motorPositions[2][1] = -20;
 motorPositions[2][2] = 0;

 motorPositions[3][0] = 24; 
 motorPositions[3][1] = -20;
 motorPositions[3][2] = 0;*/

 relativeVectorX = 20;//random(1, 200);
 relativeVectorY = 10;//random(1, 200);
 relativeVectorZ = 10;//random(1, 200);

#ifdef VERBOSE

  /*Serial.print("\nRelative Vectors\n");

  Serial.println(relativeVectorX);
  Serial.println(relativeVectorY);
  Serial.println(relativeVectorZ);
*/
#endif

}

int I2C_ClearBus() {
#if defined(TWCR) && defined(TWEN)
  TWCR &= ~(_BV(TWEN)); //Disable the Atmel 2-Wire interface so we can control the SDA and SCL pins directly
#endif

  pinMode(SDA, INPUT_PULLUP); // Make SDA (data) and SCL (clock) pins Inputs with pullup.
  pinMode(SCL, INPUT_PULLUP);

  delay(2500);  // Wait 2.5 secs. This is strictly only necessary on the first power
  // up of the DS3231 module to allow it to initialize properly,
  // but is also assists in reliable programming of FioV3 boards as it gives the
  // IDE a chance to start uploaded the program
  // before existing sketch confuses the IDE by sending Serial data.

  boolean SCL_LOW = (digitalRead(SCL) == LOW); // Check is SCL is Low.
  if (SCL_LOW) { //If it is held low Arduno cannot become the I2C master. 
    return 1; //I2C bus error. Could not clear SCL clock line held low
  }

  boolean SDA_LOW = (digitalRead(SDA) == LOW);  // vi. Check SDA input.
  int clockCount = 20; // > 2x9 clock

  while (SDA_LOW && (clockCount > 0)) { //  vii. If SDA is Low,
    clockCount--;
  // Note: I2C bus is open collector so do NOT drive SCL or SDA high.
    pinMode(SCL, INPUT); // release SCL pullup so that when made output it will be LOW
    pinMode(SCL, OUTPUT); // then clock SCL Low
    delayMicroseconds(10); //  for >5uS
    pinMode(SCL, INPUT); // release SCL LOW
    pinMode(SCL, INPUT_PULLUP); // turn on pullup resistors again
    // do not force high as slave may be holding it low for clock stretching.
    delayMicroseconds(10); //  for >5uS
    // The >5uS is so that even the slowest I2C devices are handled.
    SCL_LOW = (digitalRead(SCL) == LOW); // Check if SCL is Low.
    int counter = 20;
    while (SCL_LOW && (counter > 0)) {  //  loop waiting for SCL to become High only wait 2sec.
      counter--;
      delay(100);
      SCL_LOW = (digitalRead(SCL) == LOW);
    }
    if (SCL_LOW) { // still low after 2 sec error
      return 2; // I2C bus error. Could not clear. SCL clock line held low by slave clock stretch for >2sec
    }
    SDA_LOW = (digitalRead(SDA) == LOW); //   and check SDA input again and loop
  }
  if (SDA_LOW) { // still low
    return 3; // I2C bus error. Could not clear. SDA data line held low
  }

  // else pull SDA line low for Start or Repeated Start
  pinMode(SDA, INPUT); // remove pullup.
  pinMode(SDA, OUTPUT);  // and then make it LOW i.e. send an I2C Start or Repeated start control.
  // When there is only one I2C master a Start or Repeat Start has the same function as a Stop and clears the bus.
  /// A Repeat Start is a Start occurring after a Start with no intervening Stop.
  delayMicroseconds(10); // wait >5uS
  pinMode(SDA, INPUT); // remove output low
  pinMode(SDA, INPUT_PULLUP); // and make SDA high i.e. send I2C STOP control.
  delayMicroseconds(10); // x. wait >5uS
  pinMode(SDA, INPUT); // and reset pins as tri-state inputs which is the default state on reset
  pinMode(SCL, INPUT);
  return 0; // all ok
}


#include <stdarg.h>
void p(char *fmt, ... ){
        char buf[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(buf, 128, fmt, args);
        va_end (args);
        Serial.print(buf);
}


/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo servo1;  // create servo object to control a servo
Servo servo2;  // create servo object to control a servo
Servo servo3;  // create servo object to control a servo
Servo servo4;  // create servo object to control a servo
Servo servo5;  // create servo object to control a servo
Servo servo6;  // create servo object to control a servo

int output = 0; 
int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

void setup() {
  servo1.attach(2);
  servo2.attach(3);
  servo3.attach(4);
  servo4.attach(5);
  servo5.attach(6);
  servo6.attach(7);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  Serial.begin(9600);
}

void loop() {

  if(Serial.available() > 0)
  {
     output = Serial.parseInt();
     Serial.println(output);
  }
  servo1.writeMicroseconds(output);                  // sets the servo position according to the scaled value
  servo2.writeMicroseconds(output);                  // sets the servo position according to the scaled value
  servo3.writeMicroseconds(output);                  // sets the servo position according to the scaled value
  servo4.writeMicroseconds(output);                  // sets the servo position according to the scaled value
  servo5.writeMicroseconds(output);                  // sets the servo position according to the scaled value
  servo6.writeMicroseconds(output);                  // sets the servo position according to the scaled value
}


/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int output = 0; 
int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

void setup() {
  myservo.attach(3);  // attaches the servo on pin 9 to the servo object
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
  myservo.writeMicroseconds(output);                  // sets the servo position according to the scaled value
}


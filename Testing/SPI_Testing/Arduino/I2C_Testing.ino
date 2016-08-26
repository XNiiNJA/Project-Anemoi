#include <Wire.h>

const byte MY_ADDRESS = 43;   // me
const byte LED = 13;          // LED is on pin 13

byte ledVal = 0;

void setup() {
  // put your setup code here, to run once:
  Wire.begin (MY_ADDRESS);  // initialize hardware registers etc.

  TWAR = (MY_ADDRESS << 1) | 1;  // enable broadcasts to be received

  Wire.onReceive(receiveEvent);  // set up receive handler
  pinMode(LED, OUTPUT);          // for debugging, allow LED to be flashed
}

void loop() {
  // put your main code here, to run repeatedly:

}

void receiveEvent(int howMany)
{

  Serial.print("Bytes Received: ");
  Serial.print(howMany);
  
}


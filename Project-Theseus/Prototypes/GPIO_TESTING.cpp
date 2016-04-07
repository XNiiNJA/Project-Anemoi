#include <wiringPi.h>
#include <iostream>
#include <ctime>
#include <cmath>
//#include <conio.h>

#define TURN_RIGHT_PIN 	4
#define TURN_LEFT_PIN 	3

#define DRIVE_PIN 		1

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77


int right_pin = 0;

int left_pin = 0;

int drive_pin = 0;


using namespace std;

void turnStraight();

void turnRight();

void turnLeft();

int main (void)
{
	
	
	cout << "Test";
	
	wiringPiSetup () ;
	pinMode(TURN_RIGHT_PIN, OUTPUT) ;
	pinMode(TURN_LEFT_PIN, OUTPUT); 
	pinMode (1, PWM_OUTPUT);


	
/*	digitalWrite(0, HIGH);
	
	delay (500) ;

	digitalWrite(0, LOW);
	
	delay (500) ;*/
	
	int c;
	
  for (;;)
  {
	 c = 0;
	  
	 c = getch();
	  
	 if(c == KEY_UP)
	 {
		 pwmWrite(1, 1024);
	 }
	 else if(c == KEY_DOWN)
		pwmWrite(1, 0);
	 
	 if(c == KEY_RIGHT)
		turnRight();
		
	 else if(c == KEY_LEFT)
		turnLeft();
	
	else
			turnStraight();
	  
	  
	  
    /*//digitalWrite (0, HIGH) ; delay (5000) ;
    //digitalWrite (0,  LOW) ; delay (5000) ;
    
    if(millis() % 10000 < 3300)
		turnRight();
	else if(millis() % 10000 < 6600)
		turnLeft();
	else
		turnStraight();
		
    digitalWrite(TURN_LEFT_PIN, left_pin);
    
    digitalWrite(TURN_RIGHT_PIN, right_pin);
    
    
    int write = (sin((double)((millis()) / (double)1000) ) + 1)/2.0 * 1024;
    
    //cout << write << endl;
    
    pwmWrite(1, write);
    
   /* pinMode(1, PWM_OUTPUT);
pwmSetMode(PWM_MODE_MS); 
pwmSetClock(384); //clock at 50kHz (20us tick)
pwmSetRange(1000); //range at 1000 ticks (20ms)
pwmWrite(1, 75);  //theretically 50 (1ms) to 100 (2ms) on my servo 30-130 works ok
*/
    
  }
  return 0 ;
}

void turnStraight()
{
	cout << "Straight";
	right_pin = LOW;
	left_pin = LOW;
	
}

void turnRight()
{
	cout << "Right";
	right_pin = HIGH;
	left_pin = LOW;
	
}

void turnLeft() 
{
	cout << "Left";
	right_pin = LOW;
	left_pin = HIGH;

	
}


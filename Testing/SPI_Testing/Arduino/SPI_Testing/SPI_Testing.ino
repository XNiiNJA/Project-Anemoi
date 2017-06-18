
// Written by Nick Gammon
// February 2011
/**
 * Send arbitrary number of bits at whatever clock rate (tested at 500 KHZ and 500 HZ).
 * This script will capture the SPI bytes, when a '\n' is recieved it will then output
 * the captured byte stream via the serial.
 */

#include <SPI.h>

char buf [100];
volatile byte pos;
volatile boolean process_it;

#define MAX_COMMAND_SIZE 5

void setup (void)
{
  Serial.begin (115200);   // debugging

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  
  // get ready for an interrupt 
  pos = 0;   // buffer empty
  process_it = false;

  // now turn on interrupts
  SPI.attachInterrupt();

}  // end of setup


// SPI interrupt routine
ISR (SPI_STC_vect)
{
  

  byte c = SPDR;  // grab byte from SPI Data Register
  
  // add to buffer if room
  if (pos < sizeof buf)
    {
    buf [pos++] = c;
    
    // example: newline means time to process buffer
//    if (c == '\n')
     if(pos > MAX_COMMAND_SIZE)
      process_it = true;
      
    }  // end of room available
    
}  // end of interrupt routine SPI_STC_vect

// main loop - wait for flag set in interrupt routine
void loop (void)
{

  if (process_it)
    {
      for(int i = 0 ; i < MAX_COMMAND_SIZE; i++)
      {
        Serial.print((int)buf[i]);
        Serial.print(",");   
        buf[i] = 0;   
      }
      Serial.println();
      pos = 0;
//    Serial.println (buf);
//    buf [pos--] = 0;  
      process_it = false;
    }  // end of flag set
    
}  // end of loop

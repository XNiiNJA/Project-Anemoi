# Proto_V1

Prototype version 1 is basically a proving ground that the concept is sound, and the math checks out. This is _supposed_ to be a networking project, but quadcopters require a lot of control theory to get it to simply fly. And control theory is hard work.

So how does this system work? 

## How this system works.

This system is composed of three parts. All parts are programmed in C++

### Windows

The first part is a base station that runs on Windows. This program communicates with the quadcopter over a LAN for now. Hole punching with UDP is easy, and I've done testing with rendezvousing on a server with pretty good results. That was with video streaming, but sending the control packets should be very similar. The Windows system sends the data to the Raspberry pi on the quadcopter, and formats it with msgpack. This lets the packets stay small, while also allowing easy parsing. It also frees up the system from requiring definitions of what every byte of a packet corresponds to. The windows program also uses X-Input in order to get input from an Xbox One controller. The input from the controller is translated to vector-based controls and sent across the network. For now, it's just trying to control the orientation of the multicopter. 

### Raspberry Pi

The second part of the system is a Raspberry Pi that recieves messages from the Windows program and parses it. From the data it figures out what type of control it is recieving. In the final iteration of this system, the user will be able to control orientation, acceleration, velocity, and position. So the Raspberry Pi figures out what type of control is happening and sends it to the Arduino. 

### Arduino

The arduino recieves the data from the Raspberry Pi over SPI. It recieves data through and interrupt, puts it on a buffer, and continues with the control loop. The reason why there is an Arduino in this system, is because an Arduino is truly real-time. While the Raspberry Pi _might_ be able to control the system on it's own, a real-time system is much prefered. The Arduino works on controlling the quadcopter at all times. There are no other tasks that are running that will interrupt it. Except for recieving new control information. This control loop can only run up to 120 Hz on an Arduino Uno. I have purchased a Teensy to test with in hopes of faster processing times, but those tests have yet to be conducted. 



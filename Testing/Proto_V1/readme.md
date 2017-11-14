# Proto_V1

Prototype version 1 is basically a proving ground that the concept is sound, and the math checks out. This is _supposed_ to be a networking project, but quadcopters require a lot of control theory to get it to simply fly. And control theory is hard work.

So how does this system work? 

## How this system works.

This system is composed of three parts. The first part is a base station that runs on Windows. This communicates with the quadcopter over a LAN for now. Hole punching with UDP is easy, and I've done testing with rendezvousing on a server.

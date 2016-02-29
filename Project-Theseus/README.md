# Project-Icarus

This guide outlines the underlying mathematics of the control scheme used in the Icarus project. The mathematics used in this project heavily use 3d vectors, lines, and planes. Previous knowledge of such concepts is advised, however this guide will offer quick refreshers when needed for every new concept. 

###Section One: Philosophy

Project Icarus is tasked with creating a new control scheme for latency heavy operation of unmanned multirotors. This control scheme is needed when trying to remotely control multicopters over a cellular connection or other packet switched networks. This is a departure from the already prevalent radio controlled multicopters which can rely on very low latency. Project Icarus, however, needs a semi-autonomous control scheme which is capable of intuitive control. Basically, Project Icarus needs to depart from the popular idea of allowing the pilot to use pitch, roll, and yaw. Instead, Project Icarus draws inspiration from the hit video game series Halo and the control scheme used in "Forge Mode". The aformentioned control scheme is very stable and also very intuitive. It does not involve roll, pitch, or yaw. Project Icarus instead places a layer of abstraction over pitching, rolling, and yawing, and instead allows the pilot to focus on the controls: forward, backward, left, right, up, and down. 



###Section Two: Control Concepts

This section covers the broad concepts used in controlling the multicopter. For more in-depth math, the next section will cover all of the math needed to implement a Project Icarus controller. 

This controller was built with customization in mind. We needed a platform on which we could easily add more motors, change dimensions of the craft, and even add more sensors as necessary. Internally, the multicopter's frame is represented by a plane which is, for lack of a better term, centered at the origon. When the velocity of the craft needs to be changed, a new vector is issued to the controller. This vector is used as the normal for a plane that contains a number of motor representations, the controller will attempt to match the current motor representations to the goal motor representations. It attempts to change the motor locations by using one PID controller for each motor. When the motor gets closer to its goal representation its error gets decreased, and when it gets further away it increases its error. Negative errors may occur for motors that are "above" its goal representation. When this occurs, the controller has the choice of either trying to reverse the motors, or simply turning them off. Most implementations will simply shut down the motor. Yawing is not factored into this model as yawing is only necessary for human pilots and the human pilot issues controls to yaw whenever they deem necessary. 

Note: PID controllers will also be used to yaw by using a trade off system to make sure overall thrust is kept while also providing yawing action. 


###Section Three: Math

First things first, buckle up. This might take a while to get through. 

First off, we need a space to represent our multicopter. We will be using R^3 space to do this. The center of the multicopter is at (0,0,0), naturally. Any sensors or motors are represented by Sn and Mn respectfully. For any Sn or Mn there should be an x, y, and z coordinate specified. This is the distance from the center of the craft to the sensor or motor in the x, y, or z direction. The positive x direction is the forward direction of the craft, positive y is left, positive z is up. Every other direction can be extrapolated from there. 

The craft will have two representations, the current location represented by the x-y plane, and the future location which will also be specified by a plane of our choosing. The future plane is specified by giving a desired velocity vector relative to the craft. We will call this vector v = <m, n, o>, as is common in physics. However, this velocity vector is not in terms of veloctity, but rather the amount of power that should be sent to the motors as we can't always guarentee reaching a velocity, or maintaing it for that matter. In some implementations that rely on 8 bit integers, this vector will only represent the direction the craft should fly in, and the scalar portion of the velocity, |v|, will be provided in another variable. This allows the system to utilize all of its power in a controlled fasion. This is because if v = <127,127,127> then |v| = 219.97 which is just under 255 which is the maximum unsigned number available for an 8 bit variable.  

The future plane also comes with future representations of any Mn and Sn, these will be notated as FMn and FSn. There are two ways to calculate the error between any Mn and FMn. The first way is to simply use a straight line between the two and find its length and use that as the error, the second way is to calculate the arc length between the Mn and FMn. In order to do such a thing, we should define some equations. 

A plane with normal vector <a,b,c> and including the point (x0,y0,z0) is found by using: a(x-x0) + b(y-y0) + c(z-z0) = 0

The x-y plane has the normal <0,0,1>, and contains the point (0,0,0). It is defined as: z = 0

The future plane has the normal v = <m, n, o> and contains the point (0,0,0). It is defined as: mx + ny + cz = 0

The axis the craft should rotate around is the intersection between these two planes. The intersection of these two planes is:

mx + ny = 0

Solving for y, gets us 

y = -(m/n) x

Likewise, solving for x gets 

x = -(n/m) y

These two equations are important for finding the x and y coordinants of the perpendicular intersection of the axis of rotation and a line that runs through any Mn. This is necessary for finding the radius of rotation around the axis for any Mn.

The x and y coordiantates for any Mn will be defined as x1, y1, and z1.

The line that makes a perpendicular intersection with the axis from any Mn is defined as:

y - y1 = (n/m) * (x - x1)

Solving for y gets us 

y = (n/m) * (x - x1) + y1

Solving for x gets us 

x = (n x1+m y-m y1)/n

With these, we may solve for x and y intersection points

Solving -(m/n) x = (n/m) * (x - x1) + y1 for x gets us

x = (n (n x1-m y1))/(m^2+n^2)


Solving (n x1+m y-m y1)/n = -(n/m) y for y gets us 

y = (m (-n x1+m y1))/(m^2+n^2)

The z coordinate is always 0, so an equation does not need to be defined for it. 

Another important concept is finding the coordinates of any FMn which may be done by using the plane we had defined above: 

 mx + ny + oz = 0

In order to find the mapping of any x or y location you can simply multiply each by a coefficient. 

The coefficient for an x coordinate can be found by the following 

sqrt(1 + (n/o)^2)

y coordinate

sqrt(1 + (m/o)^2)

Mapping the z 






sqrt( (( (-b*(-bx+ay)) / ((-b)^2+a^2)) - x)^2 + ((a*(-bx+ay))/((-b)^2+a^2) - y)^2) * cos^-1(c/(sqrt(a^2+b^2+c^2))




sqrt((a x+b y)^2/(a^2+b^2)) cos^(-1)(c/sqrt(a^2+b^2+c^2))

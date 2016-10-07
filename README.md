# Project-Anemoi

Operates on port ~~58511~~ 20303

This guide outlines the underlying mathematics of the control scheme used in the Anemoi project. The mathematics used in this project heavily use 3d vectors, lines, planes, cross-products, dot products, and trionometry. Previous knowledge of such concepts is advised, however this guide will offer quick refreshers when needed for every new concept. 

###Section One: Philosophy

Project Anemoi is tasked with creating a new control scheme for latency heavy operation of unmanned multirotors. This control scheme is needed when trying to remotely control multicopters over a cellular connection or other packet switched networks. This is a departure from the already prevalent radio controlled multicopters which can rely on very low latency. Project Anemoi, however, needs a semi-autonomous control scheme which is capable of intuitive control. Basically, Project Anemoi needs to depart from the popular idea of allowing the pilot to use pitch, roll, and yaw. Instead, Project Anemoi draws inspiration from the hit video game series Halo and the control scheme used in "Forge Mode". The aformentioned control scheme is very stable and also very intuitive. It does not involve roll, pitch, or yaw. Project Anemoi instead places a layer of abstraction over pitching, rolling, and yawing, and instead allows the pilot to focus on the controls: forward, backward, left, right, up, and down. 



###Section Two: Control Concepts

This section covers the broad concepts used in controlling the multicopter. For more in-depth math, the next section will cover all of the math needed to implement a Project Anemoi controller. 

This controller was built with customization in mind. We needed a platform on which we could easily add more motors, change dimensions of the craft, and even add more sensors as necessary. Internally, the multicopter's frame is represented by a plane which is, for lack of a better term, centered at the origin. When the orientation of the craft needs to be changed, a new vector is issued to the controller. This vector is used as the normal for a plane that contains a number of motor representations, the controller will attempt to match the current motor representations to the goal motor representations. It attempts to change the motor locations by using one PID controller for each motor. When the motor gets closer to its goal representation its error gets decreased, and when it gets further away it increases its error. Negative errors may occur for motors that are "above" its goal representation. This is when total power comes into play. There is a total power variable where, if the multicopter was exactly oriented with the goal, every motor will be outputing that amout of power. The error is then superimposed over the total power for each motor. This means, if there is a positive error, the motor will output total_power + error. Negative error is exactly the opposite with total_power - error. Yawing is not yet factored into this model as yawing is only necessary for human pilots and the human pilot issues controls to yaw whenever they deem necessary. Future iterations will attempt to bring target yaw into the calculations for a more comprehensive contol scheme.

Note: PID controllers will also be used to yaw by using a trade off system to make sure overall thrust is kept while also providing yawing action. 


###Section Three: Math

First things first, buckle up. This might take a while to get through. 

The mathematics used in this control scheme is designed with complete comprehensiveness in mind. However, that wasn't found to be completely achieveable, which will be discussed further down. 

The first concept that needs to be explained is that the target force vector is relative to the craft. If, for example, the craft is to go straight up in the global coordinate system, the vector will have to be translated to the craft's local coordinate system first. If the craft is facing upside down in this example, then the target force vector will be straight down relative to the craft. This guide does not discuss converting between local and global space yet. Although, it is implemented. It just involves quaternions based on sensor input data. It's all quite complicated. If you're really curious, Google is a good source. The Proto_V1 Arduino code also has the _uncommented_ implementation if you're a masochist. 

In order to more easily explain the control scheme, it will be split into four parts
* Finding target angle error.
* Finding rotation arc radius for each motor.
* Finding rotation arc angle for each motor.
* Determine whether the motor shall rise or fall.

#####Part One: Finding target angle error

The first part of the control scheme is relatively simple. Finding the whole craft's error in terms of an angle. 

This error is the angle between the normal vector of the craft and the target vector. 

In order to find this error, the function is 

Cos^-1(Tz/|***T***|)

Where ***T*** = \<Tx, Ty, Tz\> is the target vector relative to the craft.

This function is derived from the definition of the dot product: a * b = |a||b|cos(θ)

Where: a * b is the dot product of a and b. The normal of the craft is assumed to have a magnitude of 1 for simplicity. 

Then, we have T * N = |T|cos(θ), where T is the target vector, and N is the normal vector. 

Taking the dot product between \<Tx, Ty, Tz\> and \<0, 0, 1\> gets Tz.

Now, we have Tz = |T|cos(θ).

It's now obvious that Cos^-1(Tz/|T|) = θ


#####Part Two: Finding rotation arc radius for each motor.

Ix = (Ty * (Ty * Mx - Tx * My))/((Tx * Tx) + (Ty * Ty))

Iy = (Tx * (Tx * My - Mx * Ty))/ ((Tx * Tx) + (Ty * Ty))

Where I = (Ix, Iy) is the point at which the rotation axis intersects a line that is perpendicular to the rotation axis and passes through the motor location at (Mx, My).

**STOP**. Make sure to chew on that sentence for a bit. You want to understand what we're doing here. Let me explain a bit more. 

We have a couple of pieces here. The rotation axis is a line that is perpendicular to both, the target vector and normal vector. It's the line that we will rotate about if we are to get to the orientation we want. In order to find this line, take the cross product of the normal and the target vector. Okay, rotation axis explained. Now, if the multicopter was to rotate perfectly around this line that cuts straight through its center, the path that the motors follow would make a perfect circle. So, we want to find out what the distance is between the rotation axis and the motor in order to find this circle. Basic geometry. So, in order to do this, we need to find a point on the rotation axis that would make an exact right angle if there were a line drawn from the motor to this point. The two equations above define the functions required in order to find this point.

Now, read that first sentence again. Hopefully it makes sense. 

Also, keep in mind, this is all happening in a very small timeslice. The motor will probably only use a little bit of the circle we calculate here. On the next go around of the code, it will all be calculated again.

#####Part Three: Finding rotation arc angle for each motor.

Motor angle is found using the same trick used from above when finding the angle between the normal vector and the target vector. 

In this case, the motor angle is the angle between the target vector and the motor vector. The "motor vector" being the vector that would be created by drawing a point from the origin to a motor.

Cos^-1((Tx * Mx + Ty * My + Tz * Mz)/(sqrt(Tx * Tx + Ty * Ty + Tz * Tz) * sqrt(Mx * Mx + My * My + Mz * Mz)))

This will give an angle between 0 and PI. 

#####Part Four: Determine whether the motor shall rise or fall.

In order to determine if the motor should rise or fall, simply subtract PI/2 from the motor angle. 

This will give a signed angle value. 

#####Bring it all together

error = targetAngle * radius * motorAngle

####High-Level Control Concepts

The math seen above is only the tip of the iceberg. It is, however, the core idea of the entire control concept. With the currently covered math, one would be capable of issuing a vector relative to the multirotor's current orientation. Which, by itself, is relatively useless. We, instead, need a way to issue a point relative to the world. This point would then act as a target for the multirotor. Various control schemes could then be based off this concept. That global point could be an acceleration which is relative to the world. That global point could also be a velocity, or even a position. In fact, sometimes all three will need to be used together. 

For example, if the goal is to control the velocity of the multirotor, then we must also be capable of controlling acceleration of the multirotor. We would control the velocity through the acceleration. This brings us to the use of multiple controllers. A controller on orientation, acceleration, and even velocity. The upper most controller, velocity, feeds into the next controller, acceleration. However, if the goal was to control the multirotor to lock at a location, we would have to add another controller for position. 

This is the downside of this control scheme. Multiple PID controllers working in tandem. If tuned incorrectly, the problem could be very difficult to find. This being said, upper level control settings (i.e. position, velocity) should be capable of switching between different multirotor setups. Sharing control settings could even be facilitated by communities of multirotor enthusiasts in order to find better setups that they enjoy using, and they could even go as far as sharing control settings between different multirotor setups. So, pure startup time is astronomically higher than common control schemes. However, with the use of the internet, the control schemes of others could be easily used and traded between other users. So, multiple controllers could either be thought of as a pro, or a con.

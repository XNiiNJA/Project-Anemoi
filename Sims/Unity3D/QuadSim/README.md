# QUADSIM

Oh boy! A simulation... in Unity? 

Yes, Unity. The reason why Unity is because Unity uses the PhysX physics engine by Nvidia. For a system like a quadcopter, it is good enough. Also, it really helps with visualization and giving an almost 1:1 approximation for how the system will be programmed.

## Usage

The structure of the simulation involves putting multiple controller layers onto the body of the quadcopter. When the simulation starts, activate the higher level controllers first (position, velocity,...) and end with the orientation controller. This lets the upper levels of the system level out before they are used to drive the quadcopter. I should make a video for this...

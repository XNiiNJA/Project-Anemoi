using UnityEngine;
using System.Collections;

/**********************************************************
*
* Script used in the Unity 3D simulation of the 
* Pioneer Rocketry 2016 rocket capable of airbraking. 
*
* Written by: Grant Oberhauser
* Date commented: 1/13/2016
*
**********************************************************/
public class BrakeScript : MonoBehaviour
{

    //The last altitude we were at
    float lastAltitude;
    //The time we logged this altitude
    float lastAltLogTime;
    //The starting altitude for zeroing
    float startAltitude;
    //The acceleration the rocket is experiencing in the y direction (up/down)
    //Unity has y as up/down, even though Calculus has z as up/down usually. Noobs.
    public float acceleration;
    //The minimum amount off acceleration so for. 
    //Useful for logging purposes.
    //Initialized to float.MaxValue so everything is lower than it at first. 
    public float minAccel = float.MaxValue;
    //The speed in the last cycle
    float lastSpeed = 0;
    //The maximum altitude so far
    public float maxAlt = 0;
    //The maximum speed so far
    public float maxSpeed = 0;
    //The altitude where the speed was the highest
    //Also the altitude where the motors stopped burning. 
    //Useful for logging.
    public float altAtMaxSpeed = 0;
    //Not used.
    public float nomAccel;
    //The calculated K value for the rocket WITHOUT flaps deployed
    public float KConstant;
    //The mass of the rocket when the whole motor has been burned out. 
    public float massConstant;
    //Are we using PID? Is this a logging only launch or not?
    public bool usePID;
    //The Proportional tuning contant for the PID controlled
    public float P;
    //The Integral tuning constant
    public float I;
    //The Derivative tuning constant.
    public float D;
    //The output from the PID controller. Could probably be a local variable
    public float pidout;
    //The error between the current projected maximum altitude and our altitude
    public float error;
    //The integral of the error. Initialized to zero. Make sure you do that.
    public float integral = 0;
    //The derivative of the error. 
    public float derivative;
    //The previous error, used to calculate derivative
    float prevError;

   
    //The altitude we want to get as close to possible. 
    public float targetAltitude;

    //The brake object that handles braking. 
    Brakes brake;


    //Initialize everything.
    void Start()
    {
        //Make sure the altitude is zeroed 
        startAltitude = transform.position.y;
        //Get the brake component. In the real program this might
        //Initialize an object that deals with breaks or something. 
        brake = this.GetComponent<Brakes>();
        //Make sure the brakes are off
        brake.setResistForce(0);
        //Make sure the brakes are activated. 
        brake.on = true;

    }

    //FixedUpdate is called every x seconds. In the simulation it is set to run every 0.2 seconds. 
    //This is important to note because of Time.fixedDeltaTime. This should be changed to calculate the 
    //time difference between one cycle of the code and the next.
    void FixedUpdate()
    {


        //Logging the maximum altitude. 
        //Is our current y position higher than maxAlt?
        if(transform.position.y > maxAlt)
        {
            //The current altitude is the highest we've ever been!
            maxAlt = transform.position.y;
        }


        //This is our current speed in the y direction (up)
        float curSpeed = GetComponent<Rigidbody>().velocity.y;

        //Logging max speed
        if (maxSpeed < curSpeed)
        {
            altAtMaxSpeed = transform.position.y;
            maxSpeed = curSpeed;
        }

        //If the brake is on, or applying force 
        //brake.on isn't checking if the brakes are deployed, it's making sure the module is active. 
        //We're using PID, not bang-bang or on-off control 
        if (!brake.on || brake.resistanceForce == 0)
            acceleration = (curSpeed - lastSpeed) / Time.fixedDeltaTime;

        //Logging minimum acceleration.
        minAccel = Mathf.Min(acceleration, minAccel);

        //Our current K constant calculated through the function
        // ((a - g) * m)/ v^2 = k
        //This is only for testing purposes. It should change with the braking. Higher braking percentage means higher k value. 
        float k = ((acceleration - Physics.gravity.y) * massConstant) / (curSpeed * curSpeed);

        Debug.Log("Calculated K: " + k);

        //Are we using PID? We better be. 
        //If we aren't we are probably doing a launch to obtain k values and such.
        if (usePID)
        {
            

            //Calculate what our projected final altitude is. 
            //The function used to find this is
            //(m / (2 * k)) * Log((m * -g + k * v^2)/(m * -g)) + y
            //Where:
            // m = mass in grams(?)
            // k = K constant - This is calculated beforehand and is a constant in the program. 
            // g = acceleration due to gravity. 
            // v = vertical velocity
            // y = current altitude
            float finalAltitude = (massConstant / (2 * KConstant)) * Mathf.Log((massConstant * -Physics.gravity.y + KConstant * curSpeed * curSpeed) / (massConstant * -Physics.gravity.y)) + transform.position.y;

            //Ignore this.
            //-(curSpeed * curSpeed) / (2 * (-9.81f - ((curSpeed * curSpeed * dragConstant)/massConstant))) + transform.position.y;

            //Print some logging stuff out. 
            Debug.Log("Final Alt: " + finalAltitude + " " + (massConstant * -Physics.gravity.y));

            //Are we decelerating? 
            //If we are, the motors aren't firing anymore and braking should be applied.
            if (acceleration < 0)
            {
                //Get the difference between how high we will go and how high we want to go.
                error = finalAltitude - targetAltitude;

                //Take the integral of the error
                integral = integral + error * Time.fixedDeltaTime;

                //Take the derivative of the error 
                derivative = (error - prevError) / Time.fixedDeltaTime;

                //Now apply the P I and D tuning parameters.
                //This will give us an output percentage which may be over 100%. This is fine, and expected. 
                //Just limit it in the brake function, don't limit it here. 
                float output = P * error + I * integral + D * derivative;

                //save the current error so we can calcuate the derivative. 
                prevError = error;

                //Set the braking percentage.
                brake.setResistForce(output);

                //A little bit of debugging. 
                Debug.Log("Output: " + output + ", Error: " + error);


            }

            
            
        }

        //Log the altitude quick so we can calculate vertical velocity
        logAltitude(getAltitude());

        //Log the last speed so we can calculate acceleration.
        lastSpeed = curSpeed;

    }

    //Get the speed by taking the difference between altitude at different times.
    //This works because this is a perfect system. 
    //I'm not sure if it would work the same way in the real world. 
    //Implement as necessary. 
    float getSpeed()
    {

        return (getAltitude() - lastAltitude) / (Time.time - lastAltLogTime);

    }

    //Log the altitude and the time at which is was logged at. 
    void logAltitude(float alt)
    {
        lastAltitude = alt;
        lastAltLogTime = Time.time;
    }

    //Get the current altitude by subtracting the starting altitude from the current altitude. 
    float getAltitude()
    {

        return transform.position.y - startAltitude;

    }
}

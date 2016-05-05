using UnityEngine;
using System.Collections;

public class Accelerometer : MonoBehaviour {

    Vector3 lastVelocity;
    Vector3 lastPos;
    Vector3 lastCalc;
    Vector3 cordAccel;
    Vector3 integratedVelocity;
    Vector3 gravity;
    private bool gravityInit = false;
    private bool firstVelInit = false;
    //Sensitivity of ADXL345 accelerometer
    public float sensitivity = 0.0384f;
    public Vector3 properAcceleration;
    public float properMag;
    public float realMag;
    Rigidbody rigid;
    public float res = .1f;
	// Use this for initialization
	void Start () {

        rigid = GetComponent<Rigidbody>();
	}
	
	// Update is called once per frame
	void FixedUpdate () {

        //y = mx + b
        //b = velocity
        //m = acceleration
        //x = time
        //y = new velocity

        cordAccel = ((transform.InverseTransformVector(rigid.velocity) - lastVelocity) / Time.fixedDeltaTime);


        realMag = cordAccel.magnitude;


        lastCalc = lastPos + transform.TransformVector((lastVelocity + Time.fixedDeltaTime * transform.InverseTransformVector(Physics.gravity)) * Time.fixedDeltaTime);


        //Calculte the displacement between where we should be and where we are. 
        Vector3 displacment = rigid.position - lastCalc;//makeDiscrete(rigid.position, res) - makeDiscrete(lastCalc, res);

        //properAcceleration = transform.InverseTransformDirection(displacment / Time.fixedDeltaTime / Time.fixedDeltaTime);  //Remove delta time for full second units.

        cordAccel = (rigid.velocity - lastVelocity) /  Time.fixedDeltaTime;

        Vector3 tempAccel = transform.InverseTransformVector(cordAccel - Physics.gravity);

        //float finalX = (int)(tempAccel.x / sensitivity) * sensitivity;
        //float finalY = (int)(tempAccel.y / sensitivity) * sensitivity;
        //float finalZ = (int)(tempAccel.z / sensitivity) * sensitivity;

        properAcceleration = makeDiscrete(tempAccel, sensitivity);

        //properAcceleration = new Vector3(finalX, finalY, finalZ);
        

        properMag = properAcceleration.magnitude;

        lastPos = rigid.position;
        lastVelocity = rigid.velocity;
        

	}


    /*
    Converts a vector with float values, to a vector that has float values at every tick. 
    Makes vector math more conistant with reality of sensor data.
    */
   Vector3 makeDiscrete(Vector3 vector, float tick)
    {
        float xVal = ((int)(vector.x / tick)) * tick;
        
        float yVal = ((int)(vector.y / tick)) * tick;

        float zVal = ((int)(vector.z / tick)) * tick;

        return new Vector3(xVal, yVal, zVal);

    }


}

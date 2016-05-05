using UnityEngine;
using System.Collections;

public class AccelerationLayer : MonoBehaviour {

    public Accelerometer accelero;

    public Vector3 neutralState;

    public Vector3 targetVector;

    public FlightBehaviour behave;

    public Rigidbody rigid;

    public Vector3 targetAcceleration;

    public Vector3 processedOutput;

    public bool usePID;

    public float maxAcceleration = 10;

    public Vector3 error;

    private Vector3 lastError;

    public Vector3 derivative;

    public Vector3 integral;
    
    public Vector3 integralAcceleration;

    public float P;

    public float I;

    public float D;

    private bool neutralInit = false;

	// Use this for initialization
	void Start () {

        accelero = gameObject.GetComponent<Accelerometer>();

        behave = gameObject.GetComponent<FlightBehaviour>();

        rigid = gameObject.GetComponent<Rigidbody>();
        

	}
	
	// Update is called once per frame
	void Update () {
	
	}

    void FixedUpdate()
    {

        if (!neutralInit)
        {
            neutralState = transform.TransformVector(accelero.properAcceleration);
            neutralInit = true;
        }


        Vector3 neutralTarget = transform.InverseTransformVector(neutralState + targetVector);

        Vector3 transformedTarget = transform.InverseTransformVector(targetVector);

        Vector3 globalProper = transform.TransformVector(accelero.properAcceleration);

        Vector3 transformedNeutral = transform.InverseTransformVector(neutralState);


        //<0, 9.8, 0> - <0, 0, 3.8>
        //properAcceleration - neutralState = coordinate acceleration
        
        Vector3 outputAcceleration = -(globalProper - 2 * neutralState) + targetVector; //transformedNeutral + transform.InverseTransformVector(targetVector);//transformedNeutral + (accelero.properAcceleration) + transform.InverseTransformVector(targetVector);// + accelero.properAcceleration - transform.InverseTransformVector(targetVector);

        Debug.Log(outputAcceleration + " , " + outputAcceleration.magnitude);



        //targetAcceleration = outputAcceleration - accelero.properAcceleration;

        processedOutput = outputAcceleration;

        if (usePID)
        {
            
            lastError = error;

            //Set Point: targetVector
            //Process Variable: globalProper - neutralState

            error = targetVector - (globalProper - neutralState);//outputAcceleration - globalProper;//((neutralTarget + transform.InverseTransformVector(targetVector)) - accelero.properAcceleration);
            
            derivative = error - lastError;

            integral += error;

            if(integral.magnitude >= maxAcceleration)
            {
                integral = integral.normalized * maxAcceleration;
            }

            float processedX = /*outputAcceleration.x + /**/error.x * P + derivative.x * D + integral.x * I;
            float processedY = /*outputAcceleration.y + /**/error.y * P + derivative.y * D + integral.y * I;
            float processedZ = /*outputAcceleration.z + /**/error.z * P + derivative.z * D + integral.z * I;

            processedOutput = new Vector3(processedX, processedY, processedZ);/**/

            if(processedOutput.magnitude >= maxAcceleration)
            {
                processedOutput = processedOutput.normalized * maxAcceleration;
            }

        }

        //Vector3 gravityIncluded = processedOutput + transformedNeutral;

        //Set the new vectors
        behave.currentPower = processedOutput.magnitude;// * behave.totalMass;
        
        behave.relativeTargetVector = transform.InverseTransformVector(processedOutput);

        integralAcceleration += accelero.properAcceleration - neutralState;

        //-Have + want = output 

        //behave.currentPower = accelero.properMag * 10 * behave.totalMass + behave.currentPower;

    }
}

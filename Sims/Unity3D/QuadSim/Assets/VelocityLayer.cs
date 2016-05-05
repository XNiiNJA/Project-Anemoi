using UnityEngine;
using System.Collections;

public class VelocityLayer : MonoBehaviour {

    Rigidbody rigid;

    public Vector3 targetVelocity;

    AccelerationLayer accLayer;

    public float maxVelocity = 10;

    public Vector3 error;

    public Vector3 derivative;

    public Vector3 integral;

    public float P, I, D;

	// Use this for initialization
	void Start () {
	
        rigid = this.GetComponent<Rigidbody>();

        accLayer = this.GetComponent<AccelerationLayer>();

        error = new Vector3(0, 0, 0);

    }
	
   void FixedUpdate()
    {

        Vector3 lastError = error;
       
        error = targetVelocity - rigid.velocity;

        derivative = error - lastError;
        
        integral += error;

        if (integral.magnitude >= maxVelocity)
            integral = integral.normalized * maxVelocity;


        Vector3 accelerationOutput = error * P + derivative * D + integral * I;

        accLayer.targetVector = accelerationOutput;


    }

}

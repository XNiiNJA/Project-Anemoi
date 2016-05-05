using UnityEngine;
using System.Collections;

public class PositionLayer : MonoBehaviour {

    Rigidbody rigid;

    public bool usePos = true;

    public Vector3 targetPosition;

    public GameObject targetObj;

    VelocityLayer velLayer;

    public float maxSpeed;

    public Vector3 error;

    public Vector3 derivative;

    public Vector3 integral;

    public float P, I, D;

    // Use this for initialization
    void Start()
    {

        rigid = this.GetComponent<Rigidbody>();

        velLayer = this.GetComponent<VelocityLayer>();

        error = new Vector3(0, 0, 0);

    }

    void FixedUpdate()
    {
        
        Vector3 target;

        if (usePos)
            target = targetPosition;
        else
            target = targetObj.transform.position;

        Vector3 lastError = error;

        error = target - transform.position;

        derivative = error - lastError;

        integral += error;

        Vector3 velocityOutput = error * P + derivative * D + integral * I;

        if (velocityOutput.magnitude > maxSpeed)
            velocityOutput = velocityOutput.normalized * maxSpeed;

        velLayer.targetVelocity = velocityOutput;


    }
}

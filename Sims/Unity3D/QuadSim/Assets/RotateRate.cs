using UnityEngine;
using System.Collections;

public class RotateRate : MonoBehaviour {

    private Vector3 lastEuler;

    public float xRate;

    public float yRate;

    public float zRate;

    public float xRotation;

    public float yRotation;

    public float zRotation; 

    private float lastTime;

    public Rigidbody ridgid;

    public Vector3 angleVel;

    public bool useDelta;

    // Use this for initialization
    void Start () {
        //customRotation = Vector3.zero;

        lastTime = Time.time;

        xRate = 0;
        yRate = 0;
        zRate = 0;

	}
	
	// Update is called once per frame
	void FixedUpdate () {

        //if (lastTime == Time.time)
        //    return; 

        angleVel = ridgid.angularVelocity;//gameObject.transform.InverseTransformDirection(ridgid.angularVelocity);

        yRate = transform.localEulerAngles.y - lastEuler.y;
        if (yRate > 180) yRate -= 360;
        if (yRate < -180) yRate += 360;

        zRate = transform.localEulerAngles.z - lastEuler.z;
        if (zRate > 180) zRate -= 360;
        if (zRate < -180) zRate += 360;

        xRate = transform.localEulerAngles.x - lastEuler.x;
        if (xRate > 180) xRate -= 360;
        if (xRate < -180) xRate += 360;


        if (useDelta)
        {
            xRotation = xRate + xRotation;

            yRotation = yRate + yRotation;

            zRotation = zRate + zRotation;
        }
        else
        {
            xRotation = angleVel.x + xRotation;

            yRotation = angleVel.y + yRotation;

            zRotation = angleVel.z + zRotation;
        }

        lastTime = Time.time;

        lastEuler = transform.localEulerAngles;

        

    }

}

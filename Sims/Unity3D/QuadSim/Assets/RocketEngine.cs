using UnityEngine;
using System.Collections;

public class RocketEngine : MonoBehaviour
{


    public float rocketForce;
    public long rocketTime;

    float startTime;
    float curTime;

    float lastAltitude;

    // Use this for initialization
    void Start()
    {
        startTime = Time.time;
    }

    void FixedUpdate()
    {

        curTime = Time.time;

        if (curTime - startTime <= rocketTime)
        {
            Debug.Log("Applying Force");
            applyForce(rocketForce);
        }
        Debug.Log("Current Velocity: " + (Time.fixedDeltaTime * (transform.position.y - lastAltitude)));
        Debug.Log("Time Elapsed: " + (curTime - startTime));
        lastAltitude = transform.position.y;

        

    }

    void applyForce(float forceAmount)
    {
        Rigidbody rigid = this.GetComponent<Rigidbody>();

        //Debug.DrawRay(VectorUp)

        rigid.AddForce(transform.up * forceAmount , ForceMode.Force);

    }

}

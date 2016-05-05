using UnityEngine;
using System.Collections;

public class Brakes : MonoBehaviour {

    public bool on;

    public float BreakKValue = 0;
    
    public float resistanceForce = 9.81f;

    public float maxForce = 9.81f;

    public float mass = 0;

    public float percentOn = 0;

    const float MAX_PERCENT = 100;

    private RealDrag drag;

	// Use this for initialization
	void Start () {

        Time.fixedDeltaTime = 0.015f;

        drag = GetComponent<RealDrag>();

    }
	
	// Update is called once per frame
	void FixedUpdate () {

        if (on)
        {

            drag.addK(BreakKValue * (percentOn/MAX_PERCENT)); 

           /* float resistanceForce = (rigid.velocity.magnitude * rigid.velocity.magnitude * BreakKValue) / mass;

            Debug.DrawRay(transform.position, Vector3.down * resistanceForce);

            rigid.AddForce(Vector3.down * resistanceForce, ForceMode.Acceleration);*/

            

        }

	}

    public void setResistForce(float perc)
    {


        if (perc > 0)
        {
            percentOn = Mathf.Min(perc, MAX_PERCENT);
            //resistanceForce = maxForce * (percentOn / MAX_PERCENT);

        }
        else
            percentOn = resistanceForce = 0;
    }

}

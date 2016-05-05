using UnityEngine;
using System.Collections;

public class QuadInput : MonoBehaviour {

    FlightBehaviour behave;

    public float hRate = 0.1f;

    public float tRate = 250.0f;

    public Vector3 angleVel;

	// Use this for initialization
	void Start () {

        behave = gameObject.GetComponent<FlightBehaviour>();

	}
	
	// Update is called once per frame
	void Update () {

        behave.currentPower += Input.GetAxis("Trottle") * tRate * Time.deltaTime;

        behave.targetGlobalVector.x += Input.GetAxis("Horizontal") * hRate * Time.deltaTime;

        behave.targetGlobalVector.z += Input.GetAxis("Vertical") * hRate * Time.deltaTime;

        if (Input.GetAxis("Horizontal") == 0)
        {
            if(Mathf.Abs(behave.targetGlobalVector.x) < hRate)
                behave.targetGlobalVector.x = 0;
            else if(behave.targetGlobalVector.x < 0)
                behave.targetGlobalVector.x += hRate * Time.deltaTime;
            else if (behave.targetGlobalVector.x > 0)
                behave.targetGlobalVector.x -= hRate * Time.deltaTime;
        }

        if(Input.GetAxis("Vertical") == 0)
        {

            if (Mathf.Abs(behave.targetGlobalVector.z) < hRate)
                behave.targetGlobalVector.z = 0;
            else if (behave.targetGlobalVector.z < 0)
                behave.targetGlobalVector.z += hRate * Time.deltaTime;
            else if (behave.targetGlobalVector.z > 0)
                behave.targetGlobalVector.z -= hRate * Time.deltaTime;

        }

	}
}

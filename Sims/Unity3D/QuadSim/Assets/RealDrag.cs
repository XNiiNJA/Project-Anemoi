using UnityEngine;
using System.Collections;

public class RealDrag : MonoBehaviour {

    public float k;
    private float startk;
    public float mass;

	// Use this for initialization
	void Start () {

        startk = k;
	}
	
	// Update is called once per frame
	void FixedUpdate () {

       Rigidbody rigid = GetComponent<Rigidbody>();

       float dragForce = (rigid.velocity.magnitude * rigid.velocity.magnitude * k)/ mass;

       rigid.AddForce(Vector3.Normalize(rigid.velocity) * -dragForce, ForceMode.Acceleration);

        Debug.Log("Drag Force: " + dragForce + " Y Velocity: " + rigid.velocity.y);

        Debug.DrawRay(transform.position, Vector3.down * dragForce);

    }

    public void addK(float drag)
    {
        k = startk + drag;
    }
}

using UnityEngine;
using System.Collections;

public class LookAt : MonoBehaviour {

    public GameObject lookObject;

    public bool zoom;

    public float distFullOut;

    public float distFullIn;

    public float FOVin;

    public float FOVout;


    Camera cam;




	// Use this for initialization
	void Start () {
        cam = this.GetComponent<Camera>();
    }
	
	// Update is called once per frame
	void Update () {

        this.gameObject.transform.LookAt(lookObject.transform);

        float dist = Vector3.Distance(lookObject.transform.position, this.transform.position);

        Debug.Log(dist);

        if(zoom)
            cam.fieldOfView = Mathf.Min(FOVout, Mathf.Max(  ((distFullOut - dist) * (FOVin - FOVout))/(distFullIn - distFullOut) + FOVin , FOVin));


   }
}

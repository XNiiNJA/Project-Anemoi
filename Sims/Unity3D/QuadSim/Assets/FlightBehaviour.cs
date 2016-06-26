 using UnityEngine;
using System.Collections;

public class FlightBehaviour : MonoBehaviour {

	public Rigidbody parent;

	public GameObject[] objects;

   public bool flipXRots = false;

    public bool flipYRots = false;

    public bool flipZRots = false;

    public bool tryStabalize = false;

	public float currentPower = 1;

	private float forcebase = 1;

    public float maxForce = 1200;

    public bool useTarget;

    public GameObject target;

    public bool useGlobal;

   //So Unity is a very strange child that swapped the Y and Z directions. 
   //So the Y and Z directions are swapped in this script. 
   //Real world applications would swap them back. 
   public Vector3 targetGlobalVector;

    //The target vector relative to the craft. This is what the math will actually use. 
    public Vector3 relativeTargetVector;

    public float realAppliedForce;

    public float totalMass;

    public float PCoef;

    public float ICoef;

    public float DCoef;

    public float[] errors;

    public float[] prevError;

    public float[] proportional;

    public float[] integral;

    public float[] derivative;

    public int[] negs;

    RotateRate rate;

   // Use this for initialization
    void Start () {

        rate = gameObject.GetComponent<RotateRate>();

        errors = new float[objects.Length];

        prevError = new float[objects.Length];

        proportional = new float[objects.Length];

        integral = new float[objects.Length];

        derivative = new float[objects.Length];

        negs = new int[objects.Length];

        for(int i = 0; i < objects.Length; i++)
        {
            proportional[i] = 0;
            integral[i] = 0;
            derivative[i] = 0;
            prevError[i] = 0;
        }

	}
	
	// Update is called once per frame
	void Update () {
        

   }

	void FixedUpdate()
	{

        if (useTarget)
            targetGlobalVector = new Vector3(target.transform.position.x - transform.position.x, target.transform.position.y - transform.position.y, target.transform.position.z - transform.position.z);

        Debug.DrawRay(parent.transform.position, targetGlobalVector * currentPower, Color.red );

        Accelerometer acc = gameObject.GetComponent<Accelerometer>();

        Rigidbody rb = GetComponent<Rigidbody>();

        totalMass = rb.mass + 4.06f;// * 10;

        for(int i = 0; i < objects.Length; i++)
        {

            rb = objects[i].GetComponent<Rigidbody>();

            totalMass += rb.mass;// * 10;

        }
    
        if(tryStabalize)
        currentPower = (acc.properMag * totalMass) / objects.Length;

        float time = Time.time;

        //targetGlobalVector  = new Vector3(200 - parent.transform.position.x, 200 - parent.transform.position.y, 200 - parent.transform.position.z);


        
        
        //relativeTargetVector = convertToLocal(targetGlobalVector);
        
        if(useGlobal)
        relativeTargetVector = parent.transform.InverseTransformVector(targetGlobalVector);

        Debug.DrawRay(parent.transform.position, parent.transform.TransformVector(relativeTargetVector) * 10, Color.blue);

        //Debug.DrawRay(parent.transform.position, parent.transform.TransformDirection(new Vector3(0, 10, 0)), Color.green);



        float Tx, Ty, Tz;

        Tx = relativeTargetVector.x;

        Ty = relativeTargetVector.z;

        Tz = relativeTargetVector.y;

        //float angles = Mathf.Acos(c / Mathf.Sqrt(a * a + b * b + c * c));
        
        float targetAngle = Mathf.Acos(Tz / Mathf.Sqrt(Tx * Tx + Ty * Ty + Tz * Tz));

        realAppliedForce = 0;

        for (int i = 0; i < objects.Length; i++)
        {
            Vector3 motorPos = parent.transform.InverseTransformPoint(objects[i].transform.position);
            float Mx = motorPos.x;
            float My = motorPos.z;
            float Mz = motorPos.y;

            float nx = 0;
            float ny = 0;
            float nz = 1;

            float error;

            //negs[i] = (int)(-(a * x + b * y) / (c * Mathf.Abs((a * x + b * y) / c)));

            //MTx = cos^-1(((X1 * Mx) + (Y1 * My) + (Z1 * Mz))/(sqrt(X1^2 + Y1^2 + Z1^2) * sqrt(Mx + My + Mz)))

            //T = cos^-1(((X1 * X2) + (Y1 * Y2) + (Z1 * Z2))/(sqrt(X1^2 + Y1^2 + Z1^2) * sqrt(X2^2 + Y2^2 + Z2^2)))

            /*float MotorAngle = Mathf.Acos((a * x) + (b * y) + (c * z))/(Mathf.Sqrt(a * a + b * b + c * c) * Mathf.Sqrt(x * x + y * y + z * z));

            float TargetAngle = Mathf.Acos((a * nx) + (b * ny) + (c * nz)) / (Mathf.Sqrt(a * a + b * b + c * c) * Mathf.Sqrt(nx * nx + ny * ny + nz * nz));

            float radius = Mathf.Sqrt(x * x + y * y + z * z);

            float over = (MotorAngle - Mathf.PI / 2);

            float overAbs = Mathf.Abs(MotorAngle - Mathf.PI / 2);

            float piHalf = Mathf.PI / 2;*/

            //This is a disgustingly unsimplified version of the error function split into parts. 

            //The first part is finding the angle between the target vector and the normal vector. (Done before the loop)

            //The second part is finding the distance between the rotation axis and the current motor;

            float intersectionX = (Ty * (Ty * Mx - Tx * My))/((Tx * Tx) + (Ty * Ty));

            float distX = (intersectionX - Mx);


            float intersectionY = (Tx * (Tx * My - Mx * Ty))/ ((Tx * Tx) + (Ty * Ty));


            float distY = intersectionY - My;
            
            Vector3 radiusVector = new Vector3(distX, distY, 0);
            
            Debug.DrawLine(parent.transform.TransformPoint(motorPos), parent.transform.TransformPoint(new Vector3(intersectionX, 0, intersectionY)), Color.red);

            float radius = Mathf.Sqrt(distX * distX + distY * distY);

            //The last part is calculating whether the current motor should rotate up or rotate down. 

            float motorAngle = Mathf.Acos((Tx * Mx + Ty * My + Tz * Mz)/(Mathf.Sqrt(Tx * Tx + Ty * Ty + Tz * Tz) * Mathf.Sqrt(Mx * Mx + My * My + Mz * Mz))) - (Mathf.PI / 2);

            float sign = Mathf.Abs(motorAngle) / motorAngle;

            //float signed = (MotorAngle - Mathf.PI / 2) / Mathf.Abs(MotorAngle - Mathf.PI / 2);

            float degMotorAngle = Mathf.Rad2Deg * motorAngle;
            float degTargetAngle = Mathf.Rad2Deg * targetAngle;


            error = targetAngle * radius * motorAngle;

            if (float.IsNaN(error))
                error = 0;

            //if (error < 0.001)
            //    error = 0;
           
            /*if (a == 0 && b == 0)
                error = 0;
            else
                error = Mathf.Sqrt(((a * x + b * y) * (a * x + b * y)) / (a * a + b * b)) * angles * -(a * x + b * y) / (c * Mathf.Abs((a * x + b * y) / c));
                */


            //Debug.Log(error);
            errors[i] = error;

            proportional[i] = error;

            derivative[i] = (error - prevError[i]) / Time.fixedDeltaTime;

            integral[i] = integral[i] + error * Time.fixedDeltaTime;

            float output = PCoef * proportional[i] + ICoef * integral[i] + DCoef * derivative[i];

            //if ((targetAngle / (Mathf.PI / 2)) > 0.25)
            //    currentPower = 0;

            float appliedPower = currentPower * Tz/relativeTargetVector.magnitude;

            float appliedForce = output + appliedPower;//((currentPower - currentPower * (targetAngle / Mathf.PI)) / objects.Length); //currentPower/objects.Length; //;

            Debug.Log((targetAngle / Mathf.PI) * 100);

            ApplyForce(i, appliedForce);

            realAppliedForce += appliedForce;

            prevError[i] = error;

        }

    /*    for (int i = 0; i < objects.Length; i++) 
		  {
			ApplyForce(i,currentPower);
		}
      */

	}

   //converts a vector to the local space, ignoring yaw.
   Vector3 convertToLocal(Vector3 global)
    {

        float gX, gY, gZ;

        float lX, lY, lZ, lZ2, pitch, roll;

        float zxAngle, zyAngle; 

        gX = global.x;

        //Z and Y are switched, so we are swapping them here to make sense of them. 
        gY = global.z;

        gZ = global.y;



        pitch = rate.xRotation;

        roll = rate.zRotation;

        if (flipXRots)
            pitch *= -1;
        if (flipZRots)
            roll *= -1;

        if (gX != 0)
            zxAngle = Mathf.Rad2Deg * Mathf.Atan(Mathf.Abs(gX) / Mathf.Abs(gZ)) * (Mathf.Abs(gX) / gX) + 90 * ((Mathf.Abs(gZ) - gZ) / 2 * gZ) * -gX;
        else
            zxAngle = 0;
        if (gY != 0)
            zyAngle = Mathf.Rad2Deg * Mathf.Atan(Mathf.Abs(gY) / Mathf.Abs(gZ)) * (Mathf.Abs(gY) / gY) + 90 * ((Mathf.Abs(gZ) - gZ) / 2 * gZ) * -gY;
        else
            zyAngle = 0;

        lX = Mathf.Sin(Mathf.Deg2Rad * (zxAngle - roll));


        lY = Mathf.Sin(Mathf.Deg2Rad * (zyAngle - pitch));

        lZ = Mathf.Cos(Mathf.Deg2Rad * (zxAngle - roll));

        lZ2 = Mathf.Cos(Mathf.Deg2Rad * (zyAngle - pitch));


        float magXZ = Mathf.Sqrt(lX * lX + lZ * lZ);

        lX /= magXZ;

        lZ /= magXZ;

        float magYZ = Mathf.Sqrt(lY * lY + lZ2 * lZ2);

        float equilizerMag = lZ2 / lZ;
        
        lY /= equilizerMag;

        lZ2 /= equilizerMag;

        Debug.Log("Pitch: " + pitch + " Roll: " + roll);

        Debug.Log("Vector is: <" + lX + "," + lY + "," + lZ + "> or <" + lX + "," + lY + "," + lZ2 + ">");


        //Swap the Y and Z back
        return new Vector3(lX, lZ, lY);





    }

	void ApplyForce(int index, float power)
	{

        if (power < 0)
            return;
        if (power > maxForce)
            power = maxForce;

        float force = forcebase * power * Time.fixedDeltaTime;
        
		Vector3 localUp = objects [index].transform.up;

        localUp.Set(localUp.x * force, localUp.y * force, localUp.z * force);

		//Debug.Log (objects [index].transform.up);
        
		Debug.DrawRay(objects[index].transform.position, localUp);

		//Debug.Log (actualVector);

		parent.AddForceAtPosition (localUp, objects [index].transform.position);
	}


}



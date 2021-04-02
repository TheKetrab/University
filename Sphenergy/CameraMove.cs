using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMove : MonoBehaviour
{
    public Transform target;
    
    public float smoothSpeed = 0.125f;
    public float offset;

    void FixedUpdate()
    {
        Vector3 desiredPosition = target.transform.position + new Vector3(0f,0f,-10f);

        //Vector3 desiredPosition = new Vector3(target.position.x, 2f, offset);
        Vector3 smoothedPosition = Vector3.Lerp(transform.position, desiredPosition, smoothSpeed);
        transform.position = smoothedPosition;
       
        transform.LookAt(target);
    }
}

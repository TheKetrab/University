using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Assertions;

public class Follow : MonoBehaviour
{
    public Transform target;    
    public float smoothSpeed = 0.125f;

    private void Awake()
    {
        Assert.IsNotNull(target);        
    }


    private float maxDist = 3f;
    void FixedUpdate()
    {
        
        Vector3 desiredPosition = target.transform.position;

        if (Vector3.Distance(desiredPosition, transform.position) > maxDist)
        {
            transform.position = desiredPosition;            
        }
        else
        {
            Vector3 smoothedPosition = Vector3.Lerp(transform.position, desiredPosition, smoothSpeed);
            transform.position = smoothedPosition;
        }
        
    }
}

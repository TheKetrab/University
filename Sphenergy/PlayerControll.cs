using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class PlayerControll : MonoBehaviour
{

    public float acceleration;
    public float maxSpeedX;
    public float maxSpeedY;
    public float maxSpeedTorque;

    private Rigidbody rb;

    private float torque = 0.5f;
    
    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        float inputX = Input.GetAxis("Horizontal");

        if (!IsGrounded())
        {
            rb.AddForce(new Vector3(inputX, 0f, 0f) * acceleration * 40f * Time.deltaTime);
            rb.AddTorque(Vector3.back * torque * inputX * 40f * Time.deltaTime);

        }
    }
    
    

    private float distToGround = 0.5f;
    public bool IsGrounded() {
        return Physics.Raycast(transform.position, -Vector3.up, distToGround + 0.1f);
    }
    
    void FixedUpdate()
    {
        
        // max speed
        if (!IsGrounded())
        {

            // velocity X
            // ----- ----- ----- ----- -----
            if (rb.velocity.x > maxSpeedX)
            {
                var x = maxSpeedX;
                rb.velocity = new Vector3(x, rb.velocity.y, rb.velocity.z);
            }

            else if (rb.velocity.x < -maxSpeedX)
            {
                var x = -maxSpeedX;
                rb.velocity = new Vector3(x, rb.velocity.y, rb.velocity.z);
            }


            // velocity Y
            // ----- ----- ----- ----- -----
            if (rb.velocity.y > maxSpeedY)
            {
                var y = maxSpeedY;
                rb.velocity = new Vector3(rb.velocity.x, y, rb.velocity.z);
            }

            else if (rb.velocity.y < -maxSpeedY)
            {
                var y = -maxSpeedY;
                rb.velocity = new Vector3(rb.velocity.x, y, rb.velocity.z);
            }
            

            // torque
            // ----- ----- ----- ----- -----            
            if (rb.angularVelocity.z > maxSpeedTorque)
            {
                var z = maxSpeedTorque;
                rb.angularVelocity = new Vector3(rb.angularVelocity.x, rb.angularVelocity.y, z);
            }

            else if (rb.angularVelocity.z < -maxSpeedTorque)
            {
                var z = -maxSpeedTorque;
                rb.angularVelocity = new Vector3(rb.angularVelocity.x, rb.angularVelocity.y, z);
            }

        }

        // slow if is grounded
        else
        {
            var slowFactor = 0.075f;
            
            // velocity
            if (rb.velocity.x > 0)
            {
                var x = rb.velocity.x - slowFactor;
                if (x < 0) x = 0f;
                rb.velocity = new Vector3(x, rb.velocity.y, rb.velocity.z);
            }

            else if (rb.velocity.x < 0)
            {
                var x = rb.velocity.x + slowFactor;
                if (x > 0) x = 0f;
                rb.velocity = new Vector3(x, rb.velocity.y, rb.velocity.z);
            }
            
            //torque
            if (rb.angularVelocity.z > 0)
            {
                var z = rb.angularVelocity.z - slowFactor;
                if (z < 0) z = 0f;
                rb.angularVelocity = new Vector3(rb.angularVelocity.x, rb.angularVelocity.y, z);
            }

            else if (rb.angularVelocity.z < 0)
            {
                var z = rb.angularVelocity.z + slowFactor;
                if (z > 0) z = 0f;
                rb.angularVelocity = new Vector3(rb.angularVelocity.x, rb.angularVelocity.y, z);
            }

            
        }
    }
    
}

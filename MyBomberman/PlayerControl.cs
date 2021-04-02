using System;
using System.Collections;
using System.Collections.Generic;
using System.Security.Cryptography.X509Certificates;
using UnityEngine;

public class PlayerControl : MonoBehaviour
{

	public GameObject player;
	public Camera cam;
	
	public Vector3 MoveVector;

	private Transform camTransform;
	private BallProperties bp;
	private BallMove bm;
	
	
	// Use this for initialization
	void Start ()
	{

		camTransform = cam.transform;
		bp = player.GetComponent<BallProperties>();
		bm = player.GetComponent<BallMove>();
	}
	
	void Update ()
	{
		if (Input.GetKeyDown(KeyCode.Space))
		{
			bp.InsertBomb();
		}
		
		// input from keyboard
		MoveVector = GetInputVector();
		
		// normalize by camera
		MoveVector = RotateWithView();

		// move player
		Move();

	}

	
	

	Vector3 GetInputVector()
	{
		Vector3 dir = Vector3.zero;
		
		dir.x = Input.GetAxis("Horizontal");
		dir.z = Input.GetAxis("Vertical");

		if(dir.magnitude>1)
			dir.Normalize();

		return dir;
	}


	Vector3 RotateWithView()
	{
		Vector3 dir = camTransform.TransformDirection(MoveVector);
		dir.Set(dir.x, 0, dir.z);
		return dir.normalized * MoveVector.magnitude;
	}
	
	void Move()
	{

		if (bm.IsBallStopped())
		{
			var x = MoveVector.x;
			var z = MoveVector.z;

			if (Math.Abs(x) > Math.Abs(z))
			{
				if (x > 0)
				{
					if (bm.FreePosOnRight())
						bm.goal += GameSystem.RoundToIntVector(Vector3.right);
				}
				else if (x < 0)
				{
					if (bm.FreePosOnLeft())
						bm.goal += GameSystem.RoundToIntVector(Vector3.left);
				}
			}
			else
			{
				if (z > 0)
				{
					if (bm.FreePosOnUp())
						bm.goal += GameSystem.RoundToIntVector(Vector3.forward);
				}
				else if (z < 0)
				{
					if (bm.FreePosOnDown())
						bm.goal += GameSystem.RoundToIntVector(Vector3.back);
				}
				
			}
			
		}
		

	}
}

using System.Collections;
using System.Collections.Generic;
using System.Security.Cryptography.X509Certificates;
using UnityEngine;
using UnityEngine.Serialization;

public class CameraController : MonoBehaviour
{
	public GameObject player;
	public Transform cam;
	public float RotationSpeed;
	
	// Use this for initialization
	void Start ()
	{

	}


	void Update()
	{
		transform.position = player.transform.position;
		Vector3 offset = cam.position - transform.position;
		
		if (Input.GetKey(KeyCode.Q))
			transform.Rotate(0,RotationSpeed * Time.deltaTime, 0);
		
		if (Input.GetKey(KeyCode.E))
			transform.Rotate(0,-RotationSpeed * Time.deltaTime, 0);

		if (Input.GetKey(KeyCode.R))
			cam.position += offset * Time.deltaTime;

		if (Input.GetKey(KeyCode.T))
			cam.position += -offset * Time.deltaTime;

		
		cam.LookAt(transform);

	}

}

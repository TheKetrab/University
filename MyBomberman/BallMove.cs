using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BallMove : MonoBehaviour {

	
	
	private float _moveIgnore = 0.1f;
	public float MoveUp;
	public float MoveDown;
	public float MoveLeft;
	public float MoveRight;

	public Vector3Int goal;

	private BallProperties bp;
	private AI ai;
	
	public bool IsAI = true;
	
	// Use this for initialization
	void Start ()
	{
		bp = gameObject.GetComponent<BallProperties>();
		goal = GameSystem.GetPosition(gameObject);
		
		if (IsAI)
		{
			ai = gameObject.GetComponent<AI>();	
		}
	}
	
	// Update is called once per frame
	void Update () {
		Translate();
		GoToPos();
	}
	
	void Translate()
	{
		if (MoveRight > 0f)
		{
			transform.position += Vector3.right * bp.MoveSpeed * Time.deltaTime;
			MoveRight -= bp.MoveSpeed * Time.deltaTime;
			if (MoveRight < 0)
			{
				transform.position = GameSystem.RoundToIntVector(transform.position);
				transform.rotation = Quaternion.identity;
				MoveRight = 0;
			}
		}
		
		if (MoveLeft > 0f)
		{
			transform.position += Vector3.left * bp.MoveSpeed * Time.deltaTime;
			MoveLeft -= bp.MoveSpeed * Time.deltaTime;
			if (MoveLeft < 0)
			{
				transform.position = GameSystem.RoundToIntVector(transform.position);
				transform.rotation = Quaternion.identity;
				MoveLeft = 0;
			}
		}

		if (MoveUp > 0f)
		{
			transform.position += Vector3.forward * bp.MoveSpeed * Time.deltaTime;
			MoveUp -= bp.MoveSpeed * Time.deltaTime;
			if (MoveUp < 0)
			{
				transform.position = GameSystem.RoundToIntVector(transform.position);
				transform.rotation = Quaternion.identity;
				MoveUp = 0;
			}
		}

		if (MoveDown > 0f)
		{
			transform.position += Vector3.back * bp.MoveSpeed * Time.deltaTime;
			MoveDown -= bp.MoveSpeed * Time.deltaTime;
			if (MoveDown < 0)
			{
				transform.position = GameSystem.RoundToIntVector(transform.position);
				transform.rotation = Quaternion.identity;
				MoveDown = 0;
			}
		}
	}
	
	public void AlignGoalToPos()
	{
		var actual = GameSystem.GetPosition(gameObject);
		goal = actual;
	}
	
	
	public void RandomMove()
	{
		var ballPos = GameSystem.GetPosition(gameObject);

		var tab = new int[4];
		var counter = 0;

		if (FreePosOnUp())
		{
			tab[counter] = 1;
			counter++;
		}

		if (FreePosOnDown())
		{
			tab[counter] = 2;
			counter++;
		}
		if (FreePosOnLeft())
		{
			tab[counter] = 3;
			counter++;
		}
		if (FreePosOnRight())
		{
			tab[counter] = 4;
			counter++;
		}

		var rand = UnityEngine.Random.Range(0, counter);
		var res = tab[rand];

		if (res == 1)
		{
			goal = new Vector3Int(ballPos.x, 0, ballPos.z+2);
		}
		else if (res == 2)
		{
			goal = new Vector3Int(ballPos.x, 0, ballPos.z - 2);
		}
		else if (res == 3)
		{
			goal = new Vector3Int(ballPos.x-2, 0, ballPos.z);
		}
		if (res == 4)
		{
			goal = new Vector3Int(ballPos.x+2, 0, ballPos.z);
		}

	}

	
	public bool FreePosOnUp()
	{
		var originUp = GameSystem.GetPosition(gameObject);
		RaycastHit upHit;
		var upRay    = new Ray(originUp, Vector3.forward);
		Physics.Raycast(upRay, out upHit);

		
		/* FIND BOMBS */
		if (upHit.collider.CompareTag("BLACK")
		    || upHit.collider.CompareTag("GREEN")
		    || upHit.collider.CompareTag("BALL")
		    || upHit.collider.CompareTag("BOMB"))
		{
			return (GameSystem.GetDistBetween(gameObject, upHit.transform.gameObject) >= 2);
		}
			
		return true;
	}

	public bool FreePosOnDown()
	{
		
		var originDown = GameSystem.GetPosition(gameObject);
		RaycastHit downHit;
		var downRay    = new Ray(originDown, Vector3.back);
		Physics.Raycast(downRay, out downHit);

		/* FIND BOMBS */
		if (downHit.collider.CompareTag("BLACK")
		    || downHit.collider.CompareTag("GREEN")
		    || downHit.collider.CompareTag("BALL")
		    || downHit.collider.CompareTag("BOMB"))
		{
			return (GameSystem.GetDistBetween(gameObject, downHit.transform.gameObject) >= 2);
		}
			
		return true;
	}

	public bool FreePosOnLeft()
	{
		var originLeft = GameSystem.GetPosition(gameObject);
		RaycastHit leftHit;
		var leftRay    = new Ray(originLeft, Vector3.left);
		Physics.Raycast(leftRay, out leftHit);

		/* FIND BOMBS */
		if (leftHit.collider.CompareTag("BLACK")
		    || leftHit.collider.CompareTag("GREEN")
		    || leftHit.collider.CompareTag("BALL")
		    || leftHit.collider.CompareTag("BOMB"))
		{
			return (GameSystem.GetDistBetween(gameObject, leftHit.transform.gameObject) >= 2);
		}
			
		return true;
	}	
	
	public bool FreePosOnRight()
	{
		var originRight = GameSystem.GetPosition(gameObject);
		RaycastHit rightHit;
		var rightRay    = new Ray(originRight, Vector3.right);
		Physics.Raycast(rightRay, out rightHit);

		/* FIND BOMBS */
		if (rightHit.collider.CompareTag("BLACK")
		    || rightHit.collider.CompareTag("GREEN")
		    || rightHit.collider.CompareTag("BALL")
		    || rightHit.collider.CompareTag("BOMB"))
		{
			return (GameSystem.GetDistBetween(gameObject, rightHit.transform.gameObject) >= 2);
		}
			
		return true;
	}
	
	
	
	public bool IsBallStopped()
	{
		return MoveUp <= _moveIgnore
		       && MoveDown <= _moveIgnore
		       && MoveLeft <= _moveIgnore
		       && MoveRight <= _moveIgnore;

	}
	
	public void Move(string direction)
	{
		// ruszaj sie, gdy jestes playerem
		// lub gdy to bezpieczne - jesli niebezpieczne, to zaczekaj z ruchem
		if (!IsAI || ai.SafePosOn(direction))
		{

			if (direction.Equals("left"))
				MoveLeft++;

			else if (direction.Equals("right"))
				MoveRight++;

			else if (direction.Equals("up"))
				MoveUp++;

			else if (direction.Equals("down"))
				MoveDown++;
		}

	}

	public void Stop()
	{
		MoveUp = 0;
		MoveDown = 0;
		MoveLeft = 0;
		MoveRight = 0;
	}
	
	
	private void GoToPos()
	{

		var actual = GameSystem.GetPosition(gameObject);
		


		if (GameSystem.AlmostEqualPositions(actual, goal))
		{
			return;
		}


		if (IsBallStopped())
		{

			var deltaX = goal.x - actual.x;
			var deltaZ = goal.z - actual.z;

			if (Math.Abs(deltaX) > Math.Abs(deltaZ))
			{
				if (deltaX > 0)
					Move("right");
				else
					Move("left");
			}
			else
			{
				if (deltaZ > 0)
					Move("up");
				else
					Move("down");

			}
		}


	}
}

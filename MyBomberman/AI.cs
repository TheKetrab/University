using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using Random = System.Random;

public class AI : MonoBehaviour
{

	private Rigidbody rb;
	private BallProperties bp;
	private BallMove bm;
	private GameObject OptionNotFound;
	
	
	void Start () {
		rb = gameObject.GetComponent<Rigidbody>();
		bp = gameObject.GetComponent<BallProperties>();
		bm = gameObject.GetComponent<BallMove>();
		
		OptionNotFound = new GameObject("OptionNotFound");		
	}
	
	void Update ()
	{
		if (bm.IsBallStopped())
			FindTask();
	}



	
	void OnCollisionStay(Collision collision)
	{
		
		if (collision.gameObject.CompareTag("BLACK"))
		{
			bm.AlignGoalToPos();
		}
		
		if (collision.gameObject.CompareTag("GREEN"))
		{
			bm.AlignGoalToPos();
		}

		if (collision.gameObject.CompareTag("BOMB"))
		{
			bm.AlignGoalToPos();
		}
		
		if (collision.gameObject.CompareTag("GIFT_BOMBS")
		    || collision.gameObject.CompareTag("GIFT_EXPLOSION")
		    || collision.gameObject.CompareTag("GIFT_SPEED")
		    || collision.gameObject.CompareTag("GIFT_LIVES"))
		{

		}
	}

	GameObject FindBestOption()
	{
		var pos = GameSystem.GetPosition(gameObject);
		var originUp = new Vector3(pos.x, 0.5f, pos.z);
		var originDown = new Vector3(pos.x, 0.5f, pos.z);
		var originLeft = new Vector3(pos.x, 0.5f, pos.z);
		var originRight = new Vector3(pos.x, 0.5f, pos.z);

		RaycastHit upHit;
		RaycastHit downHit;
		RaycastHit leftHit;
		RaycastHit rightHit;

		var upRay    = new Ray(originUp, Vector3.forward);
		Physics.Raycast(upRay, out upHit);
		
		var downRay  = new Ray(originDown, Vector3.back);
		Physics.Raycast(downRay, out downHit);
		
		var leftRay  = new Ray(originLeft, Vector3.left);
		Physics.Raycast(leftRay, out leftHit);
		
		var rightRay = new Ray(originRight, Vector3.right);
		Physics.Raycast(rightRay, out rightHit);

		/* FIND BOMBS */
		if (upHit.collider.CompareTag("BOMB")
		 && upHit.distance < upHit.transform.GetComponent<BombExplosion>().GetBombPower())
			return upHit.collider.gameObject;
		
		if (downHit.collider.CompareTag("BOMB")
		    && downHit.distance < downHit.transform.GetComponent<BombExplosion>().GetBombPower())
			return downHit.collider.gameObject;

		if (leftHit.collider.CompareTag("BOMB")
		    && leftHit.distance < leftHit.transform.GetComponent<BombExplosion>().GetBombPower())
			return leftHit.collider.gameObject;

		if (rightHit.collider.CompareTag("BOMB")
		    && rightHit.distance < rightHit.transform.GetComponent<BombExplosion>().GetBombPower())
			return rightHit.collider.gameObject;

		
		/* FIND SMOKE */
		if (GameSystem.CompareFirstTagLetters(upHit.collider.tag,"SMOKE"))
			return upHit.collider.gameObject;
		
		if (GameSystem.CompareFirstTagLetters(downHit.collider.tag,"SMOKE"))
			return downHit.collider.gameObject;

		if (GameSystem.CompareFirstTagLetters(leftHit.collider.tag,"SMOKE"))
			return leftHit.collider.gameObject;

		if (GameSystem.CompareFirstTagLetters(rightHit.collider.tag,"SMOKE"))
			return rightHit.collider.gameObject;


		
		/* FIND GIFTS */
		if (GameSystem.CompareFirstTagLetters(upHit.collider.tag,"GIFT"))
			return upHit.collider.gameObject;
		
		if (GameSystem.CompareFirstTagLetters(downHit.collider.tag,"GIFT"))
		    return downHit.collider.gameObject;

		if (GameSystem.CompareFirstTagLetters(leftHit.collider.tag,"GIFT"))
		    return leftHit.collider.gameObject;

		if (GameSystem.CompareFirstTagLetters(rightHit.collider.tag,"GIFT"))
			return rightHit.collider.gameObject;
		
		
		
		/* FIND OPPONENTS */
		if (upHit.collider.CompareTag("BALL"))
			return upHit.collider.gameObject;
		
		if (downHit.collider.CompareTag("BALL"))
			return downHit.collider.gameObject;

		if (leftHit.collider.CompareTag("BALL"))
			return leftHit.collider.gameObject;

		if (rightHit.collider.CompareTag("BALL"))
			return rightHit.collider.gameObject;
		
		/* FIND GREENS */
		if (upHit.collider.CompareTag("GREEN"))
			return upHit.collider.gameObject;
		
		if (downHit.collider.CompareTag("GREEN"))
			return downHit.collider.gameObject;

		if (leftHit.collider.CompareTag("GREEN"))
			return leftHit.collider.gameObject;

		if (rightHit.collider.CompareTag("GREEN"))
			return rightHit.collider.gameObject;
		
		/* NOTHING INTERESTING */
		return OptionNotFound;

	}











	private string GetRelationWithObject(GameObject o)
	{
		var posBall = GameSystem.GetPosition(gameObject);
		var posObject = GameSystem.GetPosition(o);

		if (posObject.z > posBall.z)
			return "OnUp";
		if (posObject.z < posBall.z)
			return "OnDown";
		if (posObject.x > posBall.x)
			return "OnRight";
		if (posObject.x < posBall.x)
			return "OnLeft";

		return "None";

	}

	private void FindTask()
	{
		
		var option = FindBestOption();
		//print(option);
		// ----- ----- -----
		if (option == OptionNotFound)
		{
			bm.RandomMove();
			return;
		}
		// ----- ----- -----

		var optionPos = GameSystem.GetPosition(option);

		if (option.CompareTag("SMOKE"))
		{
			print("SMOKE");
		}

		if (option.CompareTag("BOMB"))
		{
			var ballPos = GameSystem.GetPosition(gameObject);
			var relation = GetRelationWithObject(option);
			
			if (relation.Equals("OnUp"))
			{
				
				if (bm.FreePosOnRight())
					bm.goal = new Vector3Int(ballPos.x+2, 0, ballPos.z);
				else if (bm.FreePosOnLeft())
					bm.goal = new Vector3Int(ballPos.x-2, 0, ballPos.z);
				else
					bm.goal = new Vector3Int(ballPos.x, 0, ballPos.z-2);

			}
			
			else if (relation.Equals("OnDown"))
			{
				if (bm.FreePosOnRight())
					bm.goal = new Vector3Int(ballPos.x+2, 0, ballPos.z);
				else if (bm.FreePosOnLeft())
					bm.goal = new Vector3Int(ballPos.x-2, 0, ballPos.z);
				else
					bm.goal = new Vector3Int(ballPos.x, 0, ballPos.z+2);
			}

			else if (relation.Equals("OnLeft"))
			{
				if (bm.FreePosOnUp())
					bm.goal = new Vector3Int(ballPos.x, 0, ballPos.z+2);
				else if (bm.FreePosOnDown())
					bm.goal = new Vector3Int(ballPos.x, 0, ballPos.z-2);
				else
					bm.goal = new Vector3Int(ballPos.x+2, 0, ballPos.z);
			}
			
			else if (relation.Equals("OnRight"))
			{
				if (bm.FreePosOnUp())
					bm.goal = new Vector3Int(ballPos.x, 0, ballPos.z+2);
				else if (bm.FreePosOnDown())
					bm.goal = new Vector3Int(ballPos.x, 0, ballPos.z-2);
				else
					bm.goal = new Vector3Int(ballPos.x-2, 0, ballPos.z);
			}

			return;
		}

		if (GameSystem.CompareFirstTagLetters(option.tag, "GIFT"))
		{
			var relation = GetRelationWithObject(option);
			
			if (relation.Equals("OnUp"))
				bm.goal = new Vector3Int(optionPos.x, 0, optionPos.z+2);

			if (relation.Equals("OnDown"))
				bm.goal = new Vector3Int(optionPos.x, 0, optionPos.z-2);

			if (relation.Equals("OnLeft"))
				bm.goal = new Vector3Int(optionPos.x-2, 0, optionPos.z);

			if (relation.Equals("OnRight"))
				bm.goal = new Vector3Int(optionPos.x+2, 0, optionPos.z);

		}

		if (option.CompareTag("BALL"))
		{
			var dist = GameSystem.GetDistBetween(gameObject, option);
			
			if (dist <= 1) { bp.InsertBomb(); bm.RandomMove(); }
			else if (dist-2 <= bp.BombPower) bp.InsertBomb();
			else bm.goal = optionPos;

			return;		
		}

		if (option.CompareTag("GREEN"))
		{
			var dist = GameSystem.GetDistBetween(gameObject, option);
			
			if (dist <= 1) { bp.InsertBomb(); bm.RandomMove(); }
			else bm.goal = optionPos;

			return;
			
		}
		
	}

	bool IsBombDangerousForPos(GameObject bomb, Vector3Int pos)
	{

		// jesli wiecej niz sekunda to bomba jest bezpieczna
		if (bomb.GetComponent<BombExplosion>().TimeToExplosion > 1000f) return false;
		
		var bombPos = GameSystem.GetPosition(bomb);
		var n = bomb.GetComponent<BombExplosion>().GetBombPower();
		var d = GameSystem.GetDistBetween(bombPos, pos);
		
		
		return d<=n;
	}
	
	public bool SafePosOn(string direction)
	{
		var radius = 3;
		var bombs = GetDangerousBombs(radius, GameSystem.GetPosition(gameObject));
		var posToCheck = GameSystem.GetPosition(gameObject);

		if (direction.Equals("up"))
			posToCheck += new Vector3Int(0, 0, 1);
		else if (direction.Equals("down"))
			posToCheck += new Vector3Int(0, 0, -1);
		else if (direction.Equals("left"))
			posToCheck += new Vector3Int(-1, 0, 0);
		else if (direction.Equals("right"))
			posToCheck += new Vector3Int(1, 0, 0);

		while (bombs.Count != 0)
		{
			var bomb = bombs.Dequeue();
			if (IsBombDangerousForPos(bomb, posToCheck))
			{
				return false;
			}
		}
		

		return true;

	}
	
	Queue<GameObject> GetDangerousBombs(int radius, Vector3Int pos)
	{
		Collider[] hitColliders = Physics.OverlapSphere(pos, radius);

		Queue<GameObject> res = new Queue<GameObject>();

		for (int i = 0; i < hitColliders.Length; i++)
			if ((hitColliders[i].CompareTag("BOMB")
			 && !OnBombLine(hitColliders[i].gameObject)) // nie chcemy dodawac do kolejki bomb ktore maja ten sam x lub z
			|| (hitColliders[i].CompareTag("SMOKE")
			    && !hitColliders[i].GetComponent<ExplosionSmoke>().IsSmokeStopped())) // jesli znalazles smoke, ktory sie nie porusza, to jest bezpieczny
					res.Enqueue(hitColliders[i].gameObject);

		return res;
	}

	bool OnBombLine(GameObject bomb)
	{
		var bombPos = GameSystem.GetPosition(bomb);
		var ballPos = GameSystem.GetPosition(gameObject);

		return ballPos.x == bombPos.x
		       || ballPos.z == bombPos.z;

	}

	
	
}

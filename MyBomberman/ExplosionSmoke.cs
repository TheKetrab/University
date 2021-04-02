using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ExplosionSmoke : MonoBehaviour
{

	private float _moveLeft = 0;
	private float _moveRight = 0;
	private float _moveUp = 0;
	private float _moveDown = 0;
	private bool _kill;
	private float _timer = 3f; // 3s
	
	private const float Speed = 0.3f;
	private List<GameObject> Children;


	// Use this for initialization
	void Start ()
	{
		Children = new List<GameObject>();
	}



	public bool IsSmokeStopped()
	{
		return _moveLeft == 0
		       && _moveRight == 0
		       && _moveUp == 0
		       && _moveDown == 0;
	}
	
	// Update is called once per frame
	void Update () {

		if (_kill)
		{
			_timer -= Time.deltaTime;
			if (_timer < 0) Destroy(gameObject);
		}
		
		if (_moveLeft > 0)
		{
			transform.Translate(Vector3.left * Speed);
			_moveLeft -= Speed;
			if (_moveLeft < 0) _moveLeft = 0;
		}
		
		else if (_moveRight > 0)
		{
			transform.Translate(Vector3.right * Speed);
			_moveRight -= Speed;
			if (_moveRight < 0) _moveRight = 0;
		}

		else if (_moveUp > 0)
		{
			transform.Translate(Vector3.forward * Speed);
			_moveUp -= Speed;
			if (_moveUp < 0) _moveUp = 0;
		}

		else if (_moveDown > 0)
		{
			transform.Translate(Vector3.back * Speed);
			_moveDown -= Speed;
			if (_moveDown < 0) _moveDown = 0;
		}

	}
	
	

	private void InsertGift(Vector3Int pos)
	{
		// insert or not? rand -> 50 percent
		if (Random.Range(0, 2) != 0) return;
		
		// rand gift to insert
		var random = Random.Range(0, 4);

		var path = "Prefabs/Gift";
		if (random == 0) path += "Speed";
		else if (random == 1) path += "ExplosionSize";
		else if (random == 2) path += "Lives";
		else if (random == 3) path += "BombsAmount";

		var gift = Resources.Load<GameObject>(path);
		var trans = pos;
		
		Children.Add(Instantiate(gift, trans, Quaternion.identity));
		//Instantiate(gift, trans, Quaternion.identity);
		
	}
	
	
	
	
	
	
	
	
	

	public void Move(string direction, float value)
	{
		if 	    (direction.Equals("left"))  _moveLeft  += value;
		else if (direction.Equals("right")) _moveRight += value;
		else if (direction.Equals("up"))    _moveUp    += value;
		else if (direction.Equals("down"))  _moveDown  += value;

	}

	public void Kill()
	{
		_kill = true;
	}


	private void OnTriggerEnter(Collider other)
	{
		// exit if
		if (IsSmokeStopped()) return;

		if (other.gameObject.CompareTag("BALL"))
		{
			other.gameObject.GetComponent<BallProperties>().DecreaseLives();
		}
			
		
		if (other.gameObject.CompareTag("BLACK"))
		{
			StopSmoke();
		}
		
		if (other.gameObject.CompareTag("GREEN"))
		{
			var pos = GameSystem.GetPosition(other.gameObject);
			Destroy(other.gameObject);
			StopSmoke();
			InsertGift(pos);
		}

		if (other.gameObject.CompareTag("BOMB"))
		{
			/* zdecydowalem, ze bomba zabija bombe, a nie sprawia, ze eksploduje
			 * to dlatego, ze AI dziala dzieki temu duzo lepiej i jest wiecej zabawy */
			Destroy(other.gameObject);
			//other.gameObject.GetComponent<BombExplosion>().TimeToExplosion = 0;
		}
		
		if (other.gameObject.CompareTag("GIFT_BOMBS")
		 || other.gameObject.CompareTag("GIFT_EXPLOSION")
		 || other.gameObject.CompareTag("GIFT_SPEED")
		 || other.gameObject.CompareTag("GIFT_LIVES"))
		{
			/* owner smoke cannot kill its child */
			if (!Children.Contains(other.gameObject))
				Destroy(other.gameObject);
		}

		
		

	}

	void StopSmoke()
	{
		_moveUp = 0;
		_moveDown = 0;
		_moveLeft = 0;
		_moveRight = 0;
		
	}
	
}

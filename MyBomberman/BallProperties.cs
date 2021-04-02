using System;
using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using  UnityEngine.Assertions;

public class BallProperties : MonoBehaviour {


	public float MoveSpeed = 1.5f;
	public int Lives = 1;
	public int BombPower = 1;
	public int BombMax = 1;
	public int ActiveBombs;
	public TextMeshPro LivesText;
	public string Name;
	public bool isPlayer;

	private GameSystem gs;

	
	// Use this for initialization
	void Start ()
	{
		gs = GameObject.Find("GameSystem").GetComponent<GameSystem>();
		Name = gameObject.name;
		SetLivesText();
		Assert.IsNotNull(LivesText);
				
	}
	
	// Update is called once per frame
	void Update ()
	{

	}

	private void SetLivesText()
	{
		if (Lives <= 0)
			LivesText.text = Name + " : " + "Dead";
		else
			LivesText.text = Name + " : " + Lives;
	}

	public void IncreaseLives()
	{
		Lives++;
		SetLivesText();
	}

	public void DecreaseLives()
	{
		Lives--;
		SetLivesText();
		if (Lives == 0)			
			Destroy(gameObject);
	}
	
	public void InsertBomb()
	{
		if (ActiveBombs < BombMax)
		{
			ActiveBombs++;
			
			var pos = GameSystem.GetPosition(gameObject);

			const string path = "Prefabs/Bomb";
			var bomb = Resources.Load<GameObject>(path);
			var trans = pos;

			var bombPtr = Instantiate(bomb, trans, Quaternion.identity);
			bombPtr.GetComponent<BombExplosion>().Owner = gameObject;


		}
	}

	private void OnDestroy()
	{
		if (isPlayer)
		{
			gs.GameLose();
		}
		else
		{
			gs.OpponentsCount--;
			if (gs.OpponentsCount == 0)
				gs.GameWin();
		}
	}
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GiveBonus : MonoBehaviour {

	public AudioManagerScript am;
	public GameObject player;
	
	// Use this for initialization
	void Start () {
		am = GameObject.Find("AudioManager").GetComponent<AudioManagerScript>();
		player = GameObject.Find("Player");
	}
	
	// Update is called once per frame
	void Update () {
		
	}


	private void OnCollisionStay(Collision other)
	{
		if (other.gameObject.CompareTag("BALL"))
		{
			if (other.gameObject == player)
				am.PlayTakeGift();

			if (gameObject.CompareTag("GIFT_BOMBS"))
			{
				other.gameObject.GetComponent<BallProperties>().BombMax++;
			}

			if (gameObject.CompareTag("GIFT_EXPLOSION"))
			{
				other.gameObject.GetComponent<BallProperties>().BombPower++;
			}

			if (gameObject.CompareTag("GIFT_LIVES"))
			{
				other.gameObject.GetComponent<BallProperties>().IncreaseLives();
			}

			if (gameObject.CompareTag("GIFT_SPEED"))
			{
				other.gameObject.GetComponent<BallProperties>().MoveSpeed++;
			}

			Destroy(gameObject);

		}
	}
}

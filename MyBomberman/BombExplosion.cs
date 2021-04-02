using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BombExplosion : MonoBehaviour
{
	public GameObject player;
	public float TimeToExplosion;
	public GameObject Owner;
	
	private int _bombPower;
	private BallProperties bp;


	public AudioManagerScript am;
	
	// Use this for initialization
	void Start ()
	{
		am = GameObject.Find("AudioManager").GetComponent<AudioManagerScript>();

		player = GameObject.Find("Player");
		bp = Owner.GetComponent<BallProperties>();
		_bombPower = bp.BombPower;
	}
	
	// Update is called once per frame
	void Update ()
	{

		TimeToExplosion -= Time.deltaTime * 1000;

		if (TimeToExplosion <= 0)
			Explosion();
	}

	private GameObject InsertSmoke()
	{
		var path = "Prefabs/Smoke";
		
		var prefab = Resources.Load<GameObject>(path);
		var pos = GameSystem.GetPosition(gameObject);
		var trans = new Vector3(pos.x,0.5f,pos.z);

		return Instantiate(prefab, trans, Quaternion.identity);
	}

	public int GetBombPower()
	{
		return _bombPower;
	}

	private void OnDestroy()
	{
		bp.ActiveBombs--;
	}

	void Explosion()
	{
		/* active bombs */
		var n = _bombPower;
		
		
		/* smoke particles */
		var leftSmoke  = InsertSmoke();
		var rightSmoke = InsertSmoke();
		var upSmoke    = InsertSmoke();
		var downSmoke  = InsertSmoke();

		if (Vector3.Distance(player.transform.position, gameObject.transform.position) < 4.0f)
			am.PlayBombExplosion();
		
		

		/* effect */
		leftSmoke.GetComponent<ExplosionSmoke>().Move("left",n);
		rightSmoke.GetComponent<ExplosionSmoke>().Move("right",n);
		upSmoke.GetComponent<ExplosionSmoke>().Move("up",n);
		downSmoke.GetComponent<ExplosionSmoke>().Move("down",n);

		/* destructors */
		leftSmoke.GetComponent<ExplosionSmoke>().Kill();
		rightSmoke.GetComponent<ExplosionSmoke>().Kill();
		upSmoke.GetComponent<ExplosionSmoke>().Kill();
		downSmoke.GetComponent<ExplosionSmoke>().Kill();
		Destroy(gameObject);
	}
	


	private void OnTriggerExit(Collider other)
	{
		// przenikalny dopiero, gdy owner z niego wyjdzie
		if (other.gameObject == Owner)
			gameObject.GetComponent<BoxCollider>().isTrigger = false;

	}
}

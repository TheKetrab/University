using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Timeline;

public class AudioManagerScript : MonoBehaviour
{

	public AudioSource bombExplosion;
	public AudioSource takeGift;

	
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	public void PlayBombExplosion()
	{
		bombExplosion.Play();
	}
	
	public void PlayTakeGift()
	{
		takeGift.Play();
	}
	
	
}

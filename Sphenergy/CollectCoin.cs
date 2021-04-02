using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CollectCoin : MonoBehaviour
{
    private ModeDetector modeDetector;
    private PlayerStats playerStats;
    private AudioSource audioSource;
    
    
    // Start is called before the first frame update
    void Start()
    {
        playerStats = GameObject.Find("Player").GetComponent<PlayerStats>();
        modeDetector = GameObject.Find("ModeDetector").GetComponent<ModeDetector>();
        audioSource = GetComponent<AudioSource>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    
    private void OnTriggerEnter(Collider other)
    {
        // EXIT IF
        if (!other.gameObject.CompareTag("Player"))
            return;
        
        if (modeDetector.mode.Equals("PLAYMODE")
            || modeDetector.mode.Equals("GAME"))
        {
            InsertParticles();
            gameObject.SetActive(false);
            playerStats.TakeCoin();
        }
    }

    private GameObject InsertParticles()
    {
        var path = "Prefabs/CoinParticle";
		
        var prefab = Resources.Load<GameObject>(path);
        var pos = gameObject.transform.position;

        return Instantiate(prefab, pos, Quaternion.identity);
    }


}

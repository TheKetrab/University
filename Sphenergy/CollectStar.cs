using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CollectStar : MonoBehaviour
{
    private PlayerStats playerStats;
    private ModeDetector modeDetector;
    
    // Start is called before the first frame update
    void Start()
    {
        playerStats = GameObject.Find("Player").GetComponent<PlayerStats>();
        modeDetector = GameObject.Find("ModeDetector").GetComponent<ModeDetector>();
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
            playerStats.TakeStar();
        }
    }
    
    private GameObject InsertParticles()
    {
        var path = "Prefabs/StarParticle";
		
        var prefab = Resources.Load<GameObject>(path);
        var pos = gameObject.transform.position;

        return Instantiate(prefab, pos, Quaternion.identity);
    }
} 

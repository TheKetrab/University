using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CollectEnergy : MonoBehaviour
{
    private ModeDetector modeDetector;
    private PlayerStats playerStats;
    
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
            AddEnergy();
        }
    }

    private void AddEnergy()
    {
        Rigidbody rb = GameObject.Find("Player").gameObject.transform.Find("Sphere").gameObject.GetComponent<Rigidbody>();
        rb.velocity = new Vector3(rb.velocity.x,0f,rb.velocity.z);
        rb.AddForce(Vector3.up * 250f);
    }
    
    private GameObject InsertParticles()
    {
        var path = "Prefabs/AddEnergyParticle";
		
        var prefab = Resources.Load<GameObject>(path);
        var pos = gameObject.transform.position;

        return Instantiate(prefab, pos, Quaternion.identity);
    }

}

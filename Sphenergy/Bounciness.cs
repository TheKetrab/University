using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Assertions;





/* ----- ----- ----- ----- -----
 * TYPES: CONCRETE, SAND, WATER, SNOW, WOOD
 */

public class Bounciness : MonoBehaviour
{
    public string type;
    private Collider coll;

    private AudioSource audioSource;
    
    
    // Start is called before the first frame update
    void Start()
    {
        coll = GetComponent<Collider>();
        
        Assert.IsNotNull(type);
        SetMaterialParameters();
        
        InstallAudioSource();
    }

    private void InstallAudioSource()
    {
        audioSource = gameObject.AddComponent<AudioSource>();
        AudioClip clip = Resources.Load<AudioClip>("Bang");
        audioSource.clip = clip;
    }

    private void SetMaterialParameters()
    {
        if (type.Equals("CONCRETE"))
            coll.material.bounciness *= 0.5f;

        else if (type.Equals("SAND"))
            coll.material.bounciness = 1.0f;

        else if (type.Equals("WOOD"))
            coll.material.bounciness *= 0.25f;

        
        else
            /* todo throw error */ print("UNKNOW BOUNCINES");


    }

    private void OnCollisionEnter(Collision other)
    {
        audioSource.Play();
    }
}

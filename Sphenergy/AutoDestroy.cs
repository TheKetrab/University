using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AutoDestroy : MonoBehaviour
{

    public int SecToDestroy;
    
    // Start is called before the first frame update
    void Start()
    {
        Invoke("Des",SecToDestroy);
    }

    void Des()
    {
        Destroy(gameObject);
    }
    
}

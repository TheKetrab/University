using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Places : MonoBehaviour
{
    public List<Place> places;

    public bool addItem(Place p)
    {
        foreach (var place in places)
            if (p.name.Equals(place.name))
                return false;

        places.Add(p);
        return true;
    }
    
    
    
    
    
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Mover : MonoBehaviour
{
    public List<Place> places;
    private int actual;

    public bool loop;
    public float speed;
    
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        GoToActualPlace();
        
        if (loop && onPos())
            NextPos();
        
    }
    

    void GoToActualPlace()
    {
        transform.position = Vector3.Lerp(transform.position,places[actual].pos,speed); // goto actual place        
    }

    bool onPos()
    {
        return transform.position == places[actual].pos;
    }

    void NextPos()
    {
        actual = (actual + 1) % places.Count;        
    }
    
    
    
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Fan : MonoBehaviour
{
    public GameObject father;
    public Vector3 force;
    private AudioSource audioSource;
    private float fanForce = 0.025f;


    private void Awake()
    {
        audioSource = GetComponent<AudioSource>();
    }


    
    private void OnTriggerStay(Collider other)
    {
        // EXIT IF
        if (!other.gameObject.CompareTag("Player"))
            return;

        var player = other.gameObject;
        player.transform.position += Vector3.up * fanForce;
        
    }

    private void OnTriggerEnter(Collider other)
    {
        // EXIT IF
        if (!other.gameObject.CompareTag("Player"))
            return;

        
        var player = other.gameObject;
        player.GetComponent<Rigidbody>().useGravity = false;
        
        
        audioSource.Play();


        var rb = other.gameObject.GetComponent<Rigidbody>();        
        rb.velocity = new Vector3(rb.velocity.x,0f,rb.velocity.z);


    }

    private void OnTriggerExit(Collider other)
    {
        // EXIT IF
        if (!other.gameObject.CompareTag("Player"))
            return;


        var player = other.gameObject;
        player.GetComponent<Rigidbody>().useGravity = true;
        
        
        audioSource.Pause();

    }

    public static void SetRangeBox(GameObject fan, float range)
    {
        var rangeBox = fan.transform.Find("Range").gameObject;
        rangeBox.transform.localScale = new Vector3(1f, range, 1f);
        rangeBox.transform.localPosition = new Vector3(0f, range / 2f + 0.25f, 0f);
    }
}

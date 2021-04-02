using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Assertions;

public class Spikes : MonoBehaviour
{
    public GameObject manager;

    private PlayMode playMode;
    private ModeDetector modeDetector;
    
    // Start is called before the first frame update
    void Start()
    {
        manager = GameObject.Find("Manager");
        Assert.IsNotNull(manager);
        playMode = manager.GetComponent<PlayMode>();
        modeDetector = GameObject.Find("ModeDetector").GetComponent<ModeDetector>();
    }

    private void OnCollisionEnter(Collision other)
    {
        if (other.gameObject.CompareTag("Player")
        && (modeDetector.mode.Equals("PLAYMODE")
            || modeDetector.mode.Equals("GAME")))
        {
            playMode.ResetPlayMode();
        }

    }
}

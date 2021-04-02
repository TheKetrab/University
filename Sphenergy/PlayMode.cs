using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayMode : MonoBehaviour
{

    public int state; // 0 - nothing, 1 - playmode

    public GameObject specials;
    
    public GameObject player;
    public Vector3 ballDefaultPosition;



    private PlayerStats playerStats;
    private ModeDetector modeDetector;
    private SwitchCamera switchCamera;

    
    // Start is called before the first frame update
    void Start()
    {
        playerStats = player.GetComponent<PlayerStats>();
        modeDetector = GameObject.Find("ModeDetector").GetComponent<ModeDetector>();
        switchCamera = gameObject.GetComponent<SwitchCamera>();
    }

    // Update is called once per frame
    void Update()
    {

        if (Input.GetKeyDown(KeyCode.Backspace))
            ResetPlayMode();
            
        // ----- ----- -----
        if (state == 0 && modeDetector.mode.Equals("PLAYMODE"))
            StartPlayMode();

        else if (state == 1 && !modeDetector.mode.Equals("PLAYMODE"))
            EndPlayMode();
        // ----- ----- -----
        
            
    }

    public void StartPlayMode()
    {
        print("START");
        ballDefaultPosition = player.transform.Find("Sphere").transform.position;
        player.transform.Find("Sphere").GetComponent<Rigidbody>().isKinematic = false;
        
        // kamera
        switchCamera.DisableActiveCamera();
        player.transform.Find("PlayerCamera").gameObject.SetActive(true);
        
        state = 1;
    }

    public void EndPlayMode()
    {
        print("END");

        EnableSpecials();
        StopPlayer();
        playerStats.ResetStats();
        player.transform.Find("Sphere").GetComponent<Rigidbody>().isKinematic = true;
        player.transform.Find("Sphere").transform.position = ballDefaultPosition;

        // kamera
        player.transform.Find("PlayerCamera").gameObject.SetActive(false);
        switchCamera.ResetActiveCamera();


        state = 0;
    }

    public void EnableSpecials()
    {
        foreach (Transform child in specials.transform)
            child.gameObject.SetActive(true);

    }
    

    public void ResetPlayMode()
    {
        print("RESET");

        EnableSpecials();
        StopPlayer();
        playerStats.ResetStats();
        player.transform.Find("Sphere").transform.position = ballDefaultPosition;
    }

    public void StopPlayer()
    {
        player.transform.Find("Sphere").GetComponent<Rigidbody>().velocity = Vector3.zero;
        player.transform.Find("Sphere").GetComponent<Rigidbody>().angularVelocity = Vector3.zero; 
    }
    
}

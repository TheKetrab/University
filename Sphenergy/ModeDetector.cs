using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ModeDetector : MonoBehaviour
{
    public Text activeModeText;
    public string mode;
    private string prevMode;
    
    // Start is called before the first frame update
    void Start()
    {
        ChangeActiveMode("TRANSFORM");
    }

    // Update is called once per frame
    void Update()
    {
        /* ----- ===== TRANSFORM ===== ----- */
        if (Input.GetKeyDown(KeyCode.T))
            ChangeActiveMode("TRANSFORM");
        
        
        /* ----- ===== DELETE ===== ----- */
        else if (Input.GetKeyDown(KeyCode.D))
            ChangeActiveMode("DELETE");

        /* ----- ===== CAMERA ===== ----- */
        else if (Input.GetKeyDown(KeyCode.C))
            ChangeActiveMode("CAMERA");
        
        else if (Input.GetKeyDown(KeyCode.LeftAlt))
            ChangeActiveMode("CAMERA");
        
        else if (Input.GetKeyUp(KeyCode.LeftAlt))
            ChangeActiveMode(prevMode);


        /* ----- ===== PLAYMODE ===== ----- */
        else if (Input.GetKeyDown(KeyCode.P))
            ChangeActiveMode("PLAYMODE");

    }

    public void InGamePlayMode()
    {
        mode = "GAME";
    }

    void ChangeActiveMode(string s)
    {
        prevMode = mode;
        mode = s;
        activeModeText.text = "Active mode: " + s;
    }
}

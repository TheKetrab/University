using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class FinishPanel : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }


    public void ToMenu()
    {
        SceneManager.LoadScene("Scenes/Menu");
    }
    
    
    public void Restart()
    {
        // disable finish panel
        var finishPanel = GameObject.Find("Canvas").gameObject.transform.Find("FinishPanel").gameObject;
        finishPanel.SetActive(false);

        // restart
        var manager = GameObject.Find("Manager").gameObject;
        var playmode = manager.GetComponent<PlayMode>();
        playmode.ResetPlayMode();
        
        // defreeze player
        var player = GameObject.Find("Player");        
        player.transform.Find("Sphere").GetComponent<Rigidbody>().isKinematic = false;
    }

    
}

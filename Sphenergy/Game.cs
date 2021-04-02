using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Game : MonoBehaviour
{
    public int totalCoins;
    public int totalStars;
    
    
    private Serialization serialization;
    private ModeDetector modeDetector;
    
    // Start is called before the first frame update
    void Start()
    {
        serialization = GetComponent<Serialization>();
        modeDetector = GameObject.Find("ModeDetector").GetComponent<ModeDetector>();
        
        
        
        serialization.LoadMap();
        modeDetector.InGamePlayMode();

        CountCollectibles();

    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
            SceneManager.LoadScene("Scenes/Menu");   
    }


    void CountCollectibles()
    {
        int coinCounter = 0;
        int starCounter = 0;

        GameObject specials = GameObject.Find("Specials").gameObject;
        foreach (Transform child in specials.transform)
        {
            if (child.name.Equals("Coin"))
                coinCounter++;

            if (child.name.Equals("Star"))
                starCounter++;

        }

        totalCoins = coinCounter;
        totalStars = starCounter;
    }
    
}

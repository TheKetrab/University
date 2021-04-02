using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class Finish : MonoBehaviour
{
    
    private PlayerStats playerStats;
    private ModeDetector modeDetector;
    private Game game;
    
    
    private void Start()
    {
        modeDetector = GameObject.Find("ModeDetector").GetComponent<ModeDetector>();
        playerStats = GameObject.Find("Player").GetComponent<PlayerStats>();
        game = GameObject.Find("Manager").GetComponent<Game>();
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.CompareTag("Player"))
        {
            if (modeDetector.mode.Equals("PLAYMODE"))
                EndLevelPlayMode();
            else if (modeDetector.mode.Equals("GAME"))
                EndLevelGame();
            
        }
    }

    public void EndLevelPlayMode()
    {
        print("END GAME; stars: " + playerStats.stars + " coins: " + playerStats.coins);
    }
    
    public void EndLevelGame()
    {
        PlaySound();
        
        var finishPanel = GameObject.Find("Canvas").gameObject.transform.Find("FinishPanel").gameObject;
        
        SetFinishPanel(finishPanel);
        finishPanel.SetActive(true);
        finishPanel.GetComponent<Animator>().SetTrigger("EnableFinishPanel");

        var player = GameObject.Find("Player");
        
        player.transform.Find("Sphere").GetComponent<Rigidbody>().isKinematic = true;
        player.transform.Find("Sphere").GetComponent<Rigidbody>().velocity = Vector3.zero;
        player.transform.Find("Sphere").GetComponent<Rigidbody>().angularVelocity = Vector3.zero; 
        
    }

    public void SetFinishPanel(GameObject finishPanel)
    {
        var coinText = finishPanel.transform.Find("CoinText").gameObject.GetComponent<TextMeshProUGUI>();
        coinText.text = "Coins: " + playerStats.coins + "/" + game.totalCoins;

        var starText = finishPanel.transform.Find("StarText").gameObject.GetComponent<TextMeshProUGUI>();
        starText.text = "Stars: " + playerStats.stars + "/" + game.totalStars;
        
        var timeText = finishPanel.transform.Find("TimeText").gameObject.GetComponent<TextMeshProUGUI>();
        timeText.text = "Time: " + playerStats.time + "''";

    }

    private void PlaySound()
    {
        var sound = gameObject.GetComponent<AudioSource>();
        sound.Play();
    }
    

}

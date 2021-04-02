using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SelectLevel : MonoBehaviour
{
    private Animator mainMenuAnimator;
    private Animator selectLevelAnimator;

    private void Start()
    {
        selectLevelAnimator = gameObject.GetComponent<Animator>();
        mainMenuAnimator = GameObject.Find("MainMenu").gameObject.GetComponent<Animator>();
    }


    public void LoadLevel(string name)
    {
        BetweenScenes.pathToLevel = Application.dataPath + "/maps/" + name + ".map";
        print("PATH: " + BetweenScenes.pathToLevel);
        
        PlayGame();
    }
    
    public void FromFile()
    {
        BetweenScenes.pathToLevel = "EMPTY";
        PlayGame();
    }


    public void PlayGame()
    {
        SceneManager.LoadScene("Scenes/Game");
    }

    
    
    public void Back()
    {
        selectLevelAnimator.SetTrigger("GoOutScreen");
        mainMenuAnimator.SetTrigger("GoOnScreen");

    }

    
    
}

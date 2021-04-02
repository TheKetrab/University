using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class MainMenu : MonoBehaviour
{
    private Animator mainMenuAnimator;
    private Animator selectLevelAnimator;

    private void Start()
    {
        mainMenuAnimator = gameObject.GetComponent<Animator>();
        selectLevelAnimator = GameObject.Find("SelectLevel").gameObject.GetComponent<Animator>();
        
        mainMenuAnimator.SetTrigger("GoOnScreen");
        
    }

    public void PlayGame()
    {
        mainMenuAnimator.SetTrigger("GoOutScreen");
        selectLevelAnimator.SetTrigger("GoOnScreen");
    }

    public void Editor()
    {
        SceneManager.LoadScene("Scenes/Editor");
    }

    public void Quit()
    {
        Application.Quit();
    }

    

}

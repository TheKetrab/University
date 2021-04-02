using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SwitchCamera : MonoBehaviour
{
    public GameObject c0;
    public GameObject c45;
    public GameObject c90;
    public GameObject c135;
    public GameObject c180;

    private GameObject[] cams;
    private int actualCam;
    
    // Start is called before the first frame update
    void Start()
    {
        cams = new GameObject[5];
        cams[0] = c0;
        cams[1] = c45;
        cams[2] = c90;
        cams[3] = c135;
        cams[4] = c180;

        c0.SetActive(false);
        c45.SetActive(false);
        c90.SetActive(true);
        c135.SetActive(false);
        c180.SetActive(false);
        actualCam = 2;
    }

    public void NextCamera()
    {
        cams[actualCam].SetActive(false);
        actualCam = (actualCam + 1) % 5;
        cams[actualCam].SetActive(true);
    }

    public void ResetActiveCamera()
    {
        cams[actualCam].SetActive(true);
    }

    public void DisableActiveCamera()
    {
        cams[actualCam].SetActive(false);
    }
}

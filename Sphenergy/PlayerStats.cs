using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerStats : MonoBehaviour
{
    public int stars;
    public int coins;
    public int time;

    private void Awake()
    {
        InvokeRepeating("Timer",0,1);
    }

    public void ResetStats()
    {
        stars = 0;
        coins = 0;
        time = 0;
    }

    public void TakeStar()
    {
        stars += 1;
    }

    public void TakeCoin()
    {
        coins += 1;
    }
    
    public void Timer()
    {
        time += 1;
    }

    
}

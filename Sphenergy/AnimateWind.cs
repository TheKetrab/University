using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AnimateWind : MonoBehaviour
{

    public float ScrollX;
    public float ScrollY;

    private Renderer renderer;

    private void Awake()
    {
        renderer = GetComponent<Renderer>();
    }

    // Update is called once per frame
    void Update()
    {
        float OffsetX = Time.time * ScrollX;
        float OffsetY = Time.time * ScrollY;
        renderer.material.mainTextureOffset = new Vector2(OffsetX, OffsetY);

    }
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DeleteBlock : MonoBehaviour
{
    private ModeDetector modeDetector;


    // Start is called before the first frame update
    void Start()
    {
        modeDetector = GameObject.Find("ModeDetector").GetComponent<ModeDetector>();
    }

    // Update is called once per frame
    void Update()
    {
        if (modeDetector.mode.Equals("DELETE"))
            DeleteByClick();

    }

    private void DeleteByClick()
    {
        if (Input.GetMouseButtonDown(0))
        {

            RaycastHit hitInfo = new RaycastHit();
            bool hit = Physics.Raycast(Camera.main.ScreenPointToRay(Input.mousePosition), out hitInfo);
            if (hit)
            {
                if (hitInfo.transform.gameObject.CompareTag("BLOCK")
                    || hitInfo.transform.gameObject.CompareTag("SPECIAL"))
                    Destroy(hitInfo.transform.gameObject);
            }
        }
    }
}
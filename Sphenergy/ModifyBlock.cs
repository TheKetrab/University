using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.UI;

public class ModifyBlock : MonoBehaviour
{

    public GameObject selectionCube; // prefab nakladany jako dziecko na zaznaczony obiekt
    
    public GameObject prefab;
    public bool isFrozen;

    public GameObject camera;
    private GameObject blocks;
    private GameObject specials;
    private ModeDetector modeDetector;
    
    public Text transformText;
    public Text rotationText;
    public Text scaleText;

    public GameObject blockProperties;

    private bool isBlockSelected;
    private bool isBlockPropertiesOpened;
    

    private int rot;

    public void SetInfoText(string mode, float x, float y, float z)
    {
        string text = "X: " + x + "\n Y: " + y + "\n Z: " + z;

        if (mode.Equals("TRANSFORM"))
            transformText.text = text;
        else if (mode.Equals("ROTATION"))
            rotationText.text = text;
        else if (mode.Equals("SCALE"))
            scaleText.text = text;
        else
            print("Unknow text in function SetInfoText");
    }
    
    // Start is called before the first frame update
    void Start()
    {
        blocks = GameObject.Find("Blocks");
        specials = GameObject.Find("Specials");
        camera = GameObject.Find("Camera");
        modeDetector = GameObject.Find("ModeDetector").GetComponent<ModeDetector>();
        
        rot = 0;
        
        
        
    }

    // Update is called once per frame
    void Update()
    {
        
        // ----- ----- -----
        // BLOCK PROPERTIES
        if (Input.GetKeyDown(KeyCode.Tab))
        {
            if (isBlockPropertiesOpened)
            {
                blockProperties.SetActive(false);
                isBlockPropertiesOpened = false;
            }
            else
            {
                blockProperties.SetActive(true);
                isBlockPropertiesOpened = true;
            }
        }
        // ----- ----- -----

        // exit if
        if (isBlockPropertiesOpened)
            return;

        
        
        
        if (Input.GetMouseButtonDown(0))
        {
            print("LeftMousebutton");

            if (prefab != null)
            {
                print("NOtNull");
                FreezeObject();
            }
            else
                TryToActiveFrozenBlock();
        }

        
        if (!isFrozen && prefab != null)
        {
            ModifyBlockFunc();

            
            if (Input.GetKeyDown(KeyCode.Return))
                FreezeObject();

        }

    }

    public void RotateBlock()
    {
        if (Input.GetKeyDown(KeyCode.R)     // 'R'
            || Input.GetMouseButtonDown(1)) // Right Mouse Button
        {
            if(Input.GetKey(KeyCode.X))
                prefab.transform.Rotate(90,0,0);
            
            else if(Input.GetKey(KeyCode.Y))
                prefab.transform.Rotate(0,90,0);

            else
                prefab.transform.Rotate(0,0,90);

            var rot = prefab.gameObject.transform.eulerAngles;
            SetInfoText("ROTATION", rot.x, rot.y, rot.z);
        }
    }

    public void TryToActiveFrozenBlock()
    {        
        print("TryToActiveFrozenBlock");

        RaycastHit hitInfo = new RaycastHit();
        bool hit = Physics.Raycast(Camera.main.ScreenPointToRay(Input.mousePosition), out hitInfo);
        if (hit)
        {
            print("HIT: " + hitInfo.transform.gameObject);
            if (hitInfo.transform.gameObject.CompareTag("BLOCK")
                || hitInfo.transform.gameObject.CompareTag("SPECIAL"))
                    DefreezeObject(hitInfo.transform.gameObject);
        }

    }


    public void InsertBlock(string path)
    {
        
        var pos = new Vector3(camera.transform.position.x, camera.transform.position.y, 0f);
        var pref = Resources.Load<GameObject>(path);

        if (path.Contains("specials"))
        {
            var special = Instantiate(pref, pos, Quaternion.identity);
            special.name = pref.name;
            special.transform.parent = specials.transform;              
            prefab = special;

            
        }

        else
        {            
            var block = Instantiate(pref, pos, Quaternion.identity);
            block.name = pref.name;
            block.transform.parent = blocks.transform;
            prefab = block;
        }        
        isFrozen = false;
        EnableSelection();

    }
    
    
    public void ModifyBlockFunc()
    {

        if (modeDetector.mode.Equals("TRANSFORM"))
        {
            var pos = prefab.transform.position;

            if (Input.GetKeyDown(KeyCode.UpArrow) || (Input.GetKey(KeyCode.LeftControl) && Input.GetAxis("Mouse Y") > 0))
                pos.y += 0.25f;

            if (Input.GetKeyDown(KeyCode.DownArrow) || (Input.GetKey(KeyCode.LeftControl) && Input.GetAxis("Mouse Y") < 0))
                pos.y -= 0.25f;

            if (Input.GetKeyDown(KeyCode.RightArrow) || (Input.GetKey(KeyCode.LeftControl) && Input.GetAxis("Mouse X") > 0))
                pos.x += 0.25f;

            if (Input.GetKeyDown(KeyCode.LeftArrow) || (Input.GetKey(KeyCode.LeftControl) && Input.GetAxis("Mouse X") < 0))
                pos.x -= 0.25f;

            if (Input.GetKeyDown(KeyCode.A))
                pos.z += 0.25f;

            if (Input.GetKeyDown(KeyCode.Z))
                pos.z -= 0.25f;

            prefab.transform.position = pos;
            SetInfoText("TRANSFORM", pos.x, pos.y, pos.z);

            RotateBlock();

            if (Input.GetKey(KeyCode.S))
            {

                var sca = prefab.transform.localScale;

                if (Input.GetKeyDown(KeyCode.UpArrow))
                    sca.y += 0.25f;

                if (Input.GetKeyDown(KeyCode.DownArrow))
                    sca.y -= 0.25f;

                if (Input.GetKeyDown(KeyCode.RightArrow))
                    sca.x += 0.25f;


                if (Input.GetKeyDown(KeyCode.LeftArrow))
                    sca.x -= 0.25f;

                if (Input.GetKeyDown(KeyCode.A))
                    sca.z += 0.25f;

                if (Input.GetKeyDown(KeyCode.Z))
                    sca.z -= 0.25f;

                prefab.transform.localScale = sca;
            }
        }
    }

    public void EnableSelection()
    {
        var selection = Instantiate(selectionCube, prefab.transform.position, Quaternion.identity);
        print("Selection: " + selection);
        selection.name = "Selection";
        selection.transform.parent = prefab.transform;
    }

    public void DisableSelection()
    {
        Destroy(prefab.transform.Find("Selection").gameObject);        
    }
    
    public void FreezeObject()
    {
        DisableSelection();
        isFrozen = true;
        prefab = null;
    }
    
    
    public void DefreezeObject(GameObject o)
    {
        isFrozen = false;
        prefab = o;
        EnableSelection();
    }

}

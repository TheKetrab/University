using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using UnityEngine;
using UnityEngine.SceneManagement;

public class EditorLoop : MonoBehaviour
{
    public GameObject camera;
    public GameObject blocks;

    public ModifyBlock mb;
    private ModeDetector modeDetector;

    public GameObject player;


    // Start is called before the first frame update
    void Start()
    {

        camera = GameObject.Find("Camera");
        blocks = GameObject.Find("Blocks");
        modeDetector = GameObject.Find("ModeDetector").GetComponent<ModeDetector>();
        
        DisablePlayer();
        
        mb = gameObject.GetComponent<ModifyBlock>();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
            SceneManager.LoadScene("Scenes/Menu");

        
        if (modeDetector.mode.Equals("CAMERA"))
            CameraMove();
    }

    public void SelectObject()
    {

        var pos = new Vector3(camera.transform.position.x, camera.transform.position.y, 0f);
        const string path = "prefabs/blocks/Normal1x1";
        var pref = Resources.Load<GameObject>(path);
        var trans = pos;

        var block = Instantiate(pref, trans, Quaternion.identity);
        block.transform.parent = blocks.transform;
        mb.prefab = block;
        mb.isFrozen = false;

    }

    public void CameraMove()
    {
        Vector3 pos = camera.transform.position;

        // ----- ===== MOUSE ===== -----
        int camFactor = 2;
        
        if (Input.mouseScrollDelta.y > 0f)
            pos.z += 0.25f * camFactor;
        

        if (Input.mouseScrollDelta.y < 0f)
            pos.z -= 0.25f * camFactor;
        
        if (Input.GetMouseButton(0))
        {
            if (Input.GetAxis("Mouse X") < 0)
                pos.x += 0.25f * camFactor;
            
            if (Input.GetAxis("Mouse X") > 0)
                pos.x -= 0.25f * camFactor;
            
            if (Input.GetAxis("Mouse Y") < 0)
                pos.y += 0.25f * camFactor;

            if (Input.GetAxis("Mouse Y") > 0)
                pos.y -= 0.25f * camFactor;
        }
            
        // ----- ===== KB ===== -----

        if (Input.GetKey(KeyCode.UpArrow))
            pos.y += 0.25f;

        if (Input.GetKey(KeyCode.DownArrow))
            pos.y -= 0.25f;

        if (Input.GetKey(KeyCode.RightArrow))
            pos.x += 0.25f;


        if (Input.GetKey(KeyCode.LeftArrow))
            pos.x -= 0.25f;

        if (Input.GetKey(KeyCode.A))
            pos.z += 0.25f;

        if (Input.GetKey(KeyCode.Z))
            pos.z -= 0.25f;

        camera.transform.position = pos;

    }

    public void DisablePlayer()
    {
        player.transform.Find("Sphere").GetComponent<Rigidbody>().isKinematic = true;
        player.transform.Find("PlayerCamera").gameObject.SetActive(false);
        gameObject.GetComponent<SwitchCamera>().ResetActiveCamera();
        
    }
}
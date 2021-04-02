using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Assertions;
using UnityEngine.UI;

public class BlockProperties : MonoBehaviour
{
    private ModifyBlock modifyBlock;
    private GameObject prop1input;
    private Text prop1inputText;
    
    
    // Start is called before the first frame update
    void Start()
    {
        modifyBlock = GameObject.Find("Manager").gameObject.GetComponent<ModifyBlock>();
        prop1input = GameObject.Find("BlockProperties").gameObject.transform.Find("Prop1Input").gameObject;
        prop1inputText = prop1input.transform.Find("Text").gameObject.GetComponent<Text>();
        
        Assert.IsNotNull(modifyBlock);
        Assert.IsNotNull(prop1input);
        Assert.IsNotNull(prop1inputText);

    }

    public void ApplyProperties()
    {
        if (modifyBlock.prefab.name.Equals("Fan"))
        {
            var range = float.Parse(prop1inputText.text);
            // TODO ! zaokraglenie do 0.25
            print("RANGE: " + range);
            Fan.SetRangeBox(modifyBlock.prefab, range);            
        }

        else
        {
            print("ApplyProperties do " + modifyBlock.prefab.name);
        }
    }
}

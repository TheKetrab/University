using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;
using UnityEngine.Assertions;
using Crosstales.FB;

public class Serialization : MonoBehaviour
{
    public GameObject blocks;
    public GameObject specials;
    //public GameObject places;

    public string path;

    
    public void SetPath(string mode)
    {
        if (mode.Equals("LOAD"))
            path = FileBrowser.OpenSingleFile("Wybór mapy","","map");
        else if (mode.Equals("SAVE"))
            path = FileBrowser.SaveFile("Zapisywanie mapy", "", "", "map");
        else
            print("Unknown mode at SetPath: " + mode);    
    }
    
    // Start is called before the first frame update
    void Start()
    {
        Assert.IsNotNull(blocks);
        Assert.IsNotNull(specials);
        //Assert.IsNotNull(places);

    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public static Vector3 StringToVector3(string sVector)
    {

        // Remove the parentheses
        if (sVector.StartsWith ("(") && sVector.EndsWith (")")) {
            sVector = sVector.Substring(1, sVector.Length-2);
        }
 
        // split the items
        string[] sArray = sVector.Split(',');
 
        // store as a Vector3
        Vector3 result = new Vector3(
            float.Parse(sArray[0]),
            float.Parse(sArray[1]),
            float.Parse(sArray[2]));
 
        return result;
    }
   

    
    public void SaveMap()
    {
        SetPath("SAVE");
        WriteString();
    }

    public void LoadMap()
    {
        if (BetweenScenes.pathToLevel.Equals("EMPTY")) // nothing
        {
            SetPath("LOAD");
        }
        else
        {
            path = BetweenScenes.pathToLevel;
        }
        

        ReadString();
    }

    string FanInfo(GameObject block)
    {
        var pos = block.transform.position;
        var rot = block.transform.eulerAngles;
        var sca = block.transform.localScale;
        var ran = block.transform.Find("Range").gameObject.transform.localScale.y;
        
        string result = "{|";
        result += block.gameObject.name; 
        result += "|";
        result += "(" + pos.x + ", " + pos.y + ", " + pos.z + ")";
        result += "|";
        result += "(" + rot.x + ", " + rot.y + ", " + rot.z + ")";
        result += "|";
        result += "(" + sca.x + ", " + sca.y + ", " + sca.z + ")";
        result += "|";
        result += ran;
        result += "|}";

        return result;
    }
    
    string Info(GameObject block)
    {
        
        if (block.gameObject.name.Equals("Fan"))
            return FanInfo(block);
        
        var pos = block.transform.position;
        var rot = block.transform.eulerAngles;
        var sca = block.transform.localScale;
        
        string result = "{|";
        result += block.gameObject.name; 
        result += "|";
        result += "(" + pos.x + ", " + pos.y + ", " + pos.z + ")";
        result += "|";
        result += "(" + rot.x + ", " + rot.y + ", " + rot.z + ")";
        result += "|";
        result += "(" + sca.x + ", " + sca.y + ", " + sca.z + ")";
        result += "|}";

        return result;
    }

    private string StartPointLine()
    {
        Vector3 pos = gameObject.GetComponent<PlayMode>().ballDefaultPosition;

        string result = "";
        result += "{|";
        result += "StartPoint";
        result += "|";
        result += "(" + pos.x + ", " + pos.y + ", " + pos.z + ")";
        result += "|}";

        return result;
    }
    
    void WriteString()
    {
        //string path = "Assets/Resources/test.txt";
        StreamWriter writer = new StreamWriter(path,false);
        
        writer.WriteLine("[INFO]");
        writer.WriteLine(StartPointLine());
        
        writer.WriteLine("[BLOCKS]");
        foreach (Transform child in blocks.transform)
            writer.WriteLine(Info(child.gameObject));
        
        writer.WriteLine("[SPECIALS]");
        foreach (Transform child in specials.transform)
            writer.WriteLine(Info(child.gameObject));

        //writer.WriteLine("[PLACES]");
        //WritePlaces(writer);
            

        writer.Close();
    }
/*
    private void WritePlaces(StreamWriter writer)
    {
        Places p = places.GetComponent<Places>();
        
        foreach (var place in p.places)
        {
            writer.WriteLine(place);
        }
    }
    */
    void ReadString()
    {
        //string path = "Assets/Resources/test.txt";
        StreamReader reader = new StreamReader(path);

        string state = "";
        
        while (!reader.EndOfStream)
        {
            string line = reader.ReadLine();

            if (line.Equals("[INFO]"))
                state = "INFO";
            else if (line.Equals("[BLOCKS]"))
                state = "BLOCKS";
            else if (line.Equals("[SPECIALS]"))
                state = "SPECIALS";
            else
                InterpreteLine(line, state);
        }

        reader.Close();
    }

    void InterpreteLine(string line, string state)
    {
        if (state.Equals("INFO"))
        {
            InterpreteInfo(line);
        }
        
        else if (state.Equals("BLOCKS"))
        {
            string[] splitted = line.Split("|"[0]);
            InsertBlock(splitted[1],StringToVector3(splitted[2]),StringToVector3(splitted[3]),StringToVector3(splitted[4]));
        }
        
        else if (state.Equals("SPECIALS"))
        {
            string[] splitted = line.Split("|"[0]);
            
            if (splitted[1].Equals("Fan"))
                InsertFan(StringToVector3(splitted[2]),StringToVector3(splitted[3]),StringToVector3(splitted[4]), float.Parse(splitted[5]));
            else
                InsertSpecial(splitted[1],StringToVector3(splitted[2]),StringToVector3(splitted[3]),StringToVector3(splitted[4]));            
        }

        else
        {
            print("Unknow state at InterpreteLine: " + state);
        }
    }

    public void InsertFan(Vector3 pos, Vector3 rot, Vector3 sca, float range)
    {
        var fan = InsertSpecial("Fan", pos, rot, sca);
        Fan.SetRangeBox(fan, range);
    }

    
    public void InterpreteInfo(string line)
    {
        string[] splitted = line.Split("|"[0]);

        string item = splitted[1];
        
        if (item.Equals("StartPoint"))
        {
            // zmieniamy pozycje glownego obiektu player, a nie sfery
            Vector3 pos =  StringToVector3(splitted[2]);
            GameObject.Find("Sphere").gameObject.transform.position = pos;
            gameObject.GetComponent<PlayMode>().ballDefaultPosition = pos;
        }
        
        else
            print("Unknown item at InterpreteInfo: " + item);
    }
    
    
    public void InsertBlock(string name, Vector3 pos, Vector3 rot, Vector3 sca)
    {
        var pref = Resources.Load<GameObject>("prefabs/blocks/" + name);
        var block = Instantiate(pref, pos, Quaternion.identity);
        block.transform.eulerAngles = rot;
        block.transform.localScale = sca;
        block.name = pref.name;
        block.transform.parent = blocks.transform;
        
    }

    public GameObject InsertSpecial(string name, Vector3 pos, Vector3 rot, Vector3 sca)
    {
        var pref = Resources.Load<GameObject>("prefabs/specials/" + name);
        var special = Instantiate(pref, pos, Quaternion.identity);
        special.transform.eulerAngles = rot;
        special.transform.localScale = sca;
        special.name = pref.name;
        special.transform.parent = specials.transform;

        return special;
    }

    
}

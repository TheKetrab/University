using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Place
{
    public string name;
    public Vector3 pos;
    public Vector3 rot;
    public Vector3 sca;

    public Place(string name, Vector3 pos, Vector3 rot, Vector3 sca)
    {
        this.pos = pos;
        this.rot = rot;
        this.sca = sca;
    }
    
    public override string ToString()
    {        
        string result = "{|";
        result += name; 
        result += "|";
        result += "(" + pos.x + ", " + pos.y + ", " + pos.z + ")";
        result += "|";
        result += "(" + rot.x + ", " + rot.y + ", " + rot.z + ")";
        result += "|";
        result += "(" + sca.x + ", " + sca.y + ", " + sca.z + ")";
        result += "|}";

        return result;
    }
}

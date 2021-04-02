using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using TMPro;
using UnityEditor;
using UnityEngine;
using Random = System.Random;
using UnityEngine.SceneManagement;

public class GameSystem : MonoBehaviour
{

	//   z = i
	//   ^
	//   |
	//   |
	//   + - - > x = j
	// (0,0)
	
	
	public int Map = 1;
	public GameObject player;
	public GameObject opponent1;
	public GameObject opponent2;
	public GameObject opponent3;

	
	private const int Dim = 16;
	private Queue<Vector3Int> _opponents;

	public TextMeshPro GameOver;
	public int OpponentsCount = 3;

	public void GameWin()
	{
		GameOver.text = "YOU WIN !";
		Invoke("GotoMenu", 2);
	}

	public void GameLose()
	{
		GameOver.text = "YOU LOSE !";		
		Invoke("GotoMenu", 2);
	}
	
	// Use this for initialization
	void Start ()
	{
		_opponents = new Queue<Vector3Int>();
		LoadMap(Map);
		InsertPlayer();
		InsertOpponents();
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	void InsertPlayer()
	{
		if (_opponents.Count == 0)
		{
			print("NO PLACE FOR PLAYER !");
			return;
		}

		player.transform.position = _opponents.Dequeue();
		player.GetComponent<BallMove>().goal = RoundToIntVector(player.transform.position);


	}
	
	void InsertOpponents()
	{
		
		opponent1.transform.position = _opponents.Dequeue();
		opponent1.GetComponent<BallMove>().goal = RoundToIntVector(player.transform.position);
		
		opponent2.transform.position = _opponents.Dequeue();
		opponent2.GetComponent<BallMove>().goal = RoundToIntVector(player.transform.position);

		opponent3.transform.position = _opponents.Dequeue();
		opponent3.GetComponent<BallMove>().goal = RoundToIntVector(player.transform.position);

	}

	void LoadMap (int n)
	{
		var path = "Maps/map" + n;
		var mapImg = Resources.Load<Texture2D>(path);

		for (var i=0; i<Dim; i++)
			for (var j = 0; j < Dim; j++)
				InterpretePixel(i,j,mapImg.GetPixel(j, i));

	}

	public static int D()
	{
		return Dim;
	}

	void InterpretePixel(int i, int j, Color c)
	{

		var trans = new Vector3Int(j,0,i);
		var quater = Quaternion.identity;
		var rand = UnityEngine.Random.Range(1, 4);

		var path = "Prefabs/";

		if (c == Color.black)
		{
			path = path + "Black" + rand;
		}
		else if (c == Color.green)
		{
			path = path + "Green" + rand;
		}
		else if (c == Color.red)
		{
			_opponents.Enqueue(trans);
			return;
		}
		else
		{
			// empty, white, unknown
			return;
		}

		var prefab = Resources.Load<GameObject>(path);
		Instantiate(prefab, trans, quater);
	}
	
	public void GotoMenu()
	{
		SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex - 1);
	}


	public static float Square(float a)
	{
		return a * a;
	}
	
	public static Vector3Int GetPosition(GameObject g)
	{
		var x = Mathf.RoundToInt(g.transform.position.x);
		var y = Mathf.RoundToInt(g.transform.position.y);
		var z = Mathf.RoundToInt(g.transform.position.z);

		
		return new Vector3Int(x,y,z);
	}

	public static int GetDistBetween(GameObject g1, GameObject g2)
	{
		var pos1 = GetPosition(g1);
		var pos2 = GetPosition(g2);

		var sqrt = Mathf.Sqrt(Square(pos1.x - pos2.x) + Square(pos1.z - pos2.z));
		return Mathf.RoundToInt(sqrt);
		
	}
	
	
	public static int GetDistBetween(Vector3Int pos1, Vector3Int pos2)
	{
		var sqrt = Mathf.Sqrt(Square(pos1.x - pos2.x) + Square(pos1.z - pos2.z));
		return Mathf.RoundToInt(sqrt);
		
	}

	public static bool AlmostEqualPositions(Vector3Int pos1, Vector3Int pos2)
	{
		return Math.Abs(pos1.x - pos2.x) <= 1
			&& Math.Abs(pos1.y - pos2.y) <= 1
			&& Math.Abs(pos1.z - pos2.z) <= 1;

	}

	public static Vector3Int RoundToIntVector(Vector3 pos)
	{
		var x = Mathf.RoundToInt(pos.x);
		var y = Mathf.RoundToInt(pos.y);
		var z = Mathf.RoundToInt(pos.z);

		return new Vector3Int(x, y, z);
	}

	public static bool CompareFirstTagLetters(string tag, string s)
	{
		if (tag.Length < s.Length)
			return false;

		for (int i = 0; i < s.Length; i++)
			if (s[i] != tag[i])
				return false;

		return true;
	}



}

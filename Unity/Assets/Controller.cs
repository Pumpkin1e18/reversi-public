using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Controller : MonoBehaviour {
	
	public GameObject box;
	public GameObject back_result;
	public GameObject back_pass;
	public GameObject stone_white;
	public GameObject stone_black;
	public GameObject can_put;
	public GameObject point;
	public Text text_score;
	public Text text_result;
	public static GameObject[,] boxes = new GameObject[8, 8];
	public static GameObject[,] stones = new GameObject[8, 8];
	public static GameObject[,] guides = new GameObject[8, 8];
	public static GameObject pointer;
	public static int[] scores = new int[2];
	public static int h = 8, w = 8;
	float timeElapsed = 0, timeInterval = 0.2f, timeEnd = 0;
	bool ok = false, end = false;

	// init
	void Start () {
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				boxes[i,j] = Instantiate (box, new Vector2 (0, 0), Quaternion.identity);
				float d = 0.64f;
				float y = j*d-(h/2-1)*d, x = -i*d+(w/2-1)*d;
				boxes[i,j].transform.position = new Vector2 (y, x);
				string num = (i * w + j).ToString ();
				if (i * w + j < 10)num = "0" + num;
				boxes[i,j].name = "Box" + num;
			}
		}
		pointer = Instantiate (point, new Vector2 (100, 0), Quaternion.identity);
		string s = "0000000000000000000300000032100000012300000030000000000000000000**";
		UpdateStone (s);
		back_result.SetActive (false);
	}

	// put stones
	void UpdateStone(string s){
		// Debug.Log (s);
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				Destroy (stones [i, j]);
				Destroy (guides [i, j]);
			}
		}

		scores [0] = 0;scores [1] = 0;
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				
				char c = s[i*w+j];
				if (c == '1') {
					scores [0]++;
					stones [i, j] = Instantiate (stone_black, new Vector2 (0, 0), Quaternion.identity);
				} else if (c == '2') {
					scores [1]++;
					stones [i, j] = Instantiate (stone_white, new Vector2 (0, 0), Quaternion.identity);
				} else if (c == '3') {
					guides [i, j] = Instantiate (can_put, new Vector2 (0, 0), Quaternion.identity);
				}
				float d = 0.64f;
				float y = j*d-(h/2-1)*d, x = -i*d+(w/2-1)*d;
				if (stones [i, j] != null) {
					stones [i, j].transform.position = new Vector2 (y, x);
					stones [i, j].name = "Stone" + (i * w + j).ToString ();
				} else if (guides [i, j] != null) {
					guides [i, j].transform.position = new Vector2 (y, x);
				}

			}
		}
		if (s [8 * 8] != '*') {
			float d = 0.64f;
			int y = s[8*8]-'0', x = s[8*8+1]-'0';
			float xx = x*d-(h/2-1)*d, yy = -y*d+(w/2-1)*d;
			pointer.transform.position = new Vector2 (xx, yy);
		}

	}

	//If object doesn't have collider2D, return null
	private GameObject getClickObject() {
		GameObject result = null;
		Chat chat = GetComponent<Chat> ();
		if(Input.GetMouseButtonDown (0)) {
			Vector2 tapPoint = Camera.main.ScreenToWorldPoint(Input.mousePosition);
			Collider2D collition2d = Physics2D.OverlapPoint(tapPoint);
			if (collition2d) {
				result = collition2d.transform.gameObject;
			}

			// Player move
			if (result != null && ok) {
				string name = result.name;
				string s = "move"+name[3]+name[4];
				s = chat.SendCommand (s);
				if (s [0] == '-') {
					Debug.Log ("not allowed");
				} else {
					UpdateStone (s);
					ok = false;
				}
			}

		}
		return result;
	}

	void getAction(){
		Chat chat = GetComponent<Chat> ();
		string s = "get";
		s = chat.SendCommand (s);
		text_result.text = "";
		timeInterval = 0.2f;
		back_pass.SetActive (false);
		if (s [0] == 'w') {
			ok = true;
		} else if (s [0] == 'p') {
			Debug.Log ("pass");
			timeInterval = 1f;
			back_pass.SetActive (true);
			text_result.text = "<size=10>   pass</size>";
		} else if (s [0] == 'e') {
			back_result.SetActive (true);
			timeEnd = 0;
			string res = "";
			if (scores [0] == scores [1])res = "引き分け";	// Draw
			else if(StartButton.role == 1 && scores[0] < scores[1])res = "Pumpkinの勝ち";	// Pumpkinの勝ち
			else if(StartButton.role == 2 && scores[0] > scores[1])res = "Pumpkinの勝ち";	// Pumpkinの勝ち
			else res = "あなたの勝ち";	// You win
			text_result.text = res;
			end = true;
		} else {
			UpdateStone (s);
		}
	}
	
	// Update is called once per frame
	void Update () {
		
		GameObject obj = getClickObject ();
		if (obj != null) {
			// Debug.Log (obj.name);
		}
		timeElapsed += Time.deltaTime;
		timeEnd += Time.deltaTime;
		if (timeElapsed > timeInterval && end == false) {
			timeElapsed = 0;
			getAction ();
		}
		string s = "";
		if (StartButton.role == 1) {
			s += "あなた: " + scores [0] + "\n";
			s += "Pumpkin: " + scores [1];
		} else {
			s += "あなた: " + scores [1] + "\n";
			s += "Pumpkin: " + scores [0];
		}
		text_score.text = s;
		if ((Input.GetKeyDown (KeyCode.Space) || Input.GetMouseButtonDown (0)) && end && timeEnd > 1) {
			SceneManager.LoadScene ("Result");
		}

	}
}

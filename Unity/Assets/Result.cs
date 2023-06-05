using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Result : MonoBehaviour {
	public Text text_result;
	public Text names_text, scores_text;
	public GameObject scroll_view;
	int[] scores = new int[2];

	// Use this for initialization
	void Start () {
		for(int i = 0;i < 2;i++)scores[i] = Controller.scores[i];
		scroll_view.SetActive (false);

		// PlayerPrefs.DeleteAll ();
	}

	// input alert
	public void ChangeMessage(string s){
		text_result.text = s;
	}

	// make ranking
	public void MakeRanking(){
		string message = "";
		// fight count: count, win count: count, press Enter key
		int play_cnt = PlayerPrefs.GetInt("PlayCount", 0);
		int win_cnt = PlayerPrefs.GetInt ("WinCount", 0);
		if(SceneController.jump == false)play_cnt++;
		if (InputName.input_name != "" && SceneController.jump == false)win_cnt++;
		// play_cnt++;
		// if (InputName.input_name != "")win_cnt++;
		PlayerPrefs.SetInt ("PlayCount", play_cnt);
		PlayerPrefs.SetInt ("WinCount", win_cnt);
		message += "これまでの対戦回数: <color=#00ff00>" + play_cnt + "</color>回\n";
		message += "挑戦者が勝った回数: <color=#00ff00>" + win_cnt + "</color>回\n";
		message += "<color=#ffffff>タイトルに戻るには\nSpaceキーを押してください</color>";
		text_result.text = message;

		// ranking string
		float height = 50f;
		int idx = -1, n = 0;
		string[] names_list = new string[10000];
		int[] scores_list = new int[10000];
		scroll_view.SetActive (true);
		string names_string = "", scores_string = "";
		names_string += "<color=#ffff00>勝った人の名前\n</color>";
		scores_string += "<color=#ffff00>勝った回数\n</color>";

		// set names to variable
		for (int i = 0; i < 10000; i++) {
			int cnt = PlayerPrefs.GetInt ("count"+i.ToString(), -1);
			if (cnt == -1)break;
			string name = PlayerPrefs.GetString ("name"+i.ToString());
			if (InputName.input_name == name) {idx = i;cnt++;}
			names_list [i] = name;
			scores_list [i] = cnt;
			n = i + 1;
			height += 50f;
		}
		// add new name
		if (idx == -1 && InputName.input_name != "") {
			names_list [n] = InputName.input_name;
			scores_list [n] = 1;
			idx = n;n++;
			height += 50f;
		}

		// sort
		for (int i = idx; i > 0; i--) {
			if (scores_list [i] <= scores_list [i - 1])break;
			int tmp_score = scores_list[i];
			scores_list [i] = scores_list[i-1];
			scores_list [i-1] = tmp_score;
			string tmp_name = names_list[i];
			names_list[i] = names_list[i-1];
			names_list [i-1] = tmp_name;
		}

		for (int i = 0; i < n; i++) {
			names_string += names_list[i] + "\n";
			scores_string += scores_list[i] + "\n";
		}

		// save
		for (int i = 0; i < n; i++) {
			PlayerPrefs.SetString ("name"+i.ToString(), names_list[i]);
			PlayerPrefs.SetInt ("count"+i.ToString(), scores_list[i]);
		}
		PlayerPrefs.Save ();

		// set ranking string
		names_text.GetComponent<RectTransform>().sizeDelta = new Vector2(-200f, height);	// d = 40f
		scores_text.GetComponent<RectTransform>().sizeDelta = new Vector2(200f, height);	// d = 40f
		names_text.text = names_string;
		scores_text.text = scores_string;

		string s = InputName.input_name;
		Debug.Log (s);
		if (s == "name")Debug.Log ("ok");
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKeyDown (KeyCode.Space) && InputName.done_input) {
			SceneManager.LoadScene ("Start");
			InputName.input_name = "";
			// SceneController.jump = false
			Debug.Log (Controller.scores[0]);
		}
	}
}

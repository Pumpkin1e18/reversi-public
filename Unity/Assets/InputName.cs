using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public class InputName : MonoBehaviour {

	InputField inputField;
	Result result;
	public static string input_name = "";
	public GameObject game_object;
	public GameObject input;
	bool win_lose = false;	// true:pumpkin lose, win:pumpkin win
	public static bool done_input = false;
	int[] scores = new int[2];

	void Start() {
		if (SceneController.jump) {
			result = game_object.GetComponent<Result> ();
			input_name = "";
			input.SetActive(false);
			done_input = true;
			result.MakeRanking ();
			return;
		}
		// init
		for(int i = 0;i < 2;i++)scores[i] = Controller.scores[i];
		inputField = GetComponent<InputField>();
		result = game_object.GetComponent<Result> ();
		InitInputField();
		input_name = "";
		done_input = false;
		win_lose = false;

		// win lose judge
		if (StartButton.role == 1 && scores [0] > scores [1])win_lose = true;
		if (StartButton.role == 2 && scores [0] < scores [1])win_lose = true;
		// win_lose = true;
		if (win_lose == false) {
			done_input = true;
		}
		Debug.Log ("win_lose = " + win_lose);

		// invisible
		if (win_lose == false) {
			input.SetActive (false);
			result.MakeRanking ();
		}

	}

	/// full-width = true half-width = false
	bool IsFull(char c){
		return !( ( c >= 0x0 && c < 0x81 ) || ( c == 0xf8f0 ) || ( c >= 0xff61 && c < 0xffa0 ) || ( c >= 0xf8f1 && c < 0xf8f4 ) );
	}

	// input
	public void InputLogger() {
		string input_str = inputField.text;
		int n = input_str.Length;
		// result.ChangeMessage (input_str);
		bool full = false;
		for (int i = 0; i < n; i++)
			if (IsFull(input_str [i]))full = true;
		// int byte_count = System.Text.Encoding.GetEncoding(932).GetByteCount(input_str);
		if (n > 15) {
			result.ChangeMessage ("15文字以内で入力してください");
			Debug.Log ("not allowed over 16 character name");
			InitInputField ();
			return;
		}
		if (input_str == "") {
			result.ChangeMessage ("名前を入力してください");
			Debug.Log ("input name");
			InitInputField ();
			return;
		}
		if (full) {
			result.ChangeMessage ("全角文字(日本語)を含めないでください");
			Debug.Log ("don't include full-width");
			InitInputField ();
			return;
		}

		done_input = true;
		input_name = input_str;
		Debug.Log (input_str);
		input.SetActive(false);
		result.MakeRanking();
		// InitInputField();
	}

	// init field
	void InitInputField() {
		inputField.text = "";
		inputField.ActivateInputField();
	}

	void Update(){
		if (Input.GetKeyDown (KeyCode.Return) && done_input == false) {
			// InputLogger();
		}
	}
}
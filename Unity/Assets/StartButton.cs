using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement; 

public class StartButton : MonoBehaviour {
	public static int role = -1;

	public void OnClick() {
		string s = this.gameObject.name;
		if (s == "Black")role = 1;
		if (s == "White")role = 2;
		SceneManager.LoadScene ("Main");
	}

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}

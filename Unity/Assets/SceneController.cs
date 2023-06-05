using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class SceneController : MonoBehaviour {
	float timeKeyTitle = 0, timeKeyResult = 0;
	public static bool jump = false;

	// Use this for initialization
	void Start () {
		Screen.SetResolution (800, 600, false); // true: full-screen, false;window
		// PlayerPrefs.DeleteAll ();
		// jump = false;
	}
	
	// Update is called once per frame
	void Update () {
		
		// Debug.Log (SceneManager.GetActiveScene ().name);

		if (Input.GetKeyDown (KeyCode.Escape) && SceneManager.GetActiveScene().name == "Main") {
			SceneManager.LoadScene ("Start");
		}

		if (SceneManager.GetActiveScene ().name != "Result") {
			jump = false;
		}

		if (Input.GetKeyDown (KeyCode.T)) {
			timeKeyTitle = 0;
		} else if (Input.GetKey (KeyCode.T)) {
			timeKeyTitle += Time.deltaTime;
			if (timeKeyTitle > 1 && SceneManager.GetActiveScene().name != "Start") {
				SceneManager.LoadScene ("Start");
			}
		}

		if (Input.GetKeyDown (KeyCode.R)) {
			timeKeyResult = 0;
		} else if (Input.GetKey (KeyCode.R)) {
			timeKeyResult += Time.deltaTime;
			if (timeKeyResult > 1 && SceneManager.GetActiveScene().name != "Result") {
				jump = true;
				SceneManager.LoadScene ("Result");
			}
		}

	}
}

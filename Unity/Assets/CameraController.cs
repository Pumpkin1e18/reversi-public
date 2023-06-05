using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour {
	float size = 3.8f;
	private Camera cam;

	// Use this for initialization
	void Start () {
		cam = GetComponent <Camera>();
		cam.orthographicSize = size;
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}

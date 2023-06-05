using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;

public class Chat : MonoBehaviour {

	#if UNITY_ANDROID && !UNITY_EDITOR
	[DllImport("REVERSI", EntryPoint="communication")]
	private static extern IntPtr CallNativeAndroid(IntPtr hoge);
	#else
	[DllImport("REVERSI", EntryPoint="communication")]
	private static extern IntPtr CallNativeWindows(IntPtr hoge);
	#endif

	public string SendCommand(string str1){
		int size = str1.Length;
		for(int i = size;i < 8*8+2;i++)str1 += "*";
		IntPtr ptr1 = Marshal.StringToHGlobalAnsi(str1);
		#if UNITY_ANDROID && !UNITY_EDITOR  
		IntPtr ptr2 = CallNativeAndroid(ptr1);
		#else
		IntPtr ptr2 = CallNativeWindows(ptr1);
		#endif

		string str2 = Marshal.PtrToStringAnsi(ptr2);
		// print (str2);
		// Debug.Log (str2 + "end");

		Marshal.FreeHGlobal(ptr1);
		return str2;
	}

	void Start () {
		int role = StartButton.role;
		SendCommand ("init"+role.ToString());
	}
}

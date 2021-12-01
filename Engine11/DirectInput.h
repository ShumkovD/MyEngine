#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include<dinput.h>


class DirectInput
{
private:
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;
	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8 DInput;
public:
	bool DInputInitialize(HINSTANCE hInstance, HWND hwnd);
	void DetectInput(double time,  HWND hwnd);
	void SceneInput(HWND hwnd, DIMOUSESTATE currentMouseState, BYTE *keyboardState, double time);

	void DInputRelease();
};
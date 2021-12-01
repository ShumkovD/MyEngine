#include"DirectInput.h"

bool DirectInput::DInputInitialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT hr;
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DInput, NULL);
	if (FAILED(hr))
	{
		OutputDebugStringA("\nFailed to Create InputDevice\n\n");
		return false;
	}
	hr = DInput->CreateDevice(GUID_SysKeyboard,
		&DIKeyboard,
		NULL);

	if (FAILED(hr))
	{
		OutputDebugStringA("\nFailed to Create Keyboard\n\n");
		return false;
	}

	hr = DInput->CreateDevice(GUID_SysMouse,
		&DIMouse,
		NULL);

	if (FAILED(hr))
	{
		OutputDebugStringA("\nFailed to Create Mouse\n\n");
		return false;
	}
	hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = DIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = DIMouse->SetDataFormat(&c_dfDIMouse);
	hr = DIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

	return true;
}
void DirectInput::DetectInput(double time, HWND hwnd)
{
	DIMOUSESTATE currentMouseState;
	BYTE keyboardState[256];
	DIKeyboard->Acquire();
	DIMouse->Acquire();

	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &currentMouseState);
	DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	SceneInput(hwnd, currentMouseState, keyboardState, time);
}

void DirectInput::DInputRelease()
{
	DIKeyboard->Unacquire();
	DIMouse->Unacquire();
	DInput->Release();
}
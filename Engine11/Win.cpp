#include<stdio.h>
#include"MyEngine11.h"
#include"DirectInput.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance, _In_ PWSTR pCmdLine, _In_  INT nCmdShow)
{
	//�E�B���h�E�N���X�@�쐬
	const wchar_t CLASS_NAME[] = L"WindowClass";
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);
	//�E�B���h�E�@�쐬
	HWND hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"EngineD11",
		WS_OVERLAPPEDWINDOW,
		300, 300,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL,
		NULL,
		hInstance,
		nullptr
	);
	if (hwnd == NULL)
	{
		UnregisterClassW(CLASS_NAME, hInstance);
		return -1;
	}

	//�G���W���̏�����
	EngineClass eg;
	DirectInput input;
	if (!eg.EngineInitialize(hwnd))
	{
		UnregisterClassW(CLASS_NAME, hInstance);
		return -2;
	}
	//�p�C�v���C���̏�����
	if (!eg.PipelineInitialize())
	{
		UnregisterClassW(CLASS_NAME, hInstance);
		return -3;
	}
	//�O���t�B�N�X�̏�����
	if (!eg.SceneGraphicsInitialize())
	{
		UnregisterClassW(CLASS_NAME, hInstance);
		return -4;
	}
	//���[���h�̏�����
	if (!eg.SettingWorld())
	{
		UnregisterClassW(CLASS_NAME, hInstance);
		return -5;
	}
	if (!input.DInputInitialize(hInstance, hwnd))
	{
		UnregisterClassW(CLASS_NAME, hInstance);
		return -6;
	}


	//�V�[���̏�����
	eg.InitializeScene();
	//�E�B���h�E������
	ShowWindow(hwnd, nCmdShow);
	//���b�Z�[�W���[�v
	MSG msg = { };
	float time;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
		{
			UnregisterClassW(CLASS_NAME, hInstance);
			DestroyWindow(hwnd);
			break;
		}
		if (GetActiveWindow() == hwnd)
		{
			time = eg.Timer();
			//�V�[���̏�����
			eg.SettingWorld();
			//���͂̓ǂݍ���
			input.DetectInput(time, hwnd);
			//�G���W������
			eg.UpdateScene(time);
			//�G���W�`��
			eg.Render();
		}
	}
	CoUninitialize();
	input.DInputRelease();
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
		break;
	}
}
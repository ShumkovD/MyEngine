#include<Windows.h>
#include<stdio.h>

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800
#define MULTISAMPLE_COUNT 2

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance, _In_ PWSTR pCmdLine, _In_  INT nCmdShow)
{
	//�G���[�m�F
	HINSTANCE errorCheck;
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
	ShowWindow(hwnd, nCmdShow);
	//�G���W���̏�����
	
	//���b�Z�[�W���[�v
	MSG msg = { };
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
		//�G���W������

		//�G���W�`��

	}
	
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
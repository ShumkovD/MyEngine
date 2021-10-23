#include<stdio.h>
#include"MyEngine11.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance, _In_ PWSTR pCmdLine, _In_  INT nCmdShow)
{
	//ウィンドウクラス　作成
	const wchar_t CLASS_NAME[] = L"WindowClass";
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);
	//ウィンドウ　作成
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

	//エンジンの初期化
	EngineClass eg;
	if (!eg.EngineInitialize(hwnd))
	{
		UnregisterClassW(CLASS_NAME, hInstance);
		return -2;
	}
	//パイプラインの初期化
	if (!eg.PipelineInitialize())
	{
		UnregisterClassW(CLASS_NAME, hInstance);
		return -3;
	}
	//グラフィクスの初期化
	if (!eg.SceneGraphicsInitialize())
	{
		UnregisterClassW(CLASS_NAME, hInstance);
		return -4;
	}
	//ワールドの初期化
	if (!eg.SettingWorld())
	{
		UnregisterClassW(CLASS_NAME, hInstance);
		return -5;
	}
	//ウィンドウを示す
	ShowWindow(hwnd, nCmdShow);
	//メッセージループ
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
		//エンジン処理
		eg.UpdateScene();
		//エンジ描画
		eg.Render();
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
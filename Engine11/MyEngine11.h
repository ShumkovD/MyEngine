#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "atls.lib")
#include<Windows.h>
#include<d3d11.h>
#include<d3dcompiler.h>
#include"../SDK/Include/DDSTextureLoader.h"
#include<wrl/client.h>

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800
#define MULTISAMPLE_COUNT 2

using namespace Microsoft;
using namespace WRL;

class EngineClass
{
private:
	ComPtr<ID3D11Device> dev;
	ComPtr<ID3D11DeviceContext> devcon;
	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<ID3D11RenderTargetView> rtv;

	D3D11_VIEWPORT vp;



public:
	bool EngineInitialize(HWND hwnd);
private:
	bool CreateDeviceAndSwapChain(HWND hwnd);
	bool CreateRenderTargetView();
public:
	void InitScene();
	void Render();
};

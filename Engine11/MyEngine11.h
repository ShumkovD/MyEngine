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

struct Vertex
{
	float X, Y, Z, r, g, b, a;
};


class EngineClass
{
private:
	ComPtr<ID3D11Device> dev;
	ComPtr<ID3D11DeviceContext> devcon;
	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<ID3D11RenderTargetView> rtv;

	ComPtr<ID3DBlob> vertexBlob;
	ComPtr<ID3DBlob> pixelBlob;
	
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;

	ComPtr<ID3D11Buffer>	indexBuffer;
	ComPtr<ID3D11Buffer>	vertexBuffer;

	ComPtr<ID3D11Texture2D>			depthBuffer;
	ComPtr<ID3D11DepthStencilView>  depthStencil;

	D3D11_VIEWPORT vp;

	ComPtr<ID3D11InputLayout> inputLayout;

public:
	bool EngineInitialize(HWND hwnd);
	bool PipelineInitialize();
	bool SceneGraphicsInitialize();
private:
	bool CreateDepthStencilView();
	bool CreateDeviceAndSwapChain(HWND hwnd);
	bool CreateRenderTargetView();

public:
	void InitScene();
	void Render();
};

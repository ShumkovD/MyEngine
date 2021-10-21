#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "atls.lib")
#include<Windows.h>
#include<d3d11.h>
#include<d3dcompiler.h>
#include<wrl/client.h>
#include<DirectXMath.h>
#include"../SDK/Include/DDSTextureLoader.h"

#define WINDOW_HEIGHT 1024
#define WINDOW_WIDTH 1920
#define MULTISAMPLE_COUNT 4

// D3D11_FILL_WIREFRAME | D3D11_FILL_SOLID
#define RASTERIZER_FILL D3D11_FILL_SOLID

using namespace Microsoft;
using namespace WRL;
using namespace DirectX;

struct Vertex
{
	float X, Y, Z, U, V;
};

struct cbPerObjectBuffer
{
	XMMATRIX WVP;
};

class EngineClass
{
private:
	
	ComPtr<ID3D11Device> dev;
	ComPtr<ID3D11DeviceContext> devcon;
	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<ID3D11RenderTargetView> rtv;

	D3D11_VIEWPORT vp;

	ComPtr<ID3DBlob> vertexBlob;
	ComPtr<ID3DBlob> pixelBlob;
	
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;

	ComPtr<ID3D11Buffer>	indexBuffer;
	ComPtr<ID3D11Buffer>	vertexBuffer;

	ComPtr<ID3D11InputLayout> inputLayout;
	
	ComPtr<ID3D11Texture2D>			depthBuffer;
	ComPtr<ID3D11DepthStencilView>  depthStencil;

	ComPtr<ID3D11Buffer>	constantBuffer;

	ComPtr<ID3D11RasterizerState> wireframeState;

	ComPtr<ID3D11ShaderResourceView> resourceTexture;
	ComPtr<ID3D11SamplerState> texSamplerState;

public:
	bool EngineInitialize(HWND hwnd);
	bool PipelineInitialize();
	bool SceneGraphicsInitialize();
	bool SettingWorld();
private:
	bool CreateDepthStencilView();
	bool CreateDeviceAndSwapChain(HWND hwnd);
	bool CreateRenderTargetView();
	bool LoadingTexture();
public:
	void UpdateScene();
	void Render();
public:
	//行列
	XMMATRIX WVP;
	XMMATRIX    world;
	XMMATRIX camView;
	XMMATRIX camProjection;
	//ベクトル
	XMVECTOR camPos;
	XMVECTOR camTarget;
	XMVECTOR camUp;
	//Constant buffer
	cbPerObjectBuffer cbPerObject;

	//Object Matrix
	XMMATRIX cube1World;
	XMMATRIX cube2World;
	//Update Matrixes
	XMMATRIX translation;
	XMMATRIX rotation;
	XMMATRIX scaling;
	float rot = 0.005f;

};

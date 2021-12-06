#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "atls.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include<Windows.h>
#include<d3d11.h>
#include<d3dcompiler.h>
#include<wrl/client.h>
#include<DirectXMath.h>
#include<utility>
#include<DDSTextureLoader.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <string>

#include"DirectInput.h"
#include"DMesh.h"

#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920
#define MULTISAMPLE_COUNT 1

// D3D11_FILL_WIREFRAME | D3D11_FILL_SOLID
#define RASTERIZER_FILL D3D11_FILL_SOLID

using namespace Microsoft;
using namespace WRL;
using namespace DirectX;

//オブジェクトごとのバッファ
struct cbPerObjectBuffer
{
	XMMATRIX WVP;
	XMMATRIX world;
	XMFLOAT4 difColor;
	bool	 hasTexture;
	XMFLOAT3 textureScale;
};

//ライトニング構造体
struct Light
{
	XMFLOAT3 dir;
	float pad;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};

//フレームごとのバッファ
struct cbPerFrameBuffer
{
	Light light;
};




class EngineClass
{
private:
	
	ComPtr<ID3D11Device1> dev;
	ComPtr<ID3D11DeviceContext1> devcon;

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
	ComPtr<ID3D11Buffer>	constantPerFrameBuffer;

	ComPtr<ID3D11RasterizerState1> wireframeState;
	
	ComPtr<ID3D11BlendState1> transparent;
	
	ComPtr<ID3D11RasterizerState1> CCWCull;
	ComPtr<ID3D11RasterizerState1> CWCull;
	ComPtr<ID3D11RasterizerState1> NoCull;

	std::unique_ptr<SpriteBatch>	spriteBatch;
	std::unique_ptr<SpriteFont>		spriteFont;

	ComPtr<ID3D11SamplerState> texSamplerState;

	DirectInput input;

public:
	bool EngineInitialize(HWND hwnd);
	bool PipelineInitialize();
	bool SceneGraphicsInitialize();
	bool SettingWorld();
private:
	bool CreateSampler();
	bool CreateDepthStencilView();
	bool CreateDeviceAndSwapChain(HWND hwnd);
	bool CreateRenderTargetView();
	bool LoadingTexture(const wchar_t* resourceAddress, ID3D11ShaderResourceView **resourceTexture);
	bool TCreatingBlending();	//T ransparency
	void DrawMyText(const char* text);
	void StartTimer();
	double GetTime();
	double GetFrameTime();
public:
	double Timer();
	void InitializeScene();
	void UpdateScene(double time);
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
	//ライトニングの初期化
	Light light;
	//コンスタントバッファ
	cbPerObjectBuffer cbPerObject;	
	cbPerFrameBuffer cbPerFrame;

public:
	double countsPerSecond = 0.0;
	__int64 CounterStart = 0;

	int frameCount = 0;
	int fps = 0;

	__int64 frameTimeOld = 0;
	double frameTime;

};

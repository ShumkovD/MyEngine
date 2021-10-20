#include"MyEngine11.h"


bool EngineClass::EngineInitialize(HWND hwnd)
{
	if (!CreateDeviceAndSwapChain(hwnd))
		return false;
	if (!CreateRenderTargetView())
		return false;

	devcon->OMSetRenderTargets(1, rtv.GetAddressOf(), NULL);

	return true;
}





bool EngineClass::CreateDeviceAndSwapChain(HWND hwnd)
{
	HRESULT hr;
	// // スワップチェーンとデバイス　作成
//バックバッファ
	DXGI_MODE_DESC backBuffer;
	ZeroMemory(&backBuffer, sizeof(DXGI_MODE_DESC));
	backBuffer.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	backBuffer.Width = WINDOW_WIDTH;
	backBuffer.Height = WINDOW_HEIGHT;
	backBuffer.RefreshRate.Denominator = 1;
	backBuffer.RefreshRate.Numerator = 60;
	backBuffer.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	backBuffer.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	//スワップチェーン
	DXGI_SWAP_CHAIN_DESC swapchainDesc;
	ZeroMemory(&swapchainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapchainDesc.BufferCount = 1;
	swapchainDesc.BufferDesc = backBuffer;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.OutputWindow = hwnd;
	swapchainDesc.SampleDesc.Count = MULTISAMPLE_COUNT;
	swapchainDesc.SampleDesc.Quality = 0;
	swapchainDesc.Windowed = true;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	hr = D3D11CreateDeviceAndSwapChain(0,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapchainDesc,
		swapChain.GetAddressOf(),
		dev.GetAddressOf(),
		NULL,
		devcon.GetAddressOf());
	if (FAILED(hr))
	{
		OutputDebugStringA("\nFailed to Create Device And SwapChain\n\n");
		return false;
	}

	return true;
}

bool EngineClass::CreateRenderTargetView()
{
	HRESULT hr;
	ID3D11Texture2D* pBackBuffer;
	//
	hr = swapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (LPVOID*)(&pBackBuffer));
	if (FAILED(hr))
	{
		OutputDebugStringA("\nFailed to get buffet from swap chain\n\n");
		return false;
	}
	hr = dev->CreateRenderTargetView(pBackBuffer, nullptr, rtv.GetAddressOf());
	if (FAILED(hr))
	{
		OutputDebugStringA("\nFailed to Create RenderTarget\n\n");
		return false;
	}
	pBackBuffer->Release();
	return true;
}
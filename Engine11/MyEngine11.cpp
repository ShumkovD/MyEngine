#include"MyEngine11.h"

bool LoadShader(LPCWSTR fileDirectory, LPCSTR startPoint, LPCSTR profile, ComPtr<ID3DBlob> *shaderBlob);

bool EngineClass::EngineInitialize(HWND hwnd)
{
	if (!CreateDeviceAndSwapChain(hwnd))
		return false;
	if (!CreateRenderTargetView())
		return false;
	if (!CreateDepthStencilView())
		return false;

	devcon->OMSetRenderTargets(1, rtv.GetAddressOf(), depthStencil.Get());

	return true;
}

bool EngineClass::PipelineInitialize()
{
	HRESULT hr;

	//シェーダー　作成--------------------
	if (!LoadShader(L"VertexShader.hlsl", "main", "vs_5_0", &vertexBlob))
	{
		OutputDebugStringA("\nFailed to Load Vertex Shader\n\n");
		return false;
	}
	if (!LoadShader(L"PixelShader.hlsl", "main", "ps_5_0", &pixelBlob))
	{
		OutputDebugStringA("\nFailed to Load Pixel Shader\n\n");
		return false;
	}
	//-----------------
	hr = dev->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL, vertexShader.GetAddressOf());
	if (FAILED(hr))
	{
		OutputDebugStringA("\nFailed to Create Vertex Shader\n\n");
		return false;
	}
	hr = dev->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), NULL, pixelShader.GetAddressOf());
	if (FAILED(hr))
	{
		OutputDebugStringA("\nFailed to Create Pixel Shader\n\n");
		return false;
	}
	//------------------
	devcon->VSSetShader(vertexShader.Get(), 0, 0);
	devcon->PSSetShader(pixelShader.Get(), 0, 0);
	//------------------
	//Input Layout 作成

	D3D11_INPUT_ELEMENT_DESC ied[]
	{
		{"POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR" , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	hr = dev->CreateInputLayout(ied, ARRAYSIZE(ied), vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), inputLayout.GetAddressOf());
	if (FAILED(hr))
	{
		OutputDebugStringA("\nFailed to Create Input Layout\n\n");
		return false;
	}
	devcon->IASetInputLayout(inputLayout.Get());
	////Setting Constant Buffer/s
	D3D11_BUFFER_DESC cbDes;
	ZeroMemory(&cbDes, sizeof(D3D11_BUFFER_DESC));
	cbDes.ByteWidth = sizeof(cbPerObjectBuffer);
	cbDes.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = dev->CreateBuffer(&cbDes, NULL, constantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		OutputDebugStringA("\nFailed to Create Constant Buffer\n\n");
		return false;
	}

	return true;
}

bool EngineClass::SceneGraphicsInitialize()
{
	HRESULT hr;
	Vertex myVertex[] =
	{
		{-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
		{-1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f},
		{+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
		{+1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
		{-1.0f, -1.0f, +1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
		{-1.0f, +1.0f, +1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{+1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
		{+1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
	};

	DWORD indices[]
	{
	// front face
    0, 1, 2,
    0, 2, 3,

    // back face
    4, 6, 5,
    4, 7, 6,

    // left face
    4, 5, 1,
    4, 1, 0,

    // right face
    3, 2, 6,
    3, 6, 7,

    // top face
    1, 5, 6,
    1, 6, 2,

    // bottom face
    4, 0, 3, 
    4, 3, 7
	};

	////Vertex Buffer-----------------
	D3D11_BUFFER_DESC vertexDes;
	ZeroMemory(&vertexDes, sizeof(D3D11_BUFFER_DESC));
	vertexDes.ByteWidth = sizeof(Vertex) * ARRAYSIZE(myVertex);
	vertexDes.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//Subresource
	D3D11_SUBRESOURCE_DATA vertexResource;
	ZeroMemory(&vertexResource, sizeof(D3D11_SUBRESOURCE_DATA));
	vertexResource.pSysMem = myVertex;
	//Creation
	hr = dev->CreateBuffer(&vertexDes, &vertexResource, vertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		OutputDebugStringA("\nFailed to Create Vertex Buffer\n\n");
		return false;
	}
	//Setting
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	////Index Buffer-----------------
	D3D11_BUFFER_DESC indexDes;
	ZeroMemory(&indexDes, sizeof(D3D11_BUFFER_DESC));
	indexDes.ByteWidth = sizeof(DWORD) * ARRAYSIZE(indices);
	indexDes.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//Subresource
	D3D11_SUBRESOURCE_DATA pixelResource;
	ZeroMemory(&pixelResource, sizeof(D3D11_SUBRESOURCE_DATA));
	pixelResource.pSysMem = indices;
	//Creation
	dev->CreateBuffer(&indexDes, &pixelResource, indexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		OutputDebugStringA("\nFailed to Create Index Buffer\n\n");
		return false;
	}//Setting
	devcon->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	////ViewPort-----------------------------
	ZeroMemory(&vp, sizeof(vp));
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	devcon->RSSetViewports(1, &vp);
	//-------------------------------------
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
		D3D11_CREATE_DEVICE_DEBUG,
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

bool LoadShader(LPCWSTR fileDirectory, LPCSTR startPoint,LPCSTR profile, ComPtr<ID3DBlob> *shaderBlob)
{
	HRESULT hr;
	ComPtr<ID3DBlob> outBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	hr = D3DCompileFromFile(fileDirectory, NULL, NULL, startPoint, profile, NULL, NULL, outBlob.GetAddressOf(), errorBlob.GetAddressOf());
	if (FAILED(hr))
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		return false;
	}
	*shaderBlob = outBlob;
	return true;
}

bool EngineClass::CreateDepthStencilView()
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.Width = WINDOW_WIDTH;
	depthBufferDesc.Height = WINDOW_HEIGHT;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.SampleDesc.Count = MULTISAMPLE_COUNT;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hr = dev->CreateTexture2D(&depthBufferDesc, NULL, depthBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		OutputDebugStringA("\nFailed to create Depth Stencil buffer\n\n");
		return false;
	}
	hr = dev->CreateDepthStencilView(depthBuffer.Get(), NULL, depthStencil.GetAddressOf());
	if (FAILED(hr))
	{
		OutputDebugStringA("\nFailed to create Depth Stencil View\n\n");
		return false;
	}

	return true;
}

bool EngineClass::SettingWorld()
{
	camPos = XMVectorSet(0.0f,3.0f,-8.0f, 0.0f);
	camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	world = XMMatrixIdentity();
	camView = XMMatrixLookAtLH(camPos, camTarget, camUp);
	camProjection = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 1000.0f);
	//Setting Default World Setting
	WVP = world * camView * camProjection;
	cbPerObject.WVP = XMMatrixTranspose(WVP);
	devcon->UpdateSubresource(constantBuffer.Get(), 0, NULL, &cbPerObject, 0, 0);
	devcon->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	return true;
}
#include"MyEngine11.h"

DMesh obj1;

void EngineClass::InitializeScene()
{
	if (!obj1.LoadObjModel(L"../Resources/model/Only_Spider_with_Animations_Export.obj", true, false, dev))
		return;

	//カメラ
	camPos = XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);
	camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	camView = XMMatrixLookAtLH(camPos, camTarget, camUp);
	camProjection = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 1000.0f);
	//ライトニングの初期化
	light.dir = XMFLOAT3(0.0f, 1.0f, 0.0f);
	light.ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

float scale = 1.0f;
float camPosIncrement = 1.0f;
float rotation = 0.0f;

void DirectInput::SceneInput(HWND hwnd, DIMOUSESTATE currentMouseState, BYTE *keyboardState, double time)
{
	if (keyboardState[DIK_ESCAPE] & 0x80)
		PostMessage(hwnd, WM_DESTROY, 0, 0);
	if (keyboardState[DIK_RIGHT] & 0x80)
		scale += 1.0f*time;
	if (keyboardState[DIK_LEFT] & 0x80)
		scale -= 1.0f*time;
	if (keyboardState[DIK_UP] & 0x80)
		camPosIncrement += 3.0f * time;
	if (keyboardState[DIK_DOWN] & 0x80)
		camPosIncrement -= 3.0f * time;
	if (keyboardState[DIK_E] & 0x80)
		rotation += 1.0f * time;
	if (keyboardState[DIK_Q] & 0x80)
		rotation -= 1.0f * time;

	if (scale < 0.01f)
		scale = 0.01f;

	return;
}


void EngineClass::UpdateScene(double time)
{
	camTarget = XMVectorSet(0.0f, 0.0f, camPosIncrement, 0.0f);
	camPos = XMVectorSet(0.0f, 3.0f, -8.0f + camPosIncrement, 0.0f);
	camView = XMMatrixLookAtLH(camPos, camTarget, camUp);
	obj1.meshWorld = XMMatrixIdentity();
	XMMATRIX Scale = XMMatrixScaling(scale, scale, scale);
	XMMATRIX Rotation = XMMatrixRotationY(rotation);
	XMMATRIX Translation = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	obj1.meshWorld = Scale * Rotation * Translation;
}

void EngineClass::Render()
{
	//シーンリセット
	SettingWorld();

	//背景
	devcon->VSSetShader(vertexShader.Get(), 0, 0);
	devcon->PSSetShader(pixelShader.Get(), 0, 0);
	float color[] = { 0.1f,0.1f,0.1f,1.0f };
	//
	float blendFactor[] = { 0.9f,0.9f,0.9f,1.0f };
	devcon->OMSetRenderTargets(1, rtv.GetAddressOf(), depthStencil.Get());
	devcon->ClearRenderTargetView(rtv.Get(), color);
	devcon->ClearDepthStencilView(depthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//ライトニング
	cbPerFrame.light = light;
	devcon->UpdateSubresource(constantPerFrameBuffer.Get(), 0, NULL, &cbPerFrame, 0, 0);
	devcon->PSSetConstantBuffers1(0, 1, constantPerFrameBuffer.GetAddressOf(), 0, 0);
	//不透明なメッシュを読み込む
	devcon->OMSetBlendState(0, 0, 0xffffffff);
	for (long long int i = 0; i < obj1.meshSubsets; ++i)
	{
		//Set the grounds index buffer
		devcon->IASetIndexBuffer(obj1.meshIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		//Set the grounds vertex buffer
		devcon->IASetVertexBuffers(0, 1, obj1.meshVertexBuffer.GetAddressOf(), &obj1.stride, &obj1.offset);

		//Set the WVP matrix and send it to the constant buffer in effect file
		WVP = obj1.meshWorld * camView * camProjection;
		cbPerObject.WVP = XMMatrixTranspose(WVP);
		cbPerObject.world = XMMatrixTranspose(obj1.meshWorld);
		cbPerObject.difColor = obj1.material[obj1.meshSubsetTexture[i]].difColor;
		cbPerObject.textureScale = obj1.material[obj1.meshSubsetTexture[i]].scale;
		cbPerObject.hasTexture = obj1.material[obj1.meshSubsetTexture[i]].hasTexture;
		devcon->UpdateSubresource(constantBuffer.Get(), 0, NULL, &cbPerObject, 0, 0);
		devcon->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
		devcon->PSSetConstantBuffers1(1, 1, constantBuffer.GetAddressOf(), 0, 0);
		if (obj1.material[obj1.meshSubsetTexture[i]].hasTexture)
			devcon->PSSetShaderResources(0, 1, obj1.meshSRV[obj1.material[obj1.meshSubsetTexture[i]].texArrayIndex].GetAddressOf());
		devcon->PSSetSamplers(0, 1, texSamplerState.GetAddressOf());

		devcon->RSSetState(NoCull.Get());
		int indexStart = obj1.meshSubsetIndexStart[i];
		int indexDrawAmount = obj1.meshSubsetIndexStart[i + 1] - obj1.meshSubsetIndexStart[i];
		if(!obj1.material[obj1.meshSubsetTexture[i]].transparent)
			devcon->DrawIndexed(indexDrawAmount, indexStart, 0);
	}
	//透明なメッシュを読み込む
	devcon->OMSetBlendState(transparent.Get(), 0, 0xffffffff);

	for (long long int i = 0; i < obj1.meshSubsets; ++i)
	{
		//Set the grounds index buffer
		devcon->IASetIndexBuffer(obj1.meshIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		//Set the grounds vertex buffer
		devcon->IASetVertexBuffers(0, 1, obj1.meshVertexBuffer.GetAddressOf(), &obj1.stride, &obj1.offset);

		//Set the WVP matrix and send it to the constant buffer in effect file
		WVP = obj1.meshWorld * camView * camProjection;
		cbPerObject.WVP = XMMatrixTranspose(WVP);
		cbPerObject.world = XMMatrixTranspose(obj1.meshWorld);
		cbPerObject.textureScale = obj1.material[obj1.meshSubsetTexture[i]].scale;
		cbPerObject.difColor = obj1.material[obj1.meshSubsetTexture[i]].difColor;
		cbPerObject.hasTexture = obj1.material[obj1.meshSubsetTexture[i]].hasTexture;
		devcon->UpdateSubresource(constantBuffer.Get(), 0, NULL, &cbPerObject, 0, 0);
		devcon->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
		devcon->PSSetConstantBuffers1(1, 1, constantBuffer.GetAddressOf(), 0, 0);
		if (obj1.material[obj1.meshSubsetTexture[i]].hasTexture)
			devcon->PSSetShaderResources(0, 1, obj1.meshSRV[obj1.material[obj1.meshSubsetTexture[i]].texArrayIndex].GetAddressOf());
		devcon->PSSetSamplers(0, 1, texSamplerState.GetAddressOf());

		devcon->RSSetState(NoCull.Get());
		int indexStart = obj1.meshSubsetIndexStart[i];
		int indexDrawAmount = obj1.meshSubsetIndexStart[i + 1] - obj1.meshSubsetIndexStart[i];
		if (obj1.material[obj1.meshSubsetTexture[i]].transparent)
			devcon->DrawIndexed(indexDrawAmount, indexStart, 0);
	}

	//2Dレンダー
	spriteBatch->Begin();
	DrawMyText(std::to_string(fps).c_str());
	spriteBatch->End();


	swapChain->Present(1, 0);

}

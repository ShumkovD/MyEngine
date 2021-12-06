#include"MyEngine11.h"

DMesh obj[4];

void EngineClass::InitializeScene()
{
	if (!obj[0].LoadObjModel(L"../Resources/model/plant.obj", true, false, dev))
		return;
	if (!obj[1].LoadObjModel(L"../Resources/model/girl OBJ.obj", true, false, dev))
		return;
	if (!obj[2].LoadObjModel(L"../Resources/model/spider.obj", true, false, dev))
		return;
	if (!obj[3].LoadObjModel(L"../Resources/model/tree.obj", true, false, dev))
		return;
	//カメラ
	camPos = XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);
	camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	camView = XMMatrixLookAtLH(camPos, camTarget, camUp);
	camProjection = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 1000.0f);
	//ライトニングの初期化
	light.dir = XMFLOAT3(0.0f, 1.0f, 0.0f);
	light.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

float scale = 0.0f;
float camPosIncrement = 0.0f;
float rotation = 0.0f;
float rot = 0.0005f;

void DirectInput::SceneInput(HWND hwnd, DIMOUSESTATE currentMouseState, BYTE *keyboardState, double time)
{
	if (keyboardState[DIK_ESCAPE] & 0x80)
		PostMessage(hwnd, WM_DESTROY, 0, 0);
	if (keyboardState[DIK_RIGHT] & 0x80)
		scale += 1.0f*time;

	if (keyboardState[DIK_LEFT] & 0x80)
		scale -= 1.0f*time;
	if (keyboardState[DIK_W] & 0x80)
		camPosIncrement += 3.0f * time;
	if (keyboardState[DIK_S] & 0x80)
		camPosIncrement -= 3.0f * time;
	if (keyboardState[DIK_E] & 0x80)
		rotation += 1.0f * time;
	if (keyboardState[DIK_Q] & 0x80)
		rotation -= 1.0f * time;
	if (keyboardState[DIK_A] & 0x80)
		rot += 1.0f * time;
	if (keyboardState[DIK_D] & 0x80)
		rot -= 1.0f * time;

	if (scale < 0.01f)
		scale = 0.01f;

	if (rot > 6.28f)
		rot = 0.0f;

	return;
}


void EngineClass::UpdateScene(double time)
{
	
	camTarget = XMVectorSet(0.0f, 0.0f, 5.0 + camPosIncrement, 0.0f);
	camPos = XMVectorSet(0.0f, 1.0f, camPosIncrement, 0.0f);
	camView = XMMatrixLookAtLH(camPos, camTarget, camUp);
	//
	XMVECTOR axis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//植物
	obj[0].meshWorld = XMMatrixIdentity();
	XMMATRIX Scale = XMMatrixScaling(1.0f + scale, 1.0f + scale, 1.0f + scale);
	XMMATRIX WorldRotation = XMMatrixRotationAxis(axis, rot);
	XMMATRIX Rotation = XMMatrixRotationY(rotation);
	XMMATRIX Translation = XMMatrixTranslation(0.0f, 0.0f, 4.0f);
	obj[0].meshWorld = Scale * Rotation * Translation * WorldRotation;
	//キャラ
	obj[1].meshWorld = XMMatrixIdentity();
	Scale = XMMatrixScaling(1.0f + scale, 1.0f + scale, 1.0f + scale);
	WorldRotation = XMMatrixRotationAxis(axis, rot);
	Rotation = XMMatrixRotationY(rotation);
	Translation = XMMatrixTranslation(0.0f, 0.0f, -4.0f);
	obj[1].meshWorld = Scale * Rotation * Translation * WorldRotation;
	//クモ
	obj[2].meshWorld = XMMatrixIdentity();
	Scale = XMMatrixScaling(0.01f + scale*0.01f, 0.01f + scale*0.01f, 0.01f + scale*0.01f);
	WorldRotation = XMMatrixRotationAxis(axis, rot);
	Rotation = XMMatrixRotationY(rotation);
	Translation = XMMatrixTranslation(-4.0f, 0.0f, 0.0f);
	obj[2].meshWorld = Scale * Rotation * Translation * WorldRotation;
	//木
	obj[3].meshWorld = XMMatrixIdentity();
	Scale = XMMatrixScaling(0.05f + scale, 0.05f + scale, 0.05f + scale);
	WorldRotation = XMMatrixRotationAxis(axis, rot);
	Rotation = XMMatrixRotationY(rotation);
	Translation = XMMatrixTranslation(4.0f, 0.0f, 0.0f);
	obj[3].meshWorld = Scale * Rotation * Translation * WorldRotation;
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
	for (int o = 0; o < 4; o++)
	{
		for (long long int i = 0; i < obj[o].meshSubsets; ++i)
		{
			//Set the grounds index buffer
			devcon->IASetIndexBuffer(obj[o].meshIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			//Set the grounds vertex buffer
			devcon->IASetVertexBuffers(0, 1, obj[o].meshVertexBuffer.GetAddressOf(), &obj[o].stride, &obj[o].offset);

			//Set the WVP matrix and send it to the constant buffer in effect file
			WVP = obj[o].meshWorld * camView * camProjection;
			cbPerObject.WVP = XMMatrixTranspose(WVP);
			cbPerObject.world = XMMatrixTranspose(obj[o].meshWorld);
			cbPerObject.difColor = obj[o].material[obj[o].meshSubsetTexture[i]].difColor;
			cbPerObject.textureScale = obj[o].material[obj[o].meshSubsetTexture[i]].scale;
			cbPerObject.hasTexture = obj[o].material[obj[o].meshSubsetTexture[i]].hasTexture;
			devcon->UpdateSubresource(constantBuffer.Get(), 0, NULL, &cbPerObject, 0, 0);
			devcon->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
			devcon->PSSetConstantBuffers1(1, 1, constantBuffer.GetAddressOf(), 0, 0);
			if (obj[o].material[obj[o].meshSubsetTexture[i]].hasTexture)
				devcon->PSSetShaderResources(0, 1, obj[o].meshSRV[obj[o].material[obj[o].meshSubsetTexture[i]].texArrayIndex].GetAddressOf());
			devcon->PSSetSamplers(0, 1, texSamplerState.GetAddressOf());

			devcon->RSSetState(NoCull.Get());
			int indexStart = obj[o].meshSubsetIndexStart[i];
			int indexDrawAmount = obj[o].meshSubsetIndexStart[i + 1] - obj[o].meshSubsetIndexStart[i];
			if (!obj[o].material[obj[o].meshSubsetTexture[i]].transparent)
				devcon->DrawIndexed(indexDrawAmount, indexStart, 0);
		}
	}

	//透明なメッシュを読み込む
	devcon->OMSetBlendState(transparent.Get(), 0, 0xffffffff);
	for (int o = 0; o < 4; o++)
	{
		for (long long int i = 0; i < obj[o].meshSubsets; ++i)
		{
			//Set the grounds index buffer
			devcon->IASetIndexBuffer(obj[o].meshIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			//Set the grounds vertex buffer
			devcon->IASetVertexBuffers(0, 1, obj[o].meshVertexBuffer.GetAddressOf(), &obj[o].stride, &obj[o].offset);

			//Set the WVP matrix and send it to the constant buffer in effect file
			WVP = obj[o].meshWorld * camView * camProjection;
			cbPerObject.WVP = XMMatrixTranspose(WVP);
			cbPerObject.world = XMMatrixTranspose(obj[o].meshWorld);
			cbPerObject.difColor = obj[o].material[obj[o].meshSubsetTexture[i]].difColor;
			cbPerObject.textureScale = obj[o].material[obj[o].meshSubsetTexture[i]].scale;
			cbPerObject.hasTexture = obj[o].material[obj[o].meshSubsetTexture[i]].hasTexture;
			devcon->UpdateSubresource(constantBuffer.Get(), 0, NULL, &cbPerObject, 0, 0);
			devcon->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
			devcon->PSSetConstantBuffers1(1, 1, constantBuffer.GetAddressOf(), 0, 0);
			if (obj[o].material[obj[o].meshSubsetTexture[i]].hasTexture)
				devcon->PSSetShaderResources(0, 1, obj[o].meshSRV[obj[o].material[obj[o].meshSubsetTexture[i]].texArrayIndex].GetAddressOf());
			devcon->PSSetSamplers(0, 1, texSamplerState.GetAddressOf());

			devcon->RSSetState(NoCull.Get());
			int indexStart = obj[o].meshSubsetIndexStart[i];
			int indexDrawAmount = obj[o].meshSubsetIndexStart[i + 1] - obj[o].meshSubsetIndexStart[i];
			if (obj[o].material[obj[o].meshSubsetTexture[i]].transparent)
				devcon->DrawIndexed(indexDrawAmount, indexStart, 0);
		}
	}

	//2Dレンダー
	spriteBatch->Begin();
	DrawMyText(std::to_string(fps).c_str());
	spriteBatch->End();


	swapChain->Present(1, 0);

}

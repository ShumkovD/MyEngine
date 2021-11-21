#include"MyEngine11.h"

void EngineClass::InitializeScene()
{
	//ライトニングの初期化
	light.dir = XMFLOAT3(0.0f, 1.0f, 0.0f);
	light.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}


void EngineClass::UpdateScene(double time)
{

	rot += 1.0 * time;
	if (rot > 6.28)
		rot = 0;
	XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	//中心にある立方体	ピクサークリッピングに設定しています。
	cube1World = XMMatrixIdentity();
	translation = XMMatrixTranslation(0.0f, 0.0f, 4.0f);
	rotation = XMMatrixRotationAxis(rotaxis, rot);
	cube1World = translation * rotation;
	//Y軸を回転している	透明に設定しています。
	cube2World = XMMatrixIdentity();
	rotation = XMMatrixRotationAxis(rotaxis, -rot);
	scaling = XMMatrixScaling(1.3f, 1.3f, 1.3f);
	cube2World = scaling * rotation;

	rotaxis = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
	//Z軸を回転している	不透明に設定しています。
	cube3World = XMMatrixIdentity();
	translation = XMMatrixTranslation(0.0f, 4.0f, 0.0f);
	rotation = XMMatrixRotationAxis(rotaxis, rot);
	cube3World = translation * rotation;


}

void EngineClass::Render()
{
	//シーンリセット
	SettingWorld();
	//背景
	float color[] = { 0.1f,0.1f,0.1f,1 };
	//
	float blendFactor[] = { 0.75f,0.75f,0.75f,1.0f };
	devcon->ClearRenderTargetView(rtv.Get(), color);
	//ライトニング
	cbPerFrame.light = light;
	devcon->UpdateSubresource1(constantPerFrameBuffer.Get(), 0, NULL, &cbPerFrame, 0, 0, 0);
	devcon->PSSetConstantBuffers1(0, 1, constantPerFrameBuffer.GetAddressOf(), 0, 0);
	//不透明
	devcon->OMSetBlendState(0, 0, 0xffffffff);

	devcon->ClearDepthStencilView(depthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	

		//Z軸を回転している	不透明に設定しています。
		devcon->RSSetState(0);

		WVP = cube1World * camView * camProjection;
		cbPerObject.WVP = XMMatrixTranspose(WVP);
		cbPerObject.world = XMMatrixTranspose(cube1World);

		devcon->UpdateSubresource1(constantBuffer.Get(), 0, NULL, &cbPerObject, 0, 0, 0);
		devcon->VSSetConstantBuffers1(0, 1, constantBuffer.GetAddressOf(), 0, 0);
		devcon->PSSetShaderResources(0, 1, resourceTexture.GetAddressOf());
		devcon->PSSetSamplers(0, 1, texSamplerState.GetAddressOf());
		devcon->DrawIndexed(36, 0, 0);

		//中心にある立方体	ピクサークリッピングに設定しています。
		
		devcon->RSSetState(NoCull.Get());

		WVP = cube2World * camView * camProjection;
		cbPerObject.WVP = XMMatrixTranspose(WVP);
		cbPerObject.world = XMMatrixTranspose(cube2World);

		devcon->UpdateSubresource(constantBuffer.Get(), 0, NULL, &cbPerObject, 0, 0);
		devcon->VSSetConstantBuffers1(0, 1, constantBuffer.GetAddressOf(),0, 0);
		devcon->PSSetShaderResources(0, 1, resource2Texture.GetAddressOf());
		devcon->PSSetSamplers(0, 1, texSamplerState.GetAddressOf());
		devcon->DrawIndexed(36, 0, 0);


		//Y軸を回転している	透明に設定しています。
		devcon->OMSetBlendState(transparent.Get(), blendFactor, 0xffffffff);

		WVP = cube3World * camView * camProjection;
		cbPerObject.WVP = XMMatrixTranspose(WVP);
		cbPerObject.world = XMMatrixTranspose(cube3World);

		devcon->UpdateSubresource(constantBuffer.Get(), 0, NULL, &cbPerObject, 0, 0);
		devcon->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
		devcon->PSSetShaderResources(0, 1, resourceTexture.GetAddressOf());
		devcon->PSSetSamplers(0, 1, texSamplerState.GetAddressOf());
		//裏面を描画してから
		devcon->RSSetState(CCWCull.Get());
		devcon->DrawIndexed(36, 0, 0);
		//表面を描画します
		devcon->RSSetState(CWCull.Get());
		devcon->DrawIndexed(36, 0, 0);

		//2Dレンダー
	spriteBatch->Begin();
	DrawMyText(std::to_string(fps).c_str());
	spriteBatch->End();


	swapChain->Present(1, 0);

}

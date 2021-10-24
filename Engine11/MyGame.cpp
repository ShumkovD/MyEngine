#include"MyEngine11.h"


void EngineClass::UpdateScene()
{
	rot += 0.00005f;
	if (rot > 6.28)
		rot = 0;

	XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	cube1World = XMMatrixIdentity();
	translation = XMMatrixTranslation(0.0f, 0.0f, 4.0f);
	rotation = XMMatrixRotationAxis(rotaxis, rot);
	cube1World = translation * rotation;

	cube2World = XMMatrixIdentity();
	rotation = XMMatrixRotationAxis(rotaxis, -rot);
	scaling = XMMatrixScaling(1.3f, 1.3f, 1.3f);
	cube2World = scaling * rotation;

}

void EngineClass::Render()
{
	float color[] = { 0,0,0,1 };
	float blendFactor[] = { 1.0f,1.0f,1.0f,1.0f };
	devcon->ClearRenderTargetView(rtv.Get(), color);
	devcon->ClearDepthStencilView(depthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	devcon->RSSetState(NULL);
	//Setting object withot alpha channel
	devcon->RSSetState(NoCull.Get());

	WVP = cube1World * camView * camProjection;
	cbPerObject.WVP = XMMatrixTranspose(WVP);
	devcon->UpdateSubresource(constantBuffer.Get(), 0, NULL, &cbPerObject, 0, 0);

	devcon->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	devcon->PSSetShaderResources(0, 1, resourceTexture.GetAddressOf());
	devcon->PSSetSamplers(0, 1, texSamplerState.GetAddressOf());
	devcon->DrawIndexed(36, 0, 0);




	WVP = cube2World * camView * camProjection;
	cbPerObject.WVP = XMMatrixTranspose(WVP);
	devcon->UpdateSubresource(constantBuffer.Get(), 0, NULL, &cbPerObject, 0, 0);

	devcon->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	devcon->PSSetShaderResources(0, 1, resourceTexture.GetAddressOf());
	devcon->PSSetSamplers(0, 1, texSamplerState.GetAddressOf());
	devcon->DrawIndexed(36, 0, 0);

	


	swapChain->Present(0, 0);

}

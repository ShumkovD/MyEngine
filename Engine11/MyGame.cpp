#include"MyEngine11.h"

void EngineClass::InitializeScene()
{
	//���C�g�j���O�̏�����
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

	//���S�ɂ��闧����	�s�N�T�[�N���b�s���O�ɐݒ肵�Ă��܂��B
	cube1World = XMMatrixIdentity();
	translation = XMMatrixTranslation(0.0f, 0.0f, 4.0f);
	rotation = XMMatrixRotationAxis(rotaxis, rot);
	cube1World = translation * rotation;
	//Y������]���Ă���	�����ɐݒ肵�Ă��܂��B
	cube2World = XMMatrixIdentity();
	rotation = XMMatrixRotationAxis(rotaxis, -rot);
	scaling = XMMatrixScaling(1.3f, 1.3f, 1.3f);
	cube2World = scaling * rotation;

	rotaxis = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
	//Z������]���Ă���	�s�����ɐݒ肵�Ă��܂��B
	cube3World = XMMatrixIdentity();
	translation = XMMatrixTranslation(0.0f, 4.0f, 0.0f);
	rotation = XMMatrixRotationAxis(rotaxis, rot);
	cube3World = translation * rotation;


}

void EngineClass::Render()
{
	//�V�[�����Z�b�g
	SettingWorld();
	//�w�i
	float color[] = { 0.1f,0.1f,0.1f,1 };
	//
	float blendFactor[] = { 0.75f,0.75f,0.75f,1.0f };
	devcon->ClearRenderTargetView(rtv.Get(), color);
	//���C�g�j���O
	cbPerFrame.light = light;
	devcon->UpdateSubresource1(constantPerFrameBuffer.Get(), 0, NULL, &cbPerFrame, 0, 0, 0);
	devcon->PSSetConstantBuffers1(0, 1, constantPerFrameBuffer.GetAddressOf(), 0, 0);
	//�s����
	devcon->OMSetBlendState(0, 0, 0xffffffff);

	devcon->ClearDepthStencilView(depthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	

		//Z������]���Ă���	�s�����ɐݒ肵�Ă��܂��B
		devcon->RSSetState(0);

		WVP = cube1World * camView * camProjection;
		cbPerObject.WVP = XMMatrixTranspose(WVP);
		cbPerObject.world = XMMatrixTranspose(cube1World);

		devcon->UpdateSubresource1(constantBuffer.Get(), 0, NULL, &cbPerObject, 0, 0, 0);
		devcon->VSSetConstantBuffers1(0, 1, constantBuffer.GetAddressOf(), 0, 0);
		devcon->PSSetShaderResources(0, 1, resourceTexture.GetAddressOf());
		devcon->PSSetSamplers(0, 1, texSamplerState.GetAddressOf());
		devcon->DrawIndexed(36, 0, 0);

		//���S�ɂ��闧����	�s�N�T�[�N���b�s���O�ɐݒ肵�Ă��܂��B
		
		devcon->RSSetState(NoCull.Get());

		WVP = cube2World * camView * camProjection;
		cbPerObject.WVP = XMMatrixTranspose(WVP);
		cbPerObject.world = XMMatrixTranspose(cube2World);

		devcon->UpdateSubresource(constantBuffer.Get(), 0, NULL, &cbPerObject, 0, 0);
		devcon->VSSetConstantBuffers1(0, 1, constantBuffer.GetAddressOf(),0, 0);
		devcon->PSSetShaderResources(0, 1, resource2Texture.GetAddressOf());
		devcon->PSSetSamplers(0, 1, texSamplerState.GetAddressOf());
		devcon->DrawIndexed(36, 0, 0);


		//Y������]���Ă���	�����ɐݒ肵�Ă��܂��B
		devcon->OMSetBlendState(transparent.Get(), blendFactor, 0xffffffff);

		WVP = cube3World * camView * camProjection;
		cbPerObject.WVP = XMMatrixTranspose(WVP);
		cbPerObject.world = XMMatrixTranspose(cube3World);

		devcon->UpdateSubresource(constantBuffer.Get(), 0, NULL, &cbPerObject, 0, 0);
		devcon->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
		devcon->PSSetShaderResources(0, 1, resourceTexture.GetAddressOf());
		devcon->PSSetSamplers(0, 1, texSamplerState.GetAddressOf());
		//���ʂ�`�悵�Ă���
		devcon->RSSetState(CCWCull.Get());
		devcon->DrawIndexed(36, 0, 0);
		//�\�ʂ�`�悵�܂�
		devcon->RSSetState(CWCull.Get());
		devcon->DrawIndexed(36, 0, 0);

		//2D�����_�[
	spriteBatch->Begin();
	DrawMyText(std::to_string(fps).c_str());
	spriteBatch->End();


	swapChain->Present(1, 0);

}

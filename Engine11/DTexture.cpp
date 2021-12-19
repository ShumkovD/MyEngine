#include"DTexture.h"

bool DTexture::LoadDDSTexture(std::wstring resourceAddress, Microsoft::WRL::ComPtr<ID3D11Device1> dev)
{
	HRESULT hr;
	hr = DirectX::CreateDDSTextureFromFile(dev.Get(), resourceAddress.c_str(), textureResource.GetAddressOf(), textureSRV.GetAddressOf(), NULL);
	std::wstring debugOut = L"Coudnt load texture at" + resourceAddress;
	this->textureAddress = resourceAddress;
	bool checked = ConsoleDebug::ErrorCheck(hr, debugOut);
	return checked;
}

bool DTexture::LoadWICTexture(std::wstring resourceAddress, Microsoft::WRL::ComPtr<ID3D11Device1> dev)
{
	HRESULT hr;
	hr = DirectX::CreateWICTextureFromFile(dev.Get(), resourceAddress.c_str(), this->textureResource.GetAddressOf(), this->textureSRV.GetAddressOf(), NULL);
	std::wstring debugOut = L"Coudnt load texture at" + resourceAddress;
	this->textureAddress = resourceAddress;
	bool checked = ConsoleDebug::ErrorCheck(hr, debugOut);
	return checked;
}
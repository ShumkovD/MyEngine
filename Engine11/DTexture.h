#pragma once
#include<d3d11.h>
#include<wrl/client.h>

#include<WICTextureLoader.h>
#include<DDSTextureLoader.h>

#include"ConsoleDebug.h"

class DTexture
{
	Microsoft::WRL::ComPtr<ID3D11Resource>	textureResource;
public:
	std::wstring textureAddress;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureSRV;
	bool LoadDDSTexture(std::wstring resourceAddress, Microsoft::WRL::ComPtr<ID3D11Device1> dev);
	bool LoadWICTexture(std::wstring resourceAddress, Microsoft::WRL::ComPtr<ID3D11Device1> dev);
};


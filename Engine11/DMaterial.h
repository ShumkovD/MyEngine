#pragma once

#include"DTexture.h"
#include<fstream>
#include<istream>
#include<sstream>
#include<vector>
#include<DirectXMath.h>
using namespace DirectX;

class DMaterial
{
public:
	//ïœêî
	DTexture texture;
	std::wstring matName;
	XMFLOAT4 difColor;
	XMFLOAT3 scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	bool hasTexture;
	bool transparent;
	//ä÷êî
	static bool ReadMTLFile(Microsoft::WRL::ComPtr<ID3D11Device1>, std::wstring address, std::vector<DMaterial> &material);
};
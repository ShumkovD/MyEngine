#pragma once
#pragma comment(lib, "d3d11.lib")
#include<d3d11.h>
#include<vector>
#include<fstream>
#include<istream>
#include<sstream>

#include<DirectXMath.h>
#include<DDSTextureLoader.h>
#include<Windows.h>
#include<wrl/client.h>

using namespace Microsoft;
using namespace WRL;
using namespace DirectX;

struct SurfaceMaterial
{
	std::wstring matName;
	XMFLOAT4 difColor;
	int texArrayIndex;
	bool hasTexture;
	bool transparent;
};

struct Vertex
{
	Vertex() {};
	//コンストラクタ
	Vertex(float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz)
		: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

	//頂点の位置
	XMFLOAT3 pos;
	//テクスチャー座標
	XMFLOAT2 texCoord;
	//ライトニングノルマル
	XMFLOAT3 normal;
};

class DMesh
{
private:
	ComPtr<ID3D11BlendState> transparency;
	ComPtr<ID3D11Buffer>	meshVertexBuffer;
	ComPtr<ID3D11Buffer>	meshIndexBuffer;
	XMMATRIX meshWorld;
	int meshSubsets = 0;
	std::vector<int> meshSubsetIndexStart;
	std::vector<int> meshSubsetTexture;
	std::vector<ComPtr<ID3D11ShaderResourceView>> meshSRV;
	std::vector<std::wstring> textureNameArray;
	std::vector<SurfaceMaterial> material;
public:
	bool LoadObjModel(std::wstring flename,
		ComPtr<ID3D11Buffer> &vertBuff,
		ComPtr<ID3D11Buffer>& indexBuff, 
		std::vector<int>& subsetIndexStart,
		std::vector<int>& subsetMaterialArray,
		std::vector<SurfaceMaterial>& material,
		int& subsetCount,
		bool isRHCoordSys,
		bool computeNormals,
		ComPtr<ID3D11Device1> dev);
};
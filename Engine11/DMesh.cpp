#include "DMesh.h"

//オブジェクトをファイルからの読み込み
bool DMesh::LoadObjModel(std::wstring filename,
	ComPtr<ID3D11Buffer>& vertBuff,
	ComPtr<ID3D11Buffer>& indexBuff,
	std::vector<int>& subsetIndexStart,
	std::vector<int>& subsetMaterialArray,
	std::vector<SurfaceMaterial>& material,
	int& subsetCount,
	bool isRHCoordSys,
	bool computeNormals,
	ComPtr<ID3D11Device1> dev)
{
	//エラー管理
	HRESULT hr = 0;
	
	std::wifstream	fileIn(filename.c_str());
	std::wstring	meshMatLib;
	//情報配列
	std::vector<DWORD>			indices;
	std::vector<XMFLOAT3>		vertPos;
	std::vector<XMFLOAT3>		vertNorm;
	std::vector<XMFLOAT2>		vertTexCoord;
	std::vector<std::wstring>	meshMaterials;
	//Vertex情報
	std::vector<int>			vertPosIndex;
	std::vector<int>			vertNormIndex;
	std::vector<int>			vertTCIndex;
	//初期化
	bool hasTexCoord =	false;
	bool hasNorm =		false;
	//仮変数
	std::wstring	meshMaterialsTemp;
	int				vertPosIndexTemp;
	int				vertNormIndexTemp;
	int				vertTCIndexTemp;

	wchar_t checkChar;
	std::wstring face;
	int vIndex = 0;
	int triangleCount = 0;
	int totalVerts = 0;
	int meshTriangles = 0;

	if (fileIn)
	{
		while (fileIn)
		{
			checkChar = fileIn.get();
			switch (checkChar)
			{
			case '#':
				checkChar = fileIn.get();
				while (checkChar != '\n')
					checkChar = fileIn.get();
				break;
			case 'v':
				checkChar = fileIn.get();
				if (checkChar == ' ')
				{
					float vz, vy, vx;
					fileIn >> vx >> vy >> vz;
					if (isRHCoordSys)
						vertPos.push_back(XMFLOAT3(vx, vy, vz * -1.0f));
					else
						vertPos.push_back(XMFLOAT3(vx, vy, vz));
				}
				if (checkChar == 't')
				{
					float vtcu, vtcv;
					fileIn >> vtcu >> vtcv;
					if (isRHCoordSys)
						vertTexCoord.push_back(XMFLOAT2(vtcu, 1.0f - vtcv));
					else
						vertTexCoord.push_back(XMFLOAT2(vtcu, vtcv));
					hasTexCoord = true;
				}
				if (checkChar == 'n')
				{
					float vnx, vny, vnz;
					fileIn >> vnx >> vny >> vnz;

					if (isRHCoordSys)
						vertNorm.push_back(XMFLOAT3(vnx, vny, vnz * -1.0f));
					else vertNorm.push_back(XMFLOAT3(vnx, vny, vnz));
					hasNorm = true;
				}
				break;
			case 'g':
				checkChar = fileIn.get();
				if (checkChar == ' ')
				{
					subsetIndexStart.push_back(vIndex);
					subsetCount++;
				}
				break;
				//フェースインデクス
			case 'f':
				checkChar = fileIn.get();
				if (checkChar == ' ')
				{
					face = L"";
					std::wstring VertDef;
					triangleCount = 0;

					checkChar = fileIn.get();
					while (checkChar!='\n')
					{
						face += checkChar;
						checkChar = fileIn.get();
						if (checkChar == ' ')
							triangleCount++;
					}
					if (face[face.length() - 1] == ' ')
						triangleCount--;

					triangleCount -= 1;
					
					std::wstringstream ss(face);

					if (face.length() > 0)
					{
						int firstVIndex, lastVIndex;
						for (int i = 0; i < 3; ++i)
						{
							ss >> VertDef;

							std::wstring vertPart;
							int whichPart = 0;
							for (int j = 0; j < VertDef.length(); j++)
							{
								if (VertDef[j] != '/')
									vertPart += VertDef[j];
								//フェース文字列の解析
								if (VertDef[j] == '/' || j == VertDef.length() - 1)
								{
									std::wistringstream wstringToInt(vertPart);
									if (whichPart == 0)
									{
										wstringToInt >> vertPosIndexTemp;
										vertPosIndexTemp -= 1;
										if (j == VertDef.length() - 1)
										{
											vertNormIndexTemp = 0;
											vertTCIndexTemp = 0;
										}
									}
									else if (whichPart == 1)
									{
										if (vertPart != L"")
										{
											wstringToInt >> vertTCIndexTemp;
											vertTCIndexTemp -= 1;
										}
										else
											vertTCIndexTemp = 0;
										if (j == VertDef.length() - 1)
											vertNormIndexTemp = 0;
									}
									else if (whichPart == 2)
									{
										wstringToInt >> vertNormIndexTemp;
										vertNormIndexTemp -= 1;
									}
									vertPart = L"";
									whichPart++;
								}
							}
							//読み込んだ情報を保存する
							if (subsetCount == 0)
							{
								subsetIndexStart.push_back(vIndex);
								subsetCount++;
							}
							bool vertAlreadyExists = false;
							if (totalVerts >= 3)
							{
								for (int iCheck = 0; iCheck < totalVerts; iCheck++)
								{
									if(vertPosIndexTemp==vertPosIndex[iCheck]&&!vertAlreadyExists)
									{
										if (vertTCIndexTemp == vertTCIndex[iCheck])
										{
											indices.push_back(iCheck);
											vertAlreadyExists = true;
										}
									}
								}
							}
							if (!vertAlreadyExists)
							{
								vertPosIndex.push_back(vertPosIndexTemp);
								vertTCIndex.push_back(vertTCIndexTemp);
								vertNormIndex.push_back(vertNormIndexTemp);
								totalVerts++;
								indices.push_back(totalVerts - 1);
							}
							if (i == 0)
							{
								firstVIndex = indices[vIndex];
							}
							if (i == 2)
							{
								lastVIndex = indices[vIndex];
							}
							vIndex++;
						}
						meshTriangles++;
						for (int l = 0; l < triangleCount - 1; l++)
						{
							indices.push_back(firstVIndex);
							vIndex++;
							indices.push_back(lastVIndex);
							vIndex++;
							ss >> VertDef;
							std::wstring vertPart;
							int whichPart = 0;
							for (int j = 0; j < VertDef.length(); j++)
							{
								if (VertDef[j] != '/')
									vertPart += VertDef[j];
								if (VertDef[j] == '/' || j == VertDef.length() - 1)
								{
									std::wstringstream wstringToInt(vertPart);
									if (whichPart == 0)
									{
										wstringToInt >> vertPosIndexTemp;
										vertPosIndexTemp -= 1;
										if (j == VertDef.length() - 1)
										{
											vertTCIndexTemp = 0;
											vertNormIndexTemp = 0;
										}
									}
									else if (whichPart == 1)
									{
										if (vertPart != L"")
										{
											wstringToInt >> vertTCIndexTemp;
											vertTCIndexTemp -= 1;
										}
										else
											vertTCIndexTemp = 0;
										if (j == VertDef.length() - 1)
											vertNormIndexTemp = 0;
									}
									else if (whichPart == 2)
									{
										wstringToInt >> vertNormIndexTemp;
										vertNormIndexTemp -= 1;
									}
									vertPart = L"";
									whichPart++;
								}
							}
							bool vertAlreadyExists = false;
							if (totalVerts >= 3)
							{
								for (int iCheck = 0; iCheck < totalVerts; iCheck++)
								{
									if (vertPosIndexTemp == vertPosIndex[iCheck] && !vertAlreadyExists)
									{
										if (vertTCIndexTemp == vertTCIndex[iCheck])
										{
											indices.push_back(iCheck);
											vertAlreadyExists = true;
										}
									}
								}
							}
							if (!vertAlreadyExists)
							{
								vertPosIndex.push_back(vertPosIndexTemp);
								vertTCIndex.push_back(vertTCIndexTemp);
								vertNormIndex.push_back(vertNormIndexTemp);
								totalVerts++;
								indices.push_back(totalVerts - 1);
							}
							lastVIndex = indices[vIndex];
							meshTriangles++;
							vIndex++;
						}
					}
				}
				break;
				//マテリアルの読み込み
			case 'm':
				checkChar = fileIn.get();
				if (checkChar == 't')
				{
					checkChar = fileIn.get();
					if (checkChar == 'l')
					{
						checkChar = fileIn.get();
						if (checkChar == 'l')
						{
							checkChar = fileIn.get();
							if (checkChar == 'i')
							{
								checkChar = fileIn.get();
								if (checkChar == 'b')
								{
									checkChar = fileIn.get();
									if (checkChar == ' ')
									{
										fileIn >> meshMatLib;
									}
								}
							}
						}
					}
				}
				break;
				//グループマテリアルの読み込み
			case 'u':
				if (checkChar == 's')
				{
					checkChar = fileIn.get();
					if (checkChar == 'e')
					{
						checkChar = fileIn.get();
						if (checkChar == 'm')
						{
							checkChar = fileIn.get();
							if (checkChar == 't')
							{
								checkChar = fileIn.get();
								if (checkChar == 'l')
								{
									checkChar = fileIn.get();
									if (checkChar == ' ')
									{
										meshMaterialsTemp = L"";
										fileIn >> meshMaterialsTemp;
										meshMaterials.push_back(meshMaterialsTemp);
									}
								}
							}
						}
					}
				}
				break;
			default:
				break;
			}
			
		}
	}
	else
	{
		//エラー出力
		std::wstring message = L"Could not open: ";
		message += filename;
		MessageBox(0, message.c_str(),    //display message
			L"Error", MB_OK);

		return false;
	}

	subsetIndexStart.push_back(vIndex);
	if (subsetIndexStart[1] == 0)
	{
		subsetIndexStart.erase(subsetIndexStart.begin() + 1);
		meshSubsets--;
	}
	if (!hasTexCoord)
		vertTexCoord.push_back(XMFLOAT2(0.0f, 0.0f));
	if (!hasNorm)
		vertNorm.push_back(XMFLOAT3(0.0f, 0.0f, 0.0f));
	fileIn.close();
	fileIn.open(meshMatLib.c_str());
	std::wstring lastStringRead;
	unsigned int matCount = material.size();
	bool kdset = false;
	if (fileIn)
	{
		checkChar = fileIn.get();
		switch (checkChar)
		{
		case '#':
			checkChar = fileIn.get();
			while (checkChar != '\n')
				checkChar = fileIn.get();
			break;
		case'K':
			checkChar = fileIn.get();
			if (checkChar == 'd')
			{
				checkChar = fileIn.get();
				fileIn >> material[matCount - 1].difColor.x;
				fileIn >> material[matCount - 1].difColor.y;
				fileIn >> material[matCount - 1].difColor.z;
				kdset = true;
			}
			if (checkChar == 'a')
			{
				checkChar = fileIn.get();
				if (!kdset)
				{
					fileIn >> material[matCount - 1].difColor.x;
					fileIn >> material[matCount - 1].difColor.y;
					fileIn >> material[matCount - 1].difColor.z;
				}
			}
			break;
			//透明度
		case 'T':
			checkChar = fileIn.get();
			if (checkChar == 'r')
			{
				checkChar = fileIn.get();
				float Transparency;
				fileIn >> Transparency;
				material[matCount - 1].difColor.w = Transparency;
				if (Transparency > 0.0f)
					material[matCount - 1].transparent = true;
			}
			break;
			//透明度はたまにファイルで’ｄ’を使っています
		case 'd':
			checkChar = fileIn.get();
			if (checkChar == '　')
			{
				float Transparency;
				fileIn >> Transparency;

				Transparency = 1.0f - Transparency;
				material[matCount - 1].difColor.w = Transparency;
				if (Transparency > 0.0f)
					material[matCount - 1].transparent = true;
			}
			break;
		case'm':
			checkChar = fileIn.get();
			if (checkChar == 'a')
			{
				checkChar = fileIn.get();
				if (checkChar == 'p')
				{
					checkChar = fileIn.get();
					if (checkChar == '_')
					{
						checkChar = fileIn.get();

						if (checkChar == 'K')
						{
							checkChar = fileIn.get();
							if (checkChar == 'd')
							{
								std::wstring fileNamePath;
								fileIn.get();
								bool textFilePathEnd = false;
								while (!textFilePathEnd)
								{
									checkChar = fileIn.get();
									fileNamePath += checkChar;
									if (checkChar == '.')
									{
										for (int i = 0; i < 3; i++)
											fileNamePath += fileIn.get();
										textFilePathEnd = true;
									}
								}
								bool alreadyLoaded = false;
								for (int i = 0; i < textureNameArray.size(); i++)
								{
									if (fileNamePath == textureNameArray[i])
									{
										alreadyLoaded = true;
										material[matCount - 1].texArrayIndex = i;
										material[matCount - 1].hasTexture = true;
									}
								}
								if (!alreadyLoaded)
								{
									ComPtr<ID3D11ShaderResourceView> tempMeshRSV;
									hr = CreateDDSTextureFromFile(dev.Get(), fileNamePath.c_str(), NULL, &tempMeshRSV, NULL, NULL);
									if (SUCCEEDED(hr))
									{
										textureNameArray.push_back(fileNamePath.c_str());
										material[matCount - 1].texArrayIndex = meshSRV.size();
										meshSRV.push_back(tempMeshRSV);
										material[matCount - 1].hasTexture = true;
									}
								}
							}
						}
						else if (checkChar == 'd')
						{
							material[matCount - 1].transparent = true;
						}
					}
				}
			}
			break;
		case 'n':
			checkChar = fileIn.get();
			if (checkChar == 'e')
			{
				checkChar = fileIn.get();
				if (checkChar == 'w')
				{
					checkChar = fileIn.get();
					if (checkChar == 'm')
					{
						checkChar = fileIn.get();
						if (checkChar == 't')
						{
							checkChar = fileIn.get();
							if (checkChar == 'l')
							{
								checkChar = fileIn.get();
								if (checkChar == ' ')
								{
									SurfaceMaterial tempMat;
									material.push_back(tempMat);
									fileIn >> material[matCount].matName;
									material[matCount].transparent = false;
									material[matCount].hasTexture = false;
									material[matCount].texArrayIndex = 0;
									matCount++;
									kdset = false;
								}
							}
						}
					}
				}
			}
			break;
		default:
			break;
		}
		
	}
	else
	{
		std::wstring message = L"Could not open: ";
		message += meshMatLib;
		MessageBox(0, message.c_str(),
			L"Error", MB_OK);

		return false;
	}

	for (int i = 0; i < meshSubsets; i++)
	{
		bool hasMat = false;
		for (int j = 0; j < material.size(); j++)
		{
			if (meshMaterials[i] == material[j].matName)
			{
				subsetMaterialArray.push_back(j);
				hasMat = true;
			}
		}
		if (!hasMat)
			subsetMaterialArray.push_back(0);
	}
	std::vector<Vertex> vertices;
	Vertex tempVert;
	for (int j = 0; j < totalVerts; j++)
	{
		tempVert.pos = vertPos[vertPosIndex[j]];
		tempVert.texCoord = vertTexCoord[vertTCIndex[j]];
		tempVert.normal = vertNorm[vertNormIndex[j]];
		vertices.push_back(tempVert);
	}

	if (computeNormals)
	{
		std::vector<XMFLOAT3> tempNormal;
		XMFLOAT3 unnormalized = XMFLOAT3(0.0f, 0.0f, 0.0f);
		float vecX, vecY, vecZ;
		XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		for (long long int i = 0; i < meshTriangles; i++)
		{
			vecX = vertices[indices[(i * 3)]].pos.x - vertices[indices[(i * 3) + 2]].pos.x;
			vecY = vertices[indices[(i * 3)]].pos.y - vertices[indices[(i * 3) + 2]].pos.y;
			vecZ = vertices[indices[(i * 3)]].pos.z - vertices[indices[(i * 3) + 2]].pos.z;
			edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f);    //Create our first edge

			vecX = vertices[indices[(i * 3) + 2]].pos.x - vertices[indices[(i * 3) + 1]].pos.x;
			vecY = vertices[indices[(i * 3) + 2]].pos.y - vertices[indices[(i * 3) + 1]].pos.y;
			vecZ = vertices[indices[(i * 3) + 2]].pos.z - vertices[indices[(i * 3) + 1]].pos.z;
			edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f);    //Create our second edge

			XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));
			tempNormal.push_back(unnormalized); 
		}
		//TODO
	}

}
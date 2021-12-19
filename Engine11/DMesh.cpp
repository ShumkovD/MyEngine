#include "DMesh.h"

//オブジェクトをファイルからの読み込み
bool DMesh::LoadObjModel(std::wstring filename,
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
		while (!fileIn.eof())
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
			//case 'g':
			//	checkChar = fileIn.get();
			//	if (checkChar == ' ')
			//	{
			//		meshSubsetIndexStart.push_back(vIndex);
			//		meshSubsets++;
			//	}
			//	break;
			//	//フェースインデクス
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
							if (meshSubsets == 0)
							{
								meshSubsetIndexStart.push_back(vIndex);
								meshSubsets++;
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
										meshMatLib = fileNamePath;
									}
								}
							}
						}
					}
				}
				break;
				//グループマテリアルの読み込み
			case 'u':
				checkChar = fileIn.get();
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
										meshSubsets++;
										meshSubsetIndexStart.push_back(vIndex);
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

	meshSubsetIndexStart.push_back(vIndex);
	if (meshSubsetIndexStart[1] == 0)
	{
		meshSubsetIndexStart.erase(meshSubsetIndexStart.begin() + 1);
		meshSubsets--;
	}
	if (!hasTexCoord)
		vertTexCoord.push_back(XMFLOAT2(0.0f, 0.0f));
	if (!hasNorm)
		vertNorm.push_back(XMFLOAT3(0.0f, 0.0f, 0.0f));
	fileIn.close();
	
	DMaterial::ReadMTLFile(dev, meshMatLib, material);

	for (int i = 0; i < meshSubsets; i++)
	{
		bool hasMat = false;
		for (int j = 0; j < material.size(); j++)
		{
			if (meshMaterials[i] == material[j].matName)
			{
				meshSubsetTexture.push_back(j);
				hasMat = true;
			}
		}
		if (!hasMat)
			meshSubsetTexture.push_back(0);
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
		XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		int facesUsing = 0;
		float tX, tY, tZ;
		for (int i = 0; i < totalVerts; i++)
		{
			for (long long int j = 0; j < meshTriangles; j++)
			{
				if (indices[j * 3] == i || indices[(j * 3) + 1] == i || indices[(j * 3) + 2] == i)
				{
					tX = XMVectorGetX(normalSum) + tempNormal[j].x;
					tY = XMVectorGetX(normalSum) + tempNormal[j].y;
					tZ = XMVectorGetX(normalSum) + tempNormal[j].z;
					normalSum = XMVectorSet(tX, tY, tZ, 0);
					facesUsing++;
				}
				normalSum = normalSum / facesUsing;
				normalSum = XMVector3Normalize(normalSum);
				vertices[i].normal.x = XMVectorGetX(normalSum);
				vertices[i].normal.y = XMVectorGetY(normalSum);
				vertices[i].normal.z = XMVectorGetZ(normalSum);
				XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				int facesUsing = 0;
			}
		}
	}
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * meshTriangles * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	dev->CreateBuffer(&indexBufferDesc, &iinitData, meshIndexBuffer.GetAddressOf());

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * totalVerts;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];
	dev->CreateBuffer(&vertexBufferDesc, &vertexBufferData, meshVertexBuffer.GetAddressOf());
	return true;
}
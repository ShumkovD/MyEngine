#include"DMaterial.h"


bool DMaterial::ReadMTLFile(Microsoft::WRL::ComPtr<ID3D11Device1> dev, std::wstring meshMatLib, std::vector<DMaterial> &material)
{
	HRESULT hr;
	meshMatLib = L"../Resources/Model/" + meshMatLib;
	std::wifstream fileIn(meshMatLib.c_str());

	//fileIn.open(meshMatLib);
	std::wstring lastStringRead;
	unsigned int matCount = material.size();
	bool kdset = false;
	if (fileIn)
	{
		while (fileIn)
		{
			char checkChar = fileIn.get();
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
									float sx = 1.0f;
									float sy = 1.0f;
									float sz = 1.0f;
									fileIn.get();
									checkChar = fileIn.get();
									if (checkChar == '-')
									{
										checkChar = fileIn.get();
										if (checkChar == 's')
										{
											fileIn.get();
											fileIn >> sx >> sy >> sz;
											fileIn.get();
											checkChar = fileIn.get();
										}
									}
									std::wstring fileNamePath;
									fileNamePath += checkChar;
									bool textFilePathEnd = false;
									while (!textFilePathEnd)
									{
										checkChar = fileIn.get();
										fileNamePath += checkChar;
										if (checkChar == '.')
										{
											for (int i = 0; i < 3; i++)
												fileNamePath += fileIn.get();
											fileNamePath = L"../Resources/Model/" + fileNamePath;
											textFilePathEnd = true;
										}
									}
									bool alreadyLoaded = false;
									for (int i = 0; i < material.size(); i++)
									{
										if (fileNamePath == material[i].texture.textureAddress)
										{
											alreadyLoaded = true;
											material[matCount - 1].hasTexture = true;
											material[matCount - 1].texture = material[i].texture;
											material[matCount - 1].scale = XMFLOAT3(sx, sy, sz);
										}
									}
									if (!alreadyLoaded)
									{
										bool loaded = false;
										loaded = material[matCount - 1].texture.LoadWICTexture(fileNamePath.c_str(), dev.Get());
										if (loaded)
										{
											material[matCount - 1].hasTexture = true;
											material[matCount - 1].scale = XMFLOAT3(sx, sy, sz);
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
										DMaterial tempMat;
										material.push_back(tempMat);
										fileIn >> material[matCount].matName;
										material[matCount].transparent = false;
										material[matCount].hasTexture = false;
										ZeroMemory(&material[matCount].texture, sizeof(material[matCount].texture));
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
	}
	else
	{
		std::wstring message = L"Could not open Mat: ";
		message += meshMatLib;
		MessageBox(0, message.c_str(),
			L"Error", MB_OK);

		return false;
	}
	return true;
}

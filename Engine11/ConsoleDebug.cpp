#include"ConsoleDebug.h"

bool ConsoleDebug::ErrorCheck(HRESULT hr, std::wstring outputLine)
{
	if (FAILED(hr))
	{
		OutputDebugStringW(outputLine.c_str());
		OutputDebugStringW(L"\n");
		return false;
	}
	return true;
}
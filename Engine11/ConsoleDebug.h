#pragma once
#include<Windows.h>
#include<iostream>

static class ConsoleDebug
{
public:
	static bool ErrorCheck(HRESULT hr, std::wstring outputLine);
};
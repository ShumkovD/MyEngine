#include"MyEngine11.h"


void EngineClass::InitScene()
{

}

void EngineClass::Render()
{
	float color[] = { 1,1,0,1 };
	devcon->ClearRenderTargetView(rtv.Get(), color);
	swapChain->Present(0, 0);
}
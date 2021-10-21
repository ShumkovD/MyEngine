#include"MyEngine11.h"


void EngineClass::InitScene()
{

}

void EngineClass::Render()
{
	float color[] = { 0,0,0,1 };
	devcon->ClearRenderTargetView(rtv.Get(), color);
	devcon->DrawIndexed(6, 0, 0);
	swapChain->Present(0, 0);
}
#include"MyEngine11.h"


void EngineClass::InitScene()
{

}

void EngineClass::Render()
{
	float color[] = { 0,0,0,1 };
	devcon->ClearRenderTargetView(rtv.Get(), color);
	devcon->ClearDepthStencilView(depthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	devcon->DrawIndexed(9, 0, 0);
	swapChain->Present(0, 0);
}
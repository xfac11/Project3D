#include "Graphics.h"

bool Graphics::render()
{

	float color[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //parameter color?
	Direct3D->BeginScene(color);





	Direct3D->EndScene();

	return true;
}

Graphics::Graphics()
{
	this->Direct3D = nullptr;
	this->Camera = nullptr;
}

Graphics::~Graphics()
{
	this->Shutdown(); //????
}

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result = false;
	Direct3D = new D3D;
	if (!Direct3D)
	{
		result = false;
	}
	result = Direct3D->initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize Direct3D", "Error", MB_OK); //L"", L"", ;
		result = false;
	}

	return result;
}

void Graphics::Shutdown()
{
	if(Direct3D)
	{
		Direct3D->Shutdown();
		delete Direct3D;
		Direct3D = NULL;
	}
}

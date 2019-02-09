#ifndef D3D_H
#define D3D_H
#include <d3d11.h>
#include <directxmath.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
//using namespace DirectX;
__declspec(align(16)) class D3D
{

private:
	bool vSync_enabled;
	int videoCardMemory;
	char videoCardDescription[128];
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;
	//ID3D11RasterizerState* rasterState;
	DirectX::XMMATRIX projectionMatrix;
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX orthoMatrix;  //switch to 

	//maybe D3D should have CBData instead of ColorShader

	float dist;
	float gIncrement;
	/*
	XMVECTOR CamPos = XMVectorSet(0.0, 0.0, -2, 0.0);  //-dist 
	XMVECTOR LookAt = XMVectorSet(0.0, 0.0, 0.0, 0.0); //change to first person view
	XMVECTOR Up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
	XMMATRIX View = XMMatrixLookAtLH(CamPos, LookAt, Up);
	XMMATRIX World = XMMatrixRotationY(gIncrement);
	
	*/


public:
	D3D();
	~D3D();
	void* operator new(size_t i) //test
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

	bool initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	void Shutdown();
	void BeginScene(float color[4]);
	void EndScene();

	ID3D11Device*& GetDevice();
	ID3D11DeviceContext*& GetDeviceContext();

	DirectX::XMMATRIX& GetProjectionMatrix();
	DirectX::XMMATRIX& GetWorldMatrix();
	//DirectX::XMMATRIX& GetOrthoMatrix();

	int & GetVideoCardInfo(char* cardName);//(, int& memory);
};
#endif
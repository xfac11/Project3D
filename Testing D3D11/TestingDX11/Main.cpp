//#include <crtdbg.h>
#include <Windows.h>
#include <math.h>
//#include <DirectXMath.h>
//#include <d3d11.h>
#include <d3dcompiler.h>
#include "bth_image.h"
#include "Structs.h"
#include "QuadHandler.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include"headerFiles.h"
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#define WIDTH 768.0f
#define HEIGHT 768.0f
IDXGISwapChain* gSwapChain = nullptr;
ID3D11Device* gDevice = nullptr;
ID3D11DeviceContext* gDeviceContext = nullptr;
ID3D11RenderTargetView* gBackbufferRTV = nullptr;

//texture
ID3D11SamplerState* SamplerState = nullptr;
ID3D11ShaderResourceView* Texture = nullptr;

ID3D11Buffer* gVertexBuffer = nullptr;
ID3D11InputLayout* gVertexLayout = nullptr;

ID3D11VertexShader* gVertexShader = nullptr;
ID3D11PixelShader* gPixelShader = nullptr;
ID3D11GeometryShader* gGeometryShader = nullptr;

//depth test
ID3D11Texture2D* pDepthStencilB = NULL;
ID3D11DepthStencilState * pDSState = NULL;
// Create the depth stencil view
ID3D11DepthStencilView* pDSV = NULL;

//CB and perFrame have structs.h
CBData* gConstantBufferData;
ID3D11Buffer* gConstantBuffer = NULL;

PerFrameMatrices* gMatricesPerFrame = NULL;
ID3D11Buffer* gMatrixPerFrameBuffer = NULL;

HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HRESULT CreateDirect3DContext(HWND wndHandle);
void SetViewport();
HRESULT CreateShaders();
void createConstantBuffer();
void setSampler();
void CreateQuadData(Vertex3D obj[6], QuadHandler & quadHandler);
void Render(float gClearColour[3]);

void cleanUp(HWND & wndHandle);
using namespace DirectX;
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	MSG msg = { 0 };
	HWND wndHandle = InitWindow(hInstance);
	int nmcShow = nCmdShow;
	QuadHandler quadHandler;
	Vertex3D TwoTris[6] = {
						-0.5f, -0.5f, 0.f,	//pos
						//0.f, 0.f, 0.f,	//color
						0.0f, 1.0f,			//tex u & v
						1.f, 1.f, 1.f,		//normal
						-0.5f, -0.5f, 0.f,  //point

						-0.5f, 0.5f, 0.f,
						0.f, 0.f,
						1.f, 1.f, 1.f,
						-0.5f, 0.5f, 0.f,

						0.5, 0.5, 0.0,
						1.0, 0.0,
						1.f, 1.f, 1.f,
						0.5, 0.5, 0.0,

						-0.5, -0.5, 0.0,
						0.0, 1.0,
						1.f, 1.f, 1.f,
						-0.5, -0.5, 0.0,

						0.5f, 0.5f, 0.f,
						1.0, 0.0,
						1.f, 1.f, 1.f,
						-0.5, -0.5, 0.0,

						0.5f, -0.5f, 0.f,
						1.f, 1.f,
						1.f, 1.f, 1.f,
						0.5f, -0.5f, 0.f,
	};

	float gClearColour[3] = { 0.2f,0.5f,0.1f };
	float gIncrement = 0;
	float dist = 0.1f;
	float xPart = 0.0f;
	if (wndHandle)
	{
		CreateDirect3DContext(wndHandle);
		SetViewport();
		CreateShaders();
		createConstantBuffer();
		setSampler();
		CreateQuadData(TwoTris, quadHandler);
		ShowWindow(wndHandle, nmcShow);

		std::unique_ptr<DirectX::Keyboard> m_keyboard;
		
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); // (void)io;
		ImGui_ImplWin32_Init(wndHandle);
		ImGui_ImplDX11_Init(gDevice, gDeviceContext);
		ImGui::StyleColorsDark();

		while (WM_QUIT != msg.message)
		{
			if (msg.message == WM_KEYDOWN)
			{
				if (msg.wParam == 'W')
				{
					dist += 0.01f;
				}
				else if (msg.wParam == 'S')
				{
					dist -= 0.01f;
				}
				if (msg.wParam == 'A')
				{
					xPart -= 0.01f;
				}
				else if (msg.wParam == 'D')
				{
					xPart += 0.01f;
				}
			}
			
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				
				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
				ImGui::Text("This is some unuseful text.");
				ImGui::SliderFloat("dist", &dist, -10.0f, 10.0f);
				ImGui::SliderFloat("rotation", &gIncrement, -10.0f, 20.0f);
				ImGui::ColorEdit3("bg-color", (float*)&gClearColour);
				ImGui::ColorEdit4("Triangle data", (float*)gConstantBufferData);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
				//keyboard inputs from dxtk(DirectX Tool Kit) Which we can use for window and hopefully keyboard and mouse
				/*
				m_keyboard = std::make_unique<Keyboard>();
				auto kb = m_keyboard->GetState();
				if (kb.Escape)
				{
					//ExitGame();
				}
				else if (kb.W)
				{
					//Forward
				}
				*/
				//gIncrement += 0.8f * ImGui::GetIO().DeltaTime;
				gConstantBufferData->offset = sin(gIncrement);

				D3D11_MAPPED_SUBRESOURCE mappedMemory;
				gDeviceContext->Map(gConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
				memcpy(mappedMemory.pData, gConstantBufferData, sizeof(CBData));
				gDeviceContext->Unmap(gConstantBuffer, 0);

				XMVECTOR CamPos = XMVectorSet(xPart, 0.0, -dist, 0.0);  //-dist 
				XMVECTOR LookAt = XMVectorSet(0.0, 0.0, 0.0, 0.0)+CamPos; //change to first person view
				XMVECTOR Up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
				XMMATRIX View = XMMatrixLookAtLH(CamPos, LookAt, Up);
				XMMATRIX World = XMMatrixRotationY(gIncrement);
				View = XMMatrixTranspose(View);
				World = XMMatrixTranspose(World);
				XMMATRIX WorldView = XMMatrixMultiply(View, World);
				XMMATRIX Projection = XMMatrixPerspectiveFovLH((0.45f*XM_PI), WIDTH / HEIGHT, 0.1f, 20.f);
				Projection = XMMatrixTranspose(Projection);
				XMMATRIX WorldViewProj = XMMatrixMultiply(Projection, WorldView);
				WorldViewProj = XMMatrixTranspose(WorldViewProj);
				gMatricesPerFrame->worldMatrix = WorldViewProj;

				gDeviceContext->Map(gMatrixPerFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
				memcpy(mappedMemory.pData, gMatricesPerFrame, sizeof(PerFrameMatrices));
				gDeviceContext->Unmap(gMatrixPerFrameBuffer, 0);

				//Update();
				Render(gClearColour);
				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
				gSwapChain->Present(0, 0);
			}

		}
		cleanUp(wndHandle);
	}

	//system("pause");
	return (int)msg.wParam;
}

HWND InitWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	//wcex.hIcon = NULL;
	//wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//wcex.hbrBackground = NULL;
	wcex.lpszClassName ="Demo";
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, (int)WIDTH, (int)HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindow(
		"Demo",
		"Title Demo",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);
	return hwnd;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{ //event-handling, event happens to window
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

HRESULT CreateDirect3DContext(HWND wndHandle)
{
	DXGI_SWAP_CHAIN_DESC swapchainDesc;
	ZeroMemory(&swapchainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	//swapchainDesc.BufferDesc.Width = 0;
	//swapchainDesc.BufferDesc.Height = 0; //defualt to window size
	swapchainDesc.BufferCount = 1;
	swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.OutputWindow = wndHandle;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.Windowed = TRUE;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapchainDesc,
		&gSwapChain,
		&gDevice,
		NULL,
		&gDeviceContext);

	if (SUCCEEDED(hr))
	{
		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		gDevice->CreateRenderTargetView(pBackBuffer, NULL, &gBackbufferRTV);
		pBackBuffer->Release();

		//depth desc
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = (float)WIDTH;
		descDepth.Height = (float)HEIGHT;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //DXGI_FORMAT_R32_TYPELESS;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;// | D3D10_BIND_SHADER_RESOURCE;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		hr = gDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencilB);
		if (FAILED(hr))
		{
			// deal with error...
		}
		hr = gDevice->CreateDepthStencilView(pDepthStencilB, NULL, &pDSV);
		if (FAILED(hr))
		{
			// deal with error...
		}
		gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, pDSV);

		//the depth Stencil State
		D3D11_DEPTH_STENCIL_DESC dsDesc;
		ZeroMemory(&dsDesc, sizeof(dsDesc));
		// Depth test parameters
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		// Create depth stencil state
		hr = gDevice->CreateDepthStencilState(&dsDesc, &pDSState);
		if (FAILED(hr))
		{
			// deal with error...
		}
	}
	return hr;
}

HRESULT CreateShaders()
{
	// Binary Large OBject (BLOB), for compiled shader, and errors.
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	HRESULT result = D3DCompileFromFile(
		L"Vertex.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pVS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pVS)
			pVS->Release();
		return result;
	}

	gDevice->CreateVertexShader(
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		nullptr,
		&gVertexShader
	);

	// create input layout (verified using vertex shader)
	// Press F1 in Visual Studio with the cursor over the datatype to jump
	// to the documentation online!
	// please read:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb205117(v=vs.85).aspx
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"SV_POSITION",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			D3D11_APPEND_ALIGNED_ELEMENT, // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT, //2 values
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		//{
		//	"COLOR",
		//	0,				// same slot as previous (same vertexBuffer)
		//	DXGI_FORMAT_R32G32B32_FLOAT,
		//	0,
		//	D3D11_APPEND_ALIGNED_ELEMENT,							// offset of FIRST element (after POSITION)
		//	D3D11_INPUT_PER_VERTEX_DATA,
		//	0
		//},
		{
			"NORMAL",
			0,				// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"MYVALUE",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}

	};
	gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayout);

	// we do not need anymore this COM object, so we release it.
	pVS->Release();

	//create pixel shader
	ID3DBlob* pPS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"Fragment.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);


	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pPS)
			pPS->Release();
		return result;
	}

	gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShader);
	// we do not need anymore this COM object, so we release it.
	pPS->Release();

	ID3DBlob* pGS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;
	result = D3DCompileFromFile(
		L"GeometryShader.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"GS_main",		// entry point
		"gs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreateGeometryShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gGeometryShader
	);

	//gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(),nullptr,&gGeometryShader);
	pGS->Release();

	return S_OK;
}

void setSampler()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = gDevice->CreateSamplerState(&desc, &SamplerState);
	if (FAILED(hr))
	{
		//deal with error. Log it maybe
	}

	ID3D11Texture2D* tex = nullptr;
	D3D11_SUBRESOURCE_DATA texInitData = { 0 };
	ZeroMemory(&texInitData, sizeof(texInitData));
	texInitData.pSysMem = (const void*)BTH_IMAGE_DATA;
	texInitData.SysMemPitch = BTH_IMAGE_WIDTH * 4;
	texInitData.SysMemSlicePitch = BTH_IMAGE_HEIGHT * BTH_IMAGE_WIDTH * 4;


	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Height = BTH_IMAGE_HEIGHT;
	texDesc.Width = BTH_IMAGE_WIDTH;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	gDevice->CreateTexture2D(&texDesc, &texInitData, &tex);
	gDevice->CreateShaderResourceView(tex, NULL, &Texture);
}


void SetViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)WIDTH;
	vp.Height = (float)HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	gDeviceContext->RSSetViewports(1, &vp);
}


void Render(float gClearColour[3])
{
	gDeviceContext->ClearRenderTargetView(gBackbufferRTV, gClearColour);
	gDeviceContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//
	gDeviceContext->PSSetConstantBuffers(0, 1, &gConstantBuffer);
	gDeviceContext->VSSetConstantBuffers(0, 1, &gConstantBuffer);
	gDeviceContext->GSSetConstantBuffers(0, 1, &gMatrixPerFrameBuffer);
	gDeviceContext->PSSetShaderResources(0, 1, &Texture);
	gDeviceContext->OMSetDepthStencilState(pDSState, 0); //1


	gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(gGeometryShader, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);

	UINT32 vertexSize = sizeof(Vertex3D);
	UINT32 offset = 0;
	gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &vertexSize, &offset);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->IASetInputLayout(gVertexLayout);
	gDeviceContext->Draw(6, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0); //already initilized?+

}

void CreateQuadData(Vertex3D obj[6], QuadHandler & quadHandler)
{
	quadHandler.addQuad(obj, gDevice, gVertexBuffer);
}

void cleanUp(HWND &wndHandle)
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	_aligned_free(gConstantBufferData); //struct
	gConstantBuffer->Release();
	_aligned_free(gMatricesPerFrame); //struct
	gMatrixPerFrameBuffer->Release();

	gVertexBuffer->Release();
	gVertexLayout->Release();
	gVertexShader->Release();
	gGeometryShader->Release();
	gPixelShader->Release();
	
	pDepthStencilB->Release();
	pDSState->Release();
	pDSV->Release();

	SamplerState->Release();
	Texture->Release();

	gSwapChain->Release();
	gDevice->Release();
	gDeviceContext->Release();
	gBackbufferRTV->Release();

	DestroyWindow(wndHandle);
}

void createConstantBuffer()
{
	gConstantBufferData = (CBData*)_aligned_malloc(sizeof(CBData), 16);
	gConstantBufferData->colour[0] = 0.5f;
	gConstantBufferData->colour[1] = 0.5f;
	gConstantBufferData->colour[2] = 0.5f;
	gConstantBufferData->colour[3] = 1.0f;
	gConstantBufferData->offset = 0.0f;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(CBData);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA pData;
	ZeroMemory(&pData, sizeof(pData));
	pData.pSysMem = gConstantBufferData;
	pData.SysMemPitch = 0;
	pData.SysMemSlicePitch = 0;

	HRESULT hr;
	hr = gDevice->CreateBuffer(&desc, &pData, &gConstantBuffer);
	if (FAILED(hr))
	{
		// deal with error...

	}


	gMatricesPerFrame = (PerFrameMatrices*)_aligned_malloc(sizeof(PerFrameMatrices), 16);
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(PerFrameMatrices);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ZeroMemory(&pData, sizeof(pData));
	pData.pSysMem = gConstantBufferData;
	pData.SysMemPitch = 0;
	pData.SysMemSlicePitch = 0;

	hr = gDevice->CreateBuffer(&desc, &pData, &gMatrixPerFrameBuffer);
	if (FAILED(hr))
	{
		// deal with error...
	}
}
void updateCamera()
{

}
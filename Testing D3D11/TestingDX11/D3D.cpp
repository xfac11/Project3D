#include "D3D.h"

D3D::D3D()
{
	this->vSync_enabled = NULL;
	this->videoCardMemory = NULL;
	this->videoCardDescription[127] = NULL;

	this->swapChain =  nullptr;
	this->device = nullptr;
	this->deviceContext = nullptr;
	this->renderTargetView = nullptr;
	this->depthStencilBuffer = nullptr;
	this->depthStencilState = nullptr;
	this->depthDisStencilState = nullptr;
	this->depthStencilView = nullptr;
	//rasterState = nullptr;
	this->debug = nullptr;
	this->dist = 0.1f;
	this->gIncrement = 0;
}

D3D::~D3D()
{
}

void D3D::setIncrement(float g)
{
	this->gIncrement = g;
}

bool D3D::initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	//bool result = false;
	HRESULT result;
	//IDXGIFactory* factory;
	//IDXGIAdapter* adapter;
	//IDXGIOutput* adapterOutput;
	//unsigned int numModes, i, numerator, denominator;
	//unsigned long long stringLength;
	//DXGI_MODE_DESC* displayModeList;
	//DXGI_ADAPTER_DESC adapterDesc;
	//int error;
	//DXGI_SWAP_CHAIN_DESC swapchainDesc;
	//D3D_FEATURE_LEVEL featureLevel;
	//ID3D11Texture2D* backBufferPtr = nullptr;
	//D3D11_TEXTURE2D_DESC depthBufferDesc;
	//D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	//D3D11_RASTERIZER_DESC rasterDesc;
	//D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	vSync_enabled = vsync;

	DXGI_SWAP_CHAIN_DESC swapchainDesc;
	ZeroMemory(&swapchainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	//swapchainDesc.BufferDesc.Width = 0;
	//swapchainDesc.BufferDesc.Height = 0; //defualt to window size
	swapchainDesc.BufferCount = 1;
	swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.OutputWindow = hwnd;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.Windowed = TRUE;

	//if (vSync_enabled)
	//{
	//	swapchainDesc.BufferDesc.RefreshRate.Numerator = numerator;
	//	swapchainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	//}
	//else
	//{
	//	swapchainDesc.BufferDesc.RefreshRate.Numerator = 0;
	//	swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
	//}

	result = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapchainDesc,
		&swapChain,
		&device,
		NULL,
		&deviceContext);

	if (SUCCEEDED(result))
	{
		// get the address of the back buffer
		ID3D11Texture2D* backBufferPtr = nullptr;
		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
		if (FAILED(result))
		{
			//result = false;
			return false;
		}

		// use the back buffer address to create the render target
		device->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView);
		if (FAILED(result))
		{
			//result = false;
			return false;
		}
		backBufferPtr->Release();
		backBufferPtr = nullptr;

		
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = (UINT)screenWidth;
		descDepth.Height = (UINT)screenHeight;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //DXGI_FORMAT_R32_TYPELESS;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;// | D3D10_BIND_SHADER_RESOURCE;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		result = device->CreateTexture2D(&descDepth, NULL, &depthStencilBuffer);
		if (FAILED(result))
		{
			return false;
		}
		result = device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView); // &depthStencilViewDesc was NULL
		if (FAILED(result))
		{
			// deal with error...
			return false;
		}
		deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

		//the depth Stencil State
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		// Depth test parameters
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		// Create depth stencil state
		result = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
		if (FAILED(result))
		{
			// deal with error...
			return false;
		}

		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		// Depth test parameters
		depthStencilDesc.DepthEnable = false;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		// Create depth stencil state
		result = device->CreateDepthStencilState(&depthStencilDesc, &depthDisStencilState);
		if (FAILED(result))
		{
			// deal with error...
			return false;
		}

	}

	deviceContext->OMSetDepthStencilState(this->depthStencilState, 0); //1
	//void SetViewport()
	vp.Width = (float)screenWidth;
	vp.Height = (float)screenHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	deviceContext->RSSetViewports(1, &vp);

	//do this later
	//this->gIncrement += 0.8f * ImGui::GetIO().DeltaTime; 
	//gConstantBufferData->offset = sin(this->gIncrement);

	//fieldOfView = 3.141592654f / 4.0f; //0.785
	fieldOfView = 0.45f*DirectX::XM_PI;
	screenAspect = (float)screenWidth / (float)screenHeight;

	orthoMatrix = DirectX::XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight,0.1f, 100.f);
	//move to ColorShader
	this->projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, 0.1f, 100.f);
	//this->projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	//this->worldMatrix = DirectX::XMMatrixIdentity();
	//this->worldMatrix = DirectX::XMMatrixRotationY(this->gIncrement);
	//this->worldMatrix = DirectX::XMMatrixTranspose(worldMatrix); //moved to colorshader
	//this->orthoMatrix = DirectX::XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	this->device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debug));

	return true;
}

void D3D::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (swapChain)
	{
		swapChain->SetFullscreenState(false, NULL);
	}
	//if (rasterState)
	//{
	//	rasterState->Release();
	//	rasterState = nullptr;
	//}
	if (depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = nullptr;
	}
	if (depthStencilState)
	{
		depthStencilState->Release();
		depthStencilState = nullptr;
	}
	if (depthStencilBuffer)
	{
		depthStencilBuffer->Release();
		depthStencilBuffer = nullptr;
	}
	if (renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = nullptr;
	}
	if (deviceContext)
	{
		deviceContext->Release();
		deviceContext = nullptr;
	}

	if (swapChain)
	{
		swapChain->Release();
		swapChain = nullptr;
	}

	debug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);
	if (device)
	{
		device->Release();
		device = nullptr;
	}
	debug->Release();
	
	//return;
}

void D3D::BeginScene(float color[4])
{
	//float color[4];
	// Setup the color to clear the buffer to.
	//color[0] = red;
	//color[1] = green;
	//color[2] = blue;
	//color[3] = alpha;

	// Clear the back buffer.
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	// Clear the depth buffer.
	/*ID3D11BlendState* d3dBlendState;
	D3D11_BLEND_DESC omDesc;
	ZeroMemory(&omDesc,

		sizeof(D3D11_BLEND_DESC));
	omDesc.RenderTarget[0].BlendEnable =

		true;
	omDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	omDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	omDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	omDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	omDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (d3dBlendState)
	{
		d3dBlendState->Release();
	}*/
	//device->CreateBlendState(&omDesc, &d3dBlendState);
	//deviceContext->OMSetBlendState(nullptr, 0, 0xffffffff);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//this->worldMatrix = DirectX::XMMatrixRotationY(this->gIncrement);//to here
}

void D3D::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if (vSync_enabled == true)
	{
		// Lock to screen refresh rate.
		swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		swapChain->Present(0, 0);
	}
}

void D3D::setWorld(DirectX::XMMATRIX world)
{
	this->worldMatrix = world;
}

void D3D::setBackBuffer()
{
	deviceContext->OMSetRenderTargets(1, &renderTargetView,this->depthStencilView);
}

void D3D::turnOffZ()
{
	deviceContext->OMSetDepthStencilState(this->depthDisStencilState, 1); //1
}

void D3D::turnOnZ()
{
	deviceContext->OMSetDepthStencilState(this->depthStencilState, 1); //1
}

void D3D::resetViewPort()
{
	deviceContext->RSSetViewports(1, &vp);
}

ID3D11Device *& D3D::GetDevice()
{
	return device;
}

ID3D11DeviceContext *& D3D::GetDeviceContext()
{
	return deviceContext;
}

DirectX::XMMATRIX& D3D::GetProjectionMatrix()
{
	return projectionMatrix;
}

DirectX::XMMATRIX & D3D::GetWorldMatrix()
{
	return  worldMatrix;
}

DirectX::XMMATRIX & D3D::GetOrthoMatrix()
{
	return orthoMatrix;
}

int & D3D::GetVideoCardInfo(char * cardName)//, int & memory)
{
	strcpy_s(cardName, 128, this->videoCardDescription);
	return videoCardMemory;
	//return;
}

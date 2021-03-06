#include"SkyBox.h"

SkyBox::SkyBox()
{
	this->constantBuffer = nullptr;
	this->samplerState = nullptr;
	this->vertexBuffer = nullptr;
	this->cube = new Vertex[8];
	this->index = new int[36];

	Vertex cubeTemp[] =
	{
		-1.0f,  1.0f, -1.0f,1.0f,
		-1.0f, -1.0f, -1.0f,1.0f,
		 1.0f, -1.0f, -1.0f,1.0f,
		 1.0f,  1.0f, -1.0f,1.0f,
		-1.0f, -1.0f,  1.0f,1.0f,
		-1.0f,  1.0f,  1.0f,1.0f,
		 1.0f, -1.0f,  1.0f,1.0f,
		 1.0f,  1.0f,  1.0f,1.0f
	};

	int indices[] = { 0, 1, 2, 2, 3, 0, 4, 1, 0, 0, 5, 4, 2, 6, 7, 7, 3, 2, 4, 5, 7, 7, 6, 4, 0, 3, 7, 7, 5, 0, 1, 4, 2, 2, 4, 6 };

	//all the vertices inserted into cubetemp then into this->cube
	for (int i = 0; i < 8; i++)
	{
		this->cube[i] = cubeTemp[i];
	}

	for (int i = 0; i < 36; i++)
	{
		this->index[i] = indices[i];
	}
}

SkyBox::~SkyBox()
{
}

void SkyBox::shutDown()
{
	for (int i = 0; i < 6; i++)
	{
		this->textureLoad[i].Shutdown();
	}
	this->constantBuffer->Release();
	delete[] this->cube;
	delete[] this->index;
	this->cubeSRV->Release();
	this->cubeTex->Release();
	//this->textureLoad->Shutdown();
	if (this->WVPdata)
	{
		_aligned_free(this->WVPdata);
	}
	this->ps->Release();
	this->vs->Release();
	this->samplerState->Release();
	this->indexBuffer->Release();
	this->vertexBuffer->Release();
	this->vertexLayout->Release();
}

bool SkyBox::initialize(ID3D11DeviceContext * deviceContext, ID3D11Device* device, std::string file)
{
	WVPdata = (Matrices*)_aligned_malloc(sizeof(Matrices), 16);
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(Matrices);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA pData;
	ZeroMemory(&pData, sizeof(pData));
	pData.pSysMem = WVPdata;
	pData.SysMemPitch = 0;
	pData.SysMemSlicePitch = 0;

	HRESULT hr;
	hr = device->CreateBuffer(&desc, &pData, &constantBuffer);
	if (FAILED(hr))
	{
		// deal with error...
		return false;
	}
	if (!this->initializeTexture(deviceContext, device, file))
	{
		return false;
	}
	if (!this->initializeShaders(device))
	{
		return false;
	}
	if (!this->initializeVertex(device))
	{
		return false;
	}
	return true;
}

bool SkyBox::render(ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj)
{

	setShaderParams(deviceContext, world, view, proj);
	//Setvertexbandtexture
	UINT32 vertexSize = sizeof(Vertex);

	UINT32 offset = 0;
	deviceContext->PSSetShaderResources(0, 1, &this->cubeSRV);
	deviceContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->PSSetSamplers(0, 1, &this->samplerState);
	
	//render shader
	deviceContext->VSSetShader(this->vs, nullptr, 0);
	deviceContext->HSSetShader(nullptr, nullptr, 0);
	deviceContext->DSSetShader(nullptr, nullptr, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->PSSetShader(this->ps, nullptr, 0);
	deviceContext->IASetInputLayout(this->vertexLayout);

	deviceContext->DrawIndexed(36, 0, 0);
	//36 for 6 faces with 6 vertecies for each face 6*6=36
	return true;
}

bool SkyBox::initializeTexture(ID3D11DeviceContext * deviceContext, ID3D11Device* device, std::string file)
{
	
	std::string fileName = "SkyBoxes/" + file + "_rt.tga";//right
	textureLoad[0].Initialize(device, deviceContext, fileName.c_str());
	fileName = "SkyBoxes/" + file + "_lf.tga";//left
	textureLoad[1].Initialize(device, deviceContext, fileName.c_str());
	fileName = "SkyBoxes/" + file + "_up.tga";//up
	textureLoad[2].Initialize(device, deviceContext, fileName.c_str());
	fileName = "SkyBoxes/" + file + "_dn.tga";//down
	textureLoad[3].Initialize(device, deviceContext, fileName.c_str());
	fileName = "SkyBoxes/" + file + "_bk.tga";//back
	textureLoad[4].Initialize(device, deviceContext, fileName.c_str());
	fileName = "SkyBoxes/" + file + "_ft.tga";//forward
	textureLoad[5].Initialize(device, deviceContext, fileName.c_str());
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = textureLoad[0].getWidth();//Every texture has the same width and height
	texDesc.Height = textureLoad[0].getHeight();
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 6;//6 for all the faces of a cube
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.CPUAccessFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;//TextureCube
	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = texDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = texDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	D3D11_SUBRESOURCE_DATA pData[6];
	for (int cubeMapFaceIndex = 0; cubeMapFaceIndex < 6; cubeMapFaceIndex++)
	{
		pData[cubeMapFaceIndex].pSysMem = this->textureLoad[cubeMapFaceIndex].getTextureCharArray();
		pData[cubeMapFaceIndex].SysMemPitch = this->textureLoad[cubeMapFaceIndex].getWidth() * 4;
		pData[cubeMapFaceIndex].SysMemSlicePitch = 0;
	}
	HRESULT hr = device->CreateTexture2D(&texDesc,
		pData, &cubeTex);
	if (FAILED(hr))
	{
		return false;
	}
	hr = device->CreateShaderResourceView(
		cubeTex, &SMViewDesc, &cubeSRV);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool SkyBox::initializeShaders(ID3D11Device* device)
{
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"SkyVertexShader.hlsl", // filename vsFilename
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
			//OutputShaderErrorMessage(errorBlob, hwnd, vsFilename); //able when parameter active
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		else
		{
			//MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK); //able when parameter active
		}
		if (pVS)
			pVS->Release();
		return false;
	}

	device->CreateVertexShader(
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		nullptr,
		&vs
	);

	// create input layout (verified using vertex shader)
	// Press F1 in Visual Studio with the cursor over the datatype to jump
	// to the documentation online!
	// please read:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb205117(v=vs.85).aspx
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"POSITION",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32A32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			D3D11_APPEND_ALIGNED_ELEMENT, // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		}

	};
	result = device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &vertexLayout);

	if (FAILED(result))
	{
		return false;
	}
	// we do not need anymore this COM object, so we release it.
	pVS->Release();

	//create pixel shader
	ID3DBlob* pPS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"SkyFragment.hlsl", // filename
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
		return false;
	}

	device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &ps);
	// we do not need anymore this COM object, so we release it.
	pPS->Release();

	

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &this->samplerState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool SkyBox::initializeVertex(ID3D11Device* device)
{
	D3D11_SUBRESOURCE_DATA data;
	HRESULT hr;
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = 8 * sizeof(Vertex);
	data.pSysMem = this->cube;
	hr = device->CreateBuffer(&bufferDesc, &data, &this->vertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	D3D11_SUBRESOURCE_DATA pData;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = 36 * sizeof(int);
	pData.pSysMem = this->index;
	hr = device->CreateBuffer(&bufferDesc, &pData, &this->indexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	return true;	
}

bool SkyBox::setShaderParams(ID3D11DeviceContext* deviceContext,DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);
	DirectX::XMMATRIX WorldView = DirectX::XMMatrixMultiply(view, world);
	DirectX::XMMATRIX WorldViewProj = DirectX::XMMatrixMultiply(proj, WorldView);
	WorldViewProj = DirectX::XMMatrixTranspose(WorldViewProj);
	this->WVPdata->world = world;
	this->WVPdata->view = DirectX::XMMatrixTranspose(WorldView);
	this->WVPdata->projection = WorldViewProj;
	//Lock the m_matrixBuffer, set the new matrices inside it, and then unlock it.

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	if (FAILED(result))
	{
		return false;
	}
	// Get a pointer to the data in the constant buffer.
	memcpy(mappedMemory.pData, this->WVPdata, sizeof(Matrices));

	// Unlock the constant buffer.
	deviceContext->Unmap(constantBuffer, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer); //Set the constantbuffer in vs

	return true;
}

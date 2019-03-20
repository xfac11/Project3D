#include"SkyBox.h"

SkyBox::SkyBox()
{

}

SkyBox::~SkyBox()
{
}

void SkyBox::shutDown()
{
}

void SkyBox::initialize()
{
}

bool SkyBox::render(ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj)
{
	return false;
}

void SkyBox::initializeTexture(ID3D11DeviceContext * deviceContext, ID3D11Device* device, std::string file)
{
	std::string fileName = file + "_ft";//forward
	textureLoad[0].Initialize(device, deviceContext, fileName.c_str());
	fileName = file + "_lf";//left
	textureLoad[1].Initialize(device, deviceContext, fileName.c_str());
	fileName = file + "_rt";//right
	textureLoad[2].Initialize(device, deviceContext, fileName.c_str());
	fileName = file + "_bk";//back
	textureLoad[3].Initialize(device, deviceContext, fileName.c_str());
	fileName = file + "_up";//up
	textureLoad[4].Initialize(device, deviceContext, fileName.c_str());
	fileName = file + "_dn";//down
	textureLoad[5].Initialize(device, deviceContext, fileName.c_str());
	D3D11_TEXTURE3D_DESC texDesc;
	texDesc.Width = textureLoad[0].getWidth();//Every texture has the same width and height
	texDesc.Height = textureLoad[0].getHeight();
	texDesc.MipLevels = 1;
	texDesc.Depth=textureLoad[0].getHeight();
	//texDesc.ArraySize = 6;//6 for all the faces of a cube
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.CPUAccessFlags = 0;
	//texDesc.SampleDesc.Count = 1;
	//texDesc.SampleDesc.Quality = 0;
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
	//std::vector<vector4b> d[6]; // 6 images of type vector4b = 4 * unsigned char

	for (int cubeMapFaceIndex = 0; cubeMapFaceIndex < 6; cubeMapFaceIndex++)
	{
		//d[cubeMapFaceIndex].resize(description.width * description.height);

		// fill with red color  
		/*std::fill(
			d[cubeMapFaceIndex].begin(),
			d[cubeMapFaceIndex].end(),
			vector4b(255, 0, 0, 255));*/
		pData[cubeMapFaceIndex].pSysMem = this->textureLoad[cubeMapFaceIndex].getTextureCharArray();//&d[cubeMapFaceIndex][0];// description.data;
		pData[cubeMapFaceIndex].SysMemPitch = this->textureLoad[cubeMapFaceIndex].getWidth() * 4;
		pData[cubeMapFaceIndex].SysMemSlicePitch = 0;
	}

	HRESULT hr = device->CreateTexture3D(&texDesc,
		pData, &cubeTex);
	//assert(hr == S_OK);

	hr = device->CreateShaderResourceView(
		cubeTex, &SMViewDesc, &cubeSRV);
	//assert(hr == S_OK);
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
			"SV_POSITION",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32A32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			D3D11_APPEND_ALIGNED_ELEMENT, // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT, //2 values
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
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
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
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

void SkyBox::initializeVertex()
{
}

bool SkyBox::setShaderParams(DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	D3D11_MAPPED_SUBRESOURCE mappedMemorySpec;
	//	MatrixBufferType* dataPtr;
		//PerFrameMatrices* matricesPerFrame;
		//unsigned int bufferNumber;

		//Make sure to transpose matrices before sending them into the shader, this is a requirement for DirectX 11.

			// Transpose the matrices to prepare them for the shader.
	//worldMatrix = DirectX::XMMatrixIdentity();
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);
	DirectX::XMMATRIX WorldView = DirectX::XMMatrixMultiply(view, world);
	DirectX::XMMATRIX WorldViewProj = DirectX::XMMatrixMultiply(proj, WorldView);
	WorldViewProj = DirectX::XMMatrixTranspose(WorldViewProj);
	this->WVPdata->world = world;
	this->WVPdata->view = DirectX::XMMatrixTranspose(WorldView);
	this->WVPdata->projection = WorldViewProj;
	dataSpec->specularAlbedo = specularAlbedo;
	dataSpec->specularPower = specularPower;
	//Lock the m_matrixBuffer, set the new matrices inside it, and then unlock it.

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(MatrixPerFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	memcpy(mappedMemory.pData, this->WVPdata, sizeof(MatrixBuffers));

	// Unlock the constant buffer.
	deviceContext->Unmap(MatrixPerFrameBuffer, 0);

	result = deviceContext->Map(SpecPerFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemorySpec);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	memcpy(mappedMemorySpec.pData, dataSpec, sizeof(Specular));// use this and the flickering stops

	// Unlock the constant buffer.
	deviceContext->Unmap(SpecPerFrameBuffer, 0);

	//Now set the updated matrix buffer in the HLSL vertex shader.

		// Set the position of the constant buffer in the vertex shader.
	//bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	//deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	//deviceContext->PSSetConstantBuffers(0, 1, &ConstantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &SpecPerFrameBuffer);
	deviceContext->PSSetConstantBuffers(1, 1, &MatrixPerFrameBuffer);
	deviceContext->GSSetConstantBuffers(0, 1, &MatrixPerFrameBuffer);
	deviceContext->VSSetConstantBuffers(0, 1, &MatrixPerFrameBuffer); //could have bufferNumber = 0 dno why tho
	//deviceContext->GSSetConstantBuffers(0, 1, &MatrixPerFrameBuffer);


	return true;
}

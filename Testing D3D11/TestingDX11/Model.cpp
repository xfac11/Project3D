#include "Model.h"

Model::Model()
{
	this->vertexBuffer = nullptr;
	//this->indexBuffer = nullptr;
	this->constantBuffer = nullptr;
	this->SamplerState = nullptr;
	//this->indexCount = 0;
	this->vertexCount = 0;
}

Model::~Model()
{

}

bool Model::addQuads(DirectX::XMFLOAT3 pos, float width, float height, float depth, int face)
{//Will change vertexbuffer and vertexcount
	return quads.addQuad(pos, width, height, depth, face, this->vertexCount);
	this->vertexCount += 6;
}

bool Model::addCube(DirectX::XMFLOAT3 pos, float width, float height, float depth)
{
	this->cubes.addCube(pos, width, height, depth);
	return true;
}

bool Model::addTri(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3)
{
	return quads.addTri(p1, p2, p3, this->vertexCount);
}

void Model::shutdown()
{
	if (this->vertexBuffer != nullptr)
		this->vertexBuffer->Release();
	//if (this->indexBuffer != nullptr)
		//this->indexBuffer->Release();
	if (this->constantBuffer != nullptr)
		this->constantBuffer->Release();
	if (this->SamplerState != nullptr)
		this->SamplerState->Release();
	this->texture.cleanUp();
}

bool Model::createTheVertexBuffer(ID3D11Device *& gDevice)
{
	return this->cubes.insertVertexBuffer(gDevice, this->vertexBuffer, this->vertexCount);
	//return this->quads.createVertexBuffer(gDevice, this->vertexBuffer, this->vertexCount);
}

void Model::setVertexBuffer(ID3D11DeviceContext *& gDeviceContext)
{
	UINT32 vertexSize = sizeof(Vertex3D);

	UINT32 offset = 0;
	gDeviceContext->PSSetShaderResources(0, 1, &this->texture.getTexture());
	gDeviceContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &vertexSize, &offset);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//gDeviceContext->GSSetShader(nullptr, nullptr, 0); //already initilized?+
}

int Model::getVertexCount() const
{
	return this->vertexCount;
}

void Model::setTheTexture(ID3D11Device *& gDevice, ID3D11DeviceContext *&gDeviceContext, char* filename)
{
	this->texture.setTexture(gDevice, gDeviceContext, filename);
}

void Model::setSampler(ID3D11Device*& gDevice)
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
	HRESULT hr = gDevice->CreateSamplerState(&desc, &this->SamplerState);
	if (FAILED(hr))
	{
		//MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", "error", MB_OK);
		//deal with error. Log it maybe
		
	}
}
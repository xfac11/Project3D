#include "Model.h"

Model::Model()
{
	this->vertexBuffer = nullptr;
	//this->indexBuffer = nullptr;
	this->constantBuffer = nullptr;
	this->SamplerState = nullptr;
	//this->indexCount = 0;
	this->rot = 0;
	this->moveM = 0;
	this->vertexCount = 0;
	DirectX::XMVECTOR rotaxis = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMMATRIX rotTemp = DirectX::XMMatrixRotationAxis(rotaxis, 0);
	DirectX::XMMATRIX scaleTemp = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	DirectX::XMMATRIX translTemp = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&this->Rotation,rotTemp);
	DirectX::XMStoreFloat4x4(&this->Scale, scaleTemp);
	DirectX::XMStoreFloat4x4(&this->Translation, translTemp);
	this->position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	scaleTemp = DirectX::XMLoadFloat4x4(&this->Scale);
	translTemp = DirectX::XMLoadFloat4x4(&this->Translation);
}

Model::~Model()
{

}

bool Model::addQuads(DirectX::XMFLOAT3 pos, float width,float height,float depth,int face)
{//Will change vertexbuffer and vertexcount
	this->vertexCount += 6;
	return quads.addQuad(pos, width, height,depth,face,this->vertexCount);
}

bool Model::addCube(DirectX::XMFLOAT3 pos, float width, float height, float depth)
{
	this->cubes.addCube(pos, width, height, depth);
	return true;
}

bool Model::addTri(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3)
{
	return quads.addTri(p1, p2, p3,this->vertexCount);
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

bool Model::insertCubesInVec()
{
	return this->cubes.insertVector(body, this->vertexCount);
}

void Model::moveCube(int id)
{
	//this->cubes.moveCube(id);
	//insertCubesInVec();
}

bool Model::createTheVertexBuffer(ID3D11Device *& gDevice)
{
	unsigned int nrOfVertex = body.size();
	this->vertexCount = nrOfVertex;
	Vertex3D *temp = new Vertex3D[nrOfVertex];
	int vertices = 0;
	for (int i = 0; i < nrOfVertex; i++)
	{
		temp[i] = body.at(i);
	}
	D3D11_SUBRESOURCE_DATA data;
	HRESULT hr;
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = nrOfVertex * sizeof(Vertex3D);
	data.pSysMem = temp;
	hr = gDevice->CreateBuffer(&bufferDesc, &data, &this->vertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
//Sets texture in PS and vertex buffer for input
void Model::setVertexBandTexture(ID3D11DeviceContext *& gDeviceContext)
{
	UINT32 vertexSize = sizeof(Vertex3D);

	UINT32 offset = 0;
	gDeviceContext->PSSetShaderResources(0, 1, &this->texture.getTexture());
	gDeviceContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &vertexSize, &offset);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->PSSetSamplers(0, 1, &this->SamplerState);
	//gDeviceContext->GSSetShader(nullptr, nullptr, 0); //already initilized?+
}

int Model::getVertexCount() const
{
	return this->vertexCount;
}

void Model::setTheTexture( ID3D11Device *& gDevice, ID3D11DeviceContext *&gDeviceContext,std::string fileName)
{
	this->texture.setTexture(gDevice, gDeviceContext,fileName);
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
		//deal with error. Log it maybe
	}
}

void Model::loadOBJ(char * file, ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	std::string txt = load.loadFile(file, body);
	this->texture.setTexture(device, deviceContext, txt);
	this->vertexCount += body.size();
}

void Model::draw(ColorShader & shader, ID3D11DeviceContext * deviceContext)
{
	this->setVertexBandTexture(deviceContext);
	//Maybe shader.Render so the worldmatrix and all that updates everytime a new object gets drawn?
	shader.RenderShader(deviceContext, this->vertexCount);
}

void Model::rotate(float angle)
{
	//this->world = DirectX::XMMatrixRotationY(angle);
}

DirectX::XMFLOAT4X4 Model::getId()
{
	return this->world;
}

void Model::setWorld(DirectX::XMMATRIX mtrx)
{
	DirectX::XMStoreFloat4x4(&this->world, mtrx);
	//this->world = mtrx;
}

void Model::setWorld()
{
	DirectX::XMMATRIX rotTemp;
	DirectX::XMMATRIX scaleTemp;
	DirectX::XMMATRIX translTemp;
	rotTemp=DirectX::XMLoadFloat4x4(&this->Rotation);
	scaleTemp = DirectX::XMLoadFloat4x4(&this->Scale);
	translTemp = DirectX::XMLoadFloat4x4(&this->Translation);
	DirectX::XMStoreFloat4x4(&this->world,(translTemp*rotTemp*scaleTemp));
	
}

void Model::setPosition(float x, float y, float z)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
	DirectX::XMMATRIX tempTransl = DirectX::XMMatrixTranslation(this->position.x, this->position.y, this->position.z);
	DirectX::XMStoreFloat4x4(&this->Translation, tempTransl);
}

void Model::move(float x, float y, float z)
{
	this->position.x += x;
	this->position.y += y;
	this->position.z += z;
	DirectX::XMMATRIX tempTransl = DirectX::XMMatrixTranslation(this->position.x, this->position.y, this->position.z);
	DirectX::XMStoreFloat4x4(&this->Translation, tempTransl);
}

void Model::rotate(DirectX::XMVECTOR axis, float angle)
{
	DirectX::XMMATRIX tempRot = DirectX::XMMatrixRotationAxis(axis,angle);
	DirectX::XMStoreFloat4x4(&this->Rotation, tempRot);
}

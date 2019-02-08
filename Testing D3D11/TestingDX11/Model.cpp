#include "Model.h"

Model::Model()
{
	this->vertexBuffer = nullptr;
	this->indexBuffer = nullptr;
	this->constantBuffer = nullptr;
	this->indexCount = 0;
	this->vertexCount = 0;
}

Model::~Model()
{

}

void Model::addQuads(Vertex3D newQuad[6], ID3D11Device *& gDevice)
{
	quads.addQuad(newQuad, gDevice, this->vertexBuffer,this->vertexCount);
}

void Model::shutdown()
{
	if(this->vertexBuffer!=nullptr)
		this->vertexBuffer->Release();
	if(this->indexBuffer!=nullptr)
		this->indexBuffer->Release();
	if(this->constantBuffer!=nullptr)
		this->constantBuffer->Release();
}

void Model::setVertexBuffer(ID3D11DeviceContext *& gDeviceContext)
{
	UINT32 vertexSize = sizeof(Vertex3D);

	UINT32 offset = 0;
	gDeviceContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &vertexSize, &offset);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//gDeviceContext->GSSetShader(nullptr, nullptr, 0); //already initilized?+
}

int Model::getVertexCount() const
{
	return this->vertexCount;
}

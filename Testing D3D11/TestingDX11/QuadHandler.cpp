#include "QuadHandler.h"

QuadHandler::QuadHandler()
{
	this->nrOfVertex = 0;
	this->CAP = 6;
	this->CAPVertex = 12;
	this->nrOfQuads = 0;
	this->Quads = new QuadVertex*[CAP];
	for (int i = 0; i < CAP; i++)
	{
		this->Quads[i] = nullptr;
	}
}

QuadHandler::~QuadHandler()
{
	for (int i = 0; i < nrOfQuads; i++)
	{
		delete this->Quads[i];
	}
	delete[] this->Quads;
	//delete[] this->quadVertex;
}

bool QuadHandler::addQuad(Vertex3D newQuad[6], ID3D11Device *& gDevice, ID3D11Buffer *& gVertexBuffer, int & vertexCount)
{
	bool result = false;
	if (nrOfQuads < CAP&&nrOfVertex < CAPVertex)
	{
		result = true;
		Quads[nrOfQuads] = new QuadVertex(newQuad, gDevice, gVertexBuffer, vertexCount);
		for (int i = 0; i < 6; i++)
		{
			this->quadVertex[nrOfVertex]=Quads[nrOfQuads]->getQuads(i);
			nrOfVertex++;
		}
		nrOfQuads++;
	}
	
	return result;
}

bool QuadHandler::createVertexBuffer(ID3D11Device *& gDevice, ID3D11Buffer *& gVertexBuffer, int & vertexCount)
{
	D3D11_SUBRESOURCE_DATA data;
	HRESULT hr;
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(this->quadVertex);
	data.pSysMem = this->quadVertex;
	hr=gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

UINT32 QuadHandler::getSize(int id)
{
	return Quads[id]->getSize();
}

QuadVertex *& QuadHandler::getQuad(int id)
{
	return  Quads[id];
}

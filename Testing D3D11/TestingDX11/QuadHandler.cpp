#include "QuadHandler.h"

QuadHandler::QuadHandler()
{
	this->CAP = 6;
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
}

bool QuadHandler::addQuad(Vertex3D newQuad[6], ID3D11Device *& gDevice, ID3D11Buffer *& gVertexBuffer, int & vertexCount)
{
	bool result = false;
	if (nrOfQuads < CAP)
	{
		result = true;
		Quads[nrOfQuads] = new QuadVertex(newQuad, gDevice, gVertexBuffer, vertexCount);
		nrOfQuads++;
	}
	return result;
}

UINT32 QuadHandler::getSize(int id)
{
	return Quads[id]->getSize();
}

QuadVertex *& QuadHandler::getQuad(int id)
{
	return  Quads[id];
}
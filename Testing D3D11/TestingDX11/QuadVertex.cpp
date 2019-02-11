#include "QuadVertex.h"

void QuadVertex::update(ID3D11Device *& gDevice, ID3D11Buffer *& gVertexBuffer)
{
	
	
}

QuadVertex::QuadVertex(Vertex3D newQuad[6], ID3D11Device *& gDevice, ID3D11Buffer *& gVertexBuffer, int & vertexCount)
{
	Quad[0] = newQuad[0];
	vertexCount++;
	Quad[1] = newQuad[1];
	vertexCount++;
	Quad[2] = newQuad[2];
	vertexCount++;
	Quad[3] = newQuad[3];
	vertexCount++;
	Quad[4] = newQuad[4];
	vertexCount++;
	Quad[5] = newQuad[5];
	vertexCount++;


	
}

QuadVertex::~QuadVertex()
{
}

UINT32 QuadVertex::getSize()const
{
	return sizeof(Quad);
}

Vertex3D QuadVertex::getQuads(int id)
{
	return this->Quad[id];
}

void QuadVertex::setColor(int id, float rgb[3], ID3D11Device *& gDevice, ID3D11Buffer *& gVertexBuffer)
{
	//Quad[id].r = rgb[0];
	//Quad[id].b = rgb[1];
	//Quad[id].g = rgb[2];
	//Quad[id].t = rgb[3];

	update(gDevice, gVertexBuffer);
}
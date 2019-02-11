#ifndef QUADHANDLER_H
#define QUADHANDLER_H
#include "QuadVertex.h"
class QuadHandler
{
private:
	int CAP;
	int CAPVertex;
	int nrOfQuads;
	QuadVertex* *Quads;
	Vertex3D quadVertex[12];
	int nrOfVertex;
public:
	QuadHandler();
	~QuadHandler();
	bool addQuad(Vertex3D newQuad[6], ID3D11Device *& gDevice, ID3D11Buffer *& gVertexBuffer, int & vertexCount);
	bool createVertexBuffer(ID3D11Device *& gDevice, ID3D11Buffer *& gVertexBuffer, int & vertexCount);
	//void update(ID3D11DeviceContext* gDeviceContext, ID3D11Buffer *& gVertexBuffer, UINT32 & vertexSize, UINT32 & offset, ID3D11InputLayout* gVertexLayout);
	UINT32 getSize(int id);

	QuadVertex *& getQuad(int id);
};
#endif
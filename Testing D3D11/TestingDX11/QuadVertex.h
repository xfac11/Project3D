#ifndef QUADVERTEX_H
#define QUADVERTEX_H
#include "Vertex3D.h"
#include <d3d11.h>
class QuadVertex
{
private:
	Vertex3D Quad[6];
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA data;
	void update(ID3D11Device *& gDevice, ID3D11Buffer *& gVertexBuffer);
public:
	QuadVertex(Vertex3D newQuad[6], ID3D11Device *& gDevice, ID3D11Buffer *& gVertexBuffer,int & vertexCount);
	~QuadVertex();
	UINT32 getSize()const;
	Vertex3D getQuads(int id);
	void setColor(int id, float rgb[3], ID3D11Device *& gDevice, ID3D11Buffer *& gVertexBuffer);
};
#endif
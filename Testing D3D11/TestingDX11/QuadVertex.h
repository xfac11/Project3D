#ifndef QUADVERTEX_H
#define QUADVERTEX_H
#include"Triangle.h"
#include <d3d11.h>
class QuadVertex
{
private:
	Triangle quad[2];
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA data;
	void update(ID3D11Device *& gDevice, ID3D11Buffer *& gVertexBuffer);
public:
	QuadVertex();
	QuadVertex(DirectX::XMFLOAT3 pos,float width,float height,float depth ,int face);
	~QuadVertex();
	UINT32 getSize()const;
	Triangle &getTri(int id);
	void move(float x, float y, float z);
	void setRotationX(float angle);
	void setRotationY(float angle);
	void setRotationZ(float angle);
	void setColor(int id, float rgb[3], ID3D11Device *& gDevice, ID3D11Buffer *& gVertexBuffer);
};
#endif
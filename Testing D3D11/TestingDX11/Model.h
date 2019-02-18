#ifndef MODEL_H
#define MODEL_H
#include <d3d11.h>
#include <directxmath.h>
#include"CubeHandler.h"
#include"QuadHandler.h"
#include"Texture.h"
class Model
{
public:
	Model();
	~Model();
	//This before setVertexBuffer
	bool addQuads(DirectX::XMFLOAT3 pos, float width, float height, float depth,int face);
	bool addCube(DirectX::XMFLOAT3 pos, float width, float height, float depth);
	bool addTri(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3);
	void shutdown();
	bool createTheVertexBuffer(ID3D11Device *& gDevice);
	void setVertexBuffer(ID3D11DeviceContext *& gDeviceContext);
	int getVertexCount()const;
	void setTheTexture( ID3D11Device*& gDevice, ID3D11DeviceContext *&gDeviceContext, char* filename);
	void setSampler(ID3D11Device*& gDevice);
private:
	ID3D11Buffer *vertexBuffer;
	//ID3D11Buffer *indexBuffer;
	ID3D11Buffer *constantBuffer;
	ID3D11SamplerState* SamplerState;

	Texture texture;
	int vertexCount;
	//int indexCount;
	QuadHandler quads;
	CubeHandler cubes;
};
#endif // !MODEL_H
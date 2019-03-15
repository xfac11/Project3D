#ifndef MODEL_H
#define MODEL_H
#include <d3d11.h>
#include <directxmath.h>
#include"CubeHandler.h"
#include<vector>
#include"QuadHandler.h"
#include"Texture.h"
#include "Loader.h"
#include"DefferedShader.h"
#include"ColorShader.h"
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
	bool insertCubesInVec();
	void moveCube(int id);
	bool createTheVertexBuffer(ID3D11Device *& gDevice);
	void setVertexBandTexture(ID3D11DeviceContext *& gDeviceContext);
	int getVertexCount()const;
	void setTheTexture( ID3D11Device*& gDevice, ID3D11DeviceContext *&gDeviceContext, std::string fileName);
	void setSampler(ID3D11Device*& gDevice);
	void loadOBJ(char* file, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void draw(DefferedShader &shader, ID3D11DeviceContext * deviceContext);
	void draw(ColorShader &shader, ID3D11DeviceContext * deviceContext);
	void rotate(float angle);
	DirectX::XMFLOAT4X4 getId();
	void setWorld(DirectX::XMMATRIX mtrx);
	void setWorld();
	void setPosition(float x, float y, float z);
	DirectX::XMFLOAT3 getPosition();
	void move(float x, float y, float z);
	void rotate(DirectX::XMVECTOR axis, float angle);
	void setScale(float x, float y, float z);
	float rot;
	float moveM;
private:
	ID3D11Buffer *vertexBuffer;
	//ID3D11Buffer *indexBuffer;
	ID3D11Buffer *constantBuffer;
	ID3D11SamplerState* SamplerState;
	std::vector<Vertex3D> body;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 Rotation;
	DirectX::XMFLOAT4X4 Scale;
	DirectX::XMFLOAT4X4 Translation;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	Texture texture;
	Loader load;
	int vertexCount;
	//int indexCount;
	QuadHandler quads;
	CubeHandler cubes;

};
#endif // !MODEL_H

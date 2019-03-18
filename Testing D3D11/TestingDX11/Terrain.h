#ifndef TERRAIN_H
#define TERRAIN_H
#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
#include "Vertex3D.h"
#include "ColorShader.h"
#include "DeferedShader.h"
#include "Texture.h"
class Terrain
{
private:
	struct HeightMapType
	{
		float x, y, z;
	};

	//struct ModelType
	//{
	//	float x, y, z;
	//};
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 Rotation;
	DirectX::XMFLOAT4X4 Scale;
	DirectX::XMFLOAT4X4 Translation;
	DirectX::XMFLOAT3 position;

	Texture texture;
	//grid stuff
	int terrainWidth;
	int terrainHeight;
	int vertexCount;
	int indexCount;
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *indexBuffer;
	ID3D11SamplerState* SamplerState;
	bool InitializeBuffers(ID3D11Device*& device);


	//height stuff
	float heightScale;
	char* terrainFilename;
	HeightMapType* heightMap;
	Vertex3D* terrainModel;


	bool LoadSetupFile(char* filename);
	bool LoadBitmapHeightMap();
	void ShutdownHeightMap();
	void SetTerrainCoordinates();
	bool BuildTerrainModel();
	void ShutdownTerrainModel();

public:
	Terrain();
	~Terrain();
	bool Initialize(ID3D11Device*& device, char* fileName);
	void Render(ColorShader & shader, ID3D11DeviceContext* deviceContext);
	void Render(DeferedShader & shader, ID3D11DeviceContext* deviceContext);
	//int GetIndexCount();
	void setTheTexture(ID3D11Device *& gDevice, ID3D11DeviceContext *&gDeviceContext, char* filename);
	void Shutdown();

	void setWorld();
	DirectX::XMFLOAT4X4 getWorld();
	void setSampler(ID3D11Device*& gDevice);
};
#endif // !TERRAIN_H

#ifndef TERRAIN_H
#define TERRAIN_H
#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
#include <vector>
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

	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 Rotation;
	DirectX::XMFLOAT4X4 Scale;
	DirectX::XMFLOAT4X4 Translation;
	DirectX::XMFLOAT3 position;

	Texture texture;
	Texture normal;
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
	std::vector<Vertex3D> body;
	//Vertex3D* terrainModel;


	bool LoadSetupFile(char* filename);
	bool LoadBitmapHeightMap();

	void SetTerrainCoordinates();
	bool BuildTerrainModel();
	

	void calculateModelVectors();     //NORMAL MAPS
	void calculateTangentBinormal(NM_Vertex vertex1, NM_Vertex vertex2, NM_Vertex vertex3, DirectX::XMFLOAT3& tangent, DirectX::XMFLOAT3& binormal, DirectX::XMFLOAT3& normal);
	void calculateNormal(DirectX::XMFLOAT3 tangent, DirectX::XMFLOAT3 binormal, DirectX::XMFLOAT3 & normal);     //NORMAL MAPS


public:
	Terrain();
	~Terrain();
	bool Initialize(ID3D11Device*& device, char* fileName);
	//void Render(ColorShader & shader, ID3D11DeviceContext* deviceContext);
	void Render(DeferedShader & shader, ID3D11DeviceContext* deviceContext);
	//int GetIndexCount();
	void setTheTexture(ID3D11Device *& gDevice, ID3D11DeviceContext *&gDeviceContext, std::string filename, std::string normalFileName);
	void Shutdown();

	bool checkCollision(DirectX::XMFLOAT3 camPos);

	void setWorld();
	DirectX::XMFLOAT4X4 getWorld();
	void setSampler(ID3D11Device*& gDevice);
};
#endif // !TERRAIN_H

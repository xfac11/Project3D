#ifndef SKYBOX_H
#define SKYBOX_H

#include"Texture.h"
#include"TextureLoad.h"
#include <d3d11.h>
#include <directxmath.h>
#include"Structs.h"
#include<vector>
#include <d3dcompiler.h>
#define SIZE 1.0f

class SkyBox
{
public:
	SkyBox();
	~SkyBox();
	void shutDown();
	bool initialize(ID3D11DeviceContext * deviceContext, ID3D11Device* device, std::string file);
	bool render(ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj);
private:
	struct Vertex
	{
		float x, y, z, w;//16 
	};
	struct Matrices
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *indexBuffer;
	ID3D11Buffer *constantBuffer;//World, view, proj
	ID3D11PixelShader *ps;
	ID3D11VertexShader *vs;
	ID3D11InputLayout *vertexLayout;
	ID3D11SamplerState* samplerState;
	ID3D11ShaderResourceView* cubeSRV;
	Matrices* WVPdata;
	ID3D11Texture2D* cubeTex;
	TextureLoad textureLoad[6];
	Vertex* cube;
	int* index;
	bool initializeTexture(ID3D11DeviceContext * deviceContext, ID3D11Device* device, std::string file);
	bool initializeShaders(ID3D11Device* device);
	bool initializeVertex(ID3D11Device* device);
	bool setShaderParams(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj);
};
#endif // !SKYBOX_H
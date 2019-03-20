#ifndef SKYBOX_H
#define SKYBOX_H

#include"Texture.h"
#include"TextureLoad.h"
#include <d3d11.h>
#include <directxmath.h>
#include"Structs.h"
#include<vector>
#include <d3dcompiler.h>


class SkyBox
{
public:
	SkyBox();
	~SkyBox();
	void shutDown();
	void initialize();
	bool render(ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj);
private:
	struct Vertex
	{
		DirectX::XMFLOAT4 pos; //16
		DirectX::XMFLOAT3 texCoord; //4*3
		float padding; //4*1
	};
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *constantBuffer;
	ID3D11PixelShader *ps;
	ID3D11VertexShader *vs;
	ID3D11InputLayout *vertexLayout;
	ID3D11SamplerState* samplerState;
	ID3D11ShaderResourceView* cubeSRV;
	MatrixBuffers* WVPdata;
	ID3D11Texture3D* cubeTex;
	TextureLoad textureLoad[6];
	std::vector<Vertex> body;
	void initializeTexture(ID3D11DeviceContext * deviceContext, ID3D11Device* device, std::string file);
	bool initializeShaders(ID3D11Device* device);
	void initializeVertex();
	bool setShaderParams(DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj);
};
#endif // !SKYBOX_H
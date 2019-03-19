#ifndef STRUCTS_H
#define STRUCTS_H
#include <d3d11.h>
#include <DirectXMath.h>
struct PerFrameMatrices
{
	DirectX::XMMATRIX worldMatrix; //16b
	DirectX::XMMATRIX WorldViewProj;
	DirectX::XMFLOAT3 camPos; //12
	float padding;
};
struct CBData //ConstantBufferData
{
	float colour[4];
	float offset;
	float padding1, padding2, padding3;
};
struct MatrixBuffers
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMFLOAT3 camPos; //12
	float padding;
};
struct Specular
{
	DirectX::XMFLOAT3 specularAlbedo;
	float specularPower;
};
enum Direction
{
	Neutral = 0,
	Positive = 1,
	Negative = -1
};
_declspec(align(16))struct PointLight
{
	DirectX::XMMATRIX world;
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 color; //r=x, g=y, b=z, a=w
	DirectX::XMFLOAT4 CameraPos;
};
//struct TextureData
//{
//	unsigned int IMAGE_WIDTH;
//	unsigned int IMAGE_HEIGHT;
//
//	//Image data stored in 8-bit RGBA format
//	unsigned char IMAGE_DATA[16384];
//};
struct NM_Vertex  //normal maps vertex
{
	float x, y, z;
	float u, v;
	float nx, ny, nz;
};
#endif
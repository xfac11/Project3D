#ifndef STRUCTS_H
#define STRUCTS_H
#include <d3d11.h>
#include <DirectXMath.h>
struct PerFrameMatrices
{
	DirectX::XMMATRIX worldMatrix;
};
struct CBData //ConstantBufferData
{
	float colour[4];
	float offset;
	float padding1, padding2, padding3;
};
//struct TextureData
//{
//	unsigned int IMAGE_WIDTH;
//	unsigned int IMAGE_HEIGHT;
//
//	//Image data stored in 8-bit RGBA format
//	unsigned char IMAGE_DATA[16384];
//};
#endif
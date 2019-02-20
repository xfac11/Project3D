#include "QuadVertex.h"

QuadVertex::QuadVertex()
{
}

QuadVertex::QuadVertex(DirectX::XMFLOAT3 pos, float width, float height, float depth, int face)
{
	if (depth == 0)
	{
		DirectX::XMFLOAT3 posUpLeft;
		posUpLeft = DirectX::XMFLOAT3(pos.x, pos.y + height, pos.z);
		DirectX::XMFLOAT3 posDownRight = DirectX::XMFLOAT3(pos.x + width, pos.y, pos.z);
		DirectX::XMFLOAT3 posUpRight = DirectX::XMFLOAT3(pos.x + width, pos.y + height, pos.z);
		if (face == 1)
		{
			this->quad[0] = Triangle(pos, posDownRight, posUpLeft);
			this->quad[1] = Triangle(posUpRight, posUpLeft, posDownRight);
			this->quad[0].setUV(1, 1, 0, 1, 1, 0);
			this->quad[1].setUV(0, 0, 1, 0, 0, 1);
		}
		else if (face == 0)
		{
			this->quad[0] = Triangle(pos, posUpLeft, posDownRight);
			this->quad[1] = Triangle(posUpRight, posDownRight, posUpLeft);
			this->quad[0].setUV(0, 1, 0, 0, 1, 1);
			this->quad[1].setUV(1, 0, 1, 1, 0, 0);
		}

	}
	else if (height == 0)
	{
		DirectX::XMFLOAT3 posUpLeft;
		posUpLeft = DirectX::XMFLOAT3(pos.x, pos.y, pos.z + depth);
		DirectX::XMFLOAT3 posUpRight = DirectX::XMFLOAT3(pos.x + width, pos.y, pos.z + depth);
		DirectX::XMFLOAT3 posDownRight = DirectX::XMFLOAT3(pos.x + width, pos.y, pos.z);
		if (face == 1)
		{

			this->quad[0] = Triangle(pos, posDownRight, posUpLeft);
			this->quad[1] = Triangle(posUpRight, posUpLeft, posDownRight);
			this->quad[0].setUV(1, 1, 0, 1, 1, 0);
			this->quad[1].setUV(0, 0, 1, 0, 0, 1);
		}
		else if (face == 0)
		{
			this->quad[0] = Triangle(pos, posUpLeft, posDownRight);
			this->quad[1] = Triangle(posUpRight, posDownRight, posUpLeft);
			this->quad[0].setUV(0, 1, 0, 0, 1, 1);
			this->quad[1].setUV(1, 0, 1, 1, 0, 0);
		}

	}
	else if (width == 0)
	{
		DirectX::XMFLOAT3 posUpLeft;
		posUpLeft = DirectX::XMFLOAT3(pos.x, pos.y + height, pos.z);
		DirectX::XMFLOAT3 posDownRight = DirectX::XMFLOAT3(pos.x, pos.y, pos.z + depth);
		DirectX::XMFLOAT3 posUpRight = DirectX::XMFLOAT3(pos.x, pos.y + height, pos.z + depth);
		if (face == 1)
		{

			this->quad[0] = Triangle(pos, posDownRight, posUpLeft);
			this->quad[1] = Triangle(posUpRight, posUpLeft, posDownRight);
			this->quad[0].setUV(1, 1, 0, 1, 1, 0);
			this->quad[1].setUV(0, 0, 1, 0, 0, 1);
		}
		else if (face == 0)
		{
			this->quad[0] = Triangle(pos, posUpLeft, posDownRight);
			this->quad[1] = Triangle(posUpRight, posDownRight, posUpLeft);
			this->quad[0].setUV(0, 1, 0, 0, 1, 1);
			this->quad[1].setUV(1, 0, 1, 1, 0, 0);
		}
	}

}

QuadVertex::~QuadVertex()
{
	//delete[]Quad;
}

//UINT32 QuadVertex::getSize()const
//{
//	return sizeof(Quad);
//}

Triangle &QuadVertex::getTri(int id)
{
	return this->quad[id];
}

void QuadVertex::move(float x, float y, float z)
{
	this->quad[0].move(x, y, z);
	this->quad[1].move(x, y, z);
}

void QuadVertex::setRotationX(float angle)
{
	this->quad[0].rotateX(angle);
	this->quad[1].rotateX(angle);
}

void QuadVertex::setRotationY(float angle)
{
	this->quad[0].rotateY(angle);
	this->quad[1].rotateY(angle);
}

void QuadVertex::setRotationZ(float angle)
{
	this->quad[0].rotateZ(angle);
	this->quad[1].rotateZ(angle);
}
#include"Triangle.h"

Triangle::Triangle()
{

}

Triangle::Triangle(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3)
{
	this->points[0].x = p1.x;
	this->points[0].y = p1.y;
	this->points[0].z = p1.z; 
	this->points[1].x = p2.x;
	this->points[1].y = p2.y;
	this->points[1].z = p2.z;
	this->points[2].x = p3.x;
	this->points[2].y = p3.y;
	this->points[2].z = p3.z;
	this->points[0].p1 = p1.x;
	this->points[0].p2 = p1.y;
	this->points[0].p3 = p1.z;
	this->points[1].p1 = p2.x;
	this->points[1].p2 = p2.y;
	this->points[1].p3 = p2.z;
	this->points[2].p1 = p3.x;
	this->points[2].p2 = p3.y;
	this->points[2].p3 = p3.z;

}

Triangle::~Triangle()
{

}

void Triangle::move(float x, float y, float z)
{
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(x, y, z);

	trans = DirectX::XMMatrixTranspose(trans);

	DirectX::XMVECTOR point1 = DirectX::XMVectorSet(this->points[0].x, this->points[0].y, this->points[0].z, 1.0f);
	DirectX::XMVECTOR point2 = DirectX::XMVectorSet(this->points[1].x, this->points[1].y, this->points[1].z, 1.0f);
	DirectX::XMVECTOR point3 = DirectX::XMVectorSet(this->points[2].x, this->points[2].y, this->points[2].z, 1.0f);
	DirectX::XMVECTOR newP1 = DirectX::XMVector3Transform(point1, trans);
	DirectX::XMVECTOR newP2 = DirectX::XMVector3Transform(point2, trans);
	DirectX::XMVECTOR newP3 = DirectX::XMVector3Transform(point2, trans);
	updatePos(newP1, newP2, newP3);
}

void Triangle::setPosition(float x, float y, float z)
{
	
}

void Triangle::setPosition(DirectX::XMFLOAT3 pos)
{
}

void Triangle::rotateX(float offset)
{
	DirectX::XMMATRIX trans = DirectX::XMMatrixRotationX(offset);
	DirectX::XMVECTOR point1 = DirectX::XMVectorSet(this->points[0].x, this->points[0].y, this->points[0].z, 1.0f);
	DirectX::XMVECTOR point2 = DirectX::XMVectorSet(this->points[1].x, this->points[1].y, this->points[1].z, 1.0f);
	DirectX::XMVECTOR point3 = DirectX::XMVectorSet(this->points[2].x, this->points[2].y, this->points[2].z, 1.0f);
	DirectX::XMVECTOR newP1 = DirectX::XMVector3Transform(point1, trans);
	DirectX::XMVECTOR newP2 = DirectX::XMVector3Transform(point2, trans);
	DirectX::XMVECTOR newP3 = DirectX::XMVector3Transform(point2, trans);
	updatePos(newP1, newP2, newP3);


}

void Triangle::rotateY(float offset)
{
	float radians = 0.0174532925f;
	DirectX::XMMATRIX trans = DirectX::XMMatrixRotationY(radians*offset);
	//this->worldMatrix = DirectX::XMMatrixTranspose(worldMatrix); //moved to colorshader
	trans = DirectX::XMMatrixTranspose(trans);
	DirectX::XMVECTOR point1 = DirectX::XMVectorSet(this->points[0].y, -this->points[0].x, this->points[0].z, 1.0f);
	DirectX::XMVECTOR point2 = DirectX::XMVectorSet(this->points[1].y, -this->points[1].x, this->points[1].z, 1.0f);
	DirectX::XMVECTOR point3 = DirectX::XMVectorSet(this->points[2].y, -this->points[2].x, this->points[2].z, 1.0f);
	DirectX::XMVECTOR newP1 = DirectX::XMVector3Transform(point1,trans);
	DirectX::XMVECTOR newP2 = DirectX::XMVector3Transform(point2, trans);
	DirectX::XMVECTOR newP3 = DirectX::XMVector3Transform(point2, trans);
	updatePos(point1, point2, point3);

}

void Triangle::rotateZ(float offset)
{
	DirectX::XMMATRIX trans = DirectX::XMMatrixRotationZ(offset);
	DirectX::XMVECTOR point1 = DirectX::XMVectorSet(this->points[0].x, this->points[0].y, this->points[0].z, 1.0f);
	DirectX::XMVECTOR point2 = DirectX::XMVectorSet(this->points[1].x, this->points[1].y, this->points[1].z, 1.0f);
	DirectX::XMVECTOR point3 = DirectX::XMVectorSet(this->points[2].x, this->points[2].y, this->points[2].z, 1.0f);
	DirectX::XMVECTOR newP1 = DirectX::XMVector3Transform(point1, trans);
	DirectX::XMVECTOR newP2 = DirectX::XMVector3Transform(point2, trans);
	DirectX::XMVECTOR newP3 = DirectX::XMVector3Transform(point2, trans);
	updatePos(newP1, newP2, newP3);
}

void Triangle::setUV(float u, float v, float u2, float v2, float u3, float v3)
{
	this->points[0].u = u;
	this->points[1].u = u2;
	this->points[2].u = u3;
	this->points[0].v = v;
	this->points[1].v = v2;
	this->points[2].v = v3;
}

void Triangle::updatePos(DirectX::XMVECTOR p1, DirectX::XMVECTOR p2, DirectX::XMVECTOR p3)
{
	this->points[0].x = DirectX::XMVectorGetX(p1);
	this->points[0].y = DirectX::XMVectorGetY(p1);
	this->points[0].z = DirectX::XMVectorGetZ(p1);
	this->points[1].x = DirectX::XMVectorGetX(p2);
	this->points[1].y = DirectX::XMVectorGetY(p2);
	this->points[1].z = DirectX::XMVectorGetZ(p2);
	this->points[2].x = DirectX::XMVectorGetX(p3);
	this->points[2].y = DirectX::XMVectorGetY(p3);
	this->points[2].z = DirectX::XMVectorGetZ(p3);
}

Vertex3D * Triangle::getThePoints()
{
	return this->points;
}

Vertex3D Triangle::getPoint(const int id)//0-2
{
	if (id < 0 || id > 2)
	{
		return Vertex3D();//Cant return less than or bigger than on the array.
	}
	return this->points[id];
}

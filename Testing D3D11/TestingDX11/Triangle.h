#ifndef TRIANGLE_H
#define TRIANGLE_H
#include"Vertex3D.h"
#include<DirectXMath.h>
class Triangle
{
public:
	Triangle();
	Triangle(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3);
	~Triangle();
	void move(float x, float y, float z);
	void setPosition(float x, float y, float z);
	void setPosition(DirectX::XMFLOAT3 pos);
	void rotateX(float offset);
	void rotateY(float offset);
	void rotateZ(float offset);
	void setUV(float u, float v, float u2, float v2, float u3, float v3);
	void updatePos(DirectX::XMVECTOR p1, DirectX::XMVECTOR p2, DirectX::XMVECTOR p3);
	Vertex3D* getThePoints();
	Vertex3D getPoint(const int id);
private:
	Vertex3D points[3];
	
};
#endif // !TRIANGLE_H

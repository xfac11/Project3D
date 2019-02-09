#ifndef CAMERA_H
#define CAMERA_H
#include <math.h>
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
class Camera
{
public:
	Camera();
	~Camera();
	DirectX::XMFLOAT3 get();
	XMFLOAT3 getPos();
	void setPos(float x, float y, float z);

	void set();
	XMMATRIX getAndUpdate(float width, float height);
	void getViewMatrix(DirectX::XMMATRIX& obj);
private:
	XMVECTOR position;
	XMVECTOR forward;//Direction the camera is pointing
	XMVECTOR right;  //perpendicular vector pointing to the right
	XMVECTOR up;     //-------------|--------------- up
	XMVECTOR target;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMMATRIX rotationMatrix;
	float yaw, pitch, roll;
};

#endif // !CAMERA_H

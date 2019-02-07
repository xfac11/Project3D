#ifndef CAMERA_H
#define CAMERA_H
#include <directxmath.h>
class Camera
{
private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	//DirectX::XMFLOAT3 lookAt; 
	DirectX::XMMATRIX viewMatrix;
public:
	Camera();
	~Camera();
	void SetPosition(float x, float y , float z);
	void SetPosition(DirectX::XMFLOAT3 position);
	void SetRotation(float x, float y , float z);
	void SetRotation(DirectX::XMFLOAT3 rotation);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();

	void Render();
	DirectX::XMMATRIX& GetViewMatrix();
};
#endif
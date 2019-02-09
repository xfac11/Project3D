#include"Camera.h"

Camera::Camera()
{
	this->target = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	this->up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	this->right = XMVector3Cross(this->forward, this->up);
	this->up = XMVectorMultiply(XMVector3Cross(this->forward, this->right), this->up);
	this->forward = XMVectorSet(0.0, 0.0, 1.0f, 0.0);
	this->position = XMVectorSet(0.0, 0.0, 0.0, 0.0);
	this->rotation = XMFLOAT3(0.0, 0.0, 0.0);
	this->yaw = 0.0f;
	this->pitch = 0.0f;
	this->roll = 0.0f;
}

Camera::~Camera()
{

}

DirectX::XMFLOAT3 Camera::get()
{
	return DirectX::XMFLOAT3();
}

XMFLOAT3 Camera::getPos()
{
	return this->position;
}

void Camera::setPos(float x, float y, float z)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
}

void Camera::set()
{

}

XMMATRIX Camera::getAndUpdate(float width, float height, float xOffset, float yOffset)
{
	XMVECTOR CamPos = this->position;  //-dist 
	XMVECTOR LookAt = XMVectorSet(0.0, 0.0, 0.0, 0.0) + CamPos; //change to first person view
	XMVECTOR Up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
	XMMATRIX View = XMMatrixLookAtLH(CamPos, LookAt, Up);
	this->forward = LookAt - this->position;
	//mouse
	this->yaw += xOffset / 4.0f;
	this->pitch += yOffset / 4.0f;
	this->pitch = min(89.0, max(-89.0, this->pitch));
	XMVECTOR front;
	front=XMVectorSet(cos(XMConvertToRadians(this->yaw - 90.0f))*cos(XMConvertToRadians(this->pitch)),
		sin(XMConvertToRadians(this->pitch)),
	XMVectorSetX(front, cos(XMConvertToRadians(this->yaw - 90.0f))*cos(XMConvertToRadians(this->pitch)));
	XMVectorSetY()
	XMMATRIX World = XMMatrixRotationY(gIncrement);
	View = XMMatrixTranspose(View);
	World = XMMatrixTranspose(World);
	XMMATRIX WorldView = XMMatrixMultiply(View, World);
	XMMATRIX Projection = XMMatrixPerspectiveFovLH((0.45f*XM_PI), WIDTH / HEIGHT, 0.1f, 20.f);
	Projection = XMMatrixTranspose(Projection);
	XMMATRIX WorldViewProj = XMMatrixMultiply(Projection, WorldView);
	WorldViewProj = XMMatrixTranspose(WorldViewProj);	
}

void Camera::getViewMatrix(DirectX::XMMATRIX & obj)
{

}

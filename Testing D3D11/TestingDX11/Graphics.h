#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "D3D.h"
#include "Camera.h"
#include "ColorShader.h"
#include "Model.h"
#include "Triangle.h"
#include <string>
//#include "TextureData.h"
//#include "bth_image.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
class Graphics
{
private:
	D3D* Direct3D;
	Camera* theCamera;
	Model* *theModel;
	ColorShader* theColorShader;
	int cap = 2;

	float jumpTimer;
	bool isJumping;
	float height;

	float color[4]; //scene class
	DirectX::XMFLOAT3 camPos; //scene class
	DirectX::XMFLOAT3 camRot; //scene class
	float gIncrement; //scene class

	void renderImgui();
	bool render(); //float [4]color
public:
	
	
	Graphics();
	~Graphics();
	void initImgui(HWND & hWnd);
	void move(Direction forward, Direction left_right, Direction up_down, bool flyMode, int mouseX, int mouseY);// Direction forward, Direction left_Right, Direction, Direction upDown);
	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);

	void Shutdown();
	bool Frame();
};
#endif
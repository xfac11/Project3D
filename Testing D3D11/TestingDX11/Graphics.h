#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "D3D.h"
#include "Camera.h"
#include "ColorShader.h"
#include "Model.h"
#include"Triangle.h"
#include"Loader.h"
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

	DirectX::XMVECTOR rotaxis;
	D3D* Direct3D;
	Camera* theCamera;
	Model* *theModel;
	int cap;
	ColorShader* theColorShader;
	Loader loadOBJ;
	float color[4]; //parameter color?
	void renderImgui();
	float jumpTimer;
	bool isJumping;
	float height;
	//float dist;
	//float xPos;
	//float yPos;
	//float xRot;
	//float yRot;
	DirectX::XMFLOAT3 camPos; //scene class
	DirectX::XMFLOAT3 camRot; //scene class
	std::vector<Vertex3D> OBJ;
	float gIncrement;
	bool render(); //float [4]color
public:
	void* operator new(size_t i) //test
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
	Graphics();
	~Graphics();
	void initImgui(HWND hWnd);
	void move(Direction forward, Direction left_right, Direction up_down, bool flyMode, int mouseX, int mouseY);// Direction
	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);

	void Shutdown();
	bool Frame(unsigned char key);
};
#endif

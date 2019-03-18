#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <string>
#include "D3D.h"
#include "Camera.h"
#include "GBuffer.h"
#include "LightShader.h"
#include "OrthoWindow.h"
#include "Model.h"
#include "Triangle.h"
#include "Terrain.h" 
#include "particlesystem.h"
//#include "ParticleHandler.h"
//#include "bth_image.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
__declspec(align(16)) class Graphics
{
private:
	D3D* Direct3D;
	Camera* theCamera;
	Model* *theModel;
	ColorShader* theColorShader[2];//forward

	Terrain* theTerrain;
	ParticleSystem* particles;

	//defered render
	DeferedShader* dShader;
	LightShader* lShader;
	GBuffer* gBuffer;
	OrthoWindow* fullQuad;

	int cap; //models cap

	float jumpTimer;
	bool isJumping;
	float height;

	float color[4]; //scene class
	DirectX::XMFLOAT3 camPos; //scene class
	DirectX::XMFLOAT3 camRot; //scene class
	float gIncrement; //scene class

	DirectX::XMVECTOR rotaxis;

	void renderImgui();
	bool render(); //float [4]color
	void renderToTexture();
	void deferredRender();
public:
	Graphics();
	~Graphics();
	void* operator new(size_t i) //test
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
	void initImgui(HWND & hWnd);
	void move(Direction forward, Direction left_right, Direction up_down, bool flyMode, int mouseX, int mouseY);// Direction forward, Direction left_Right, Direction, Direction upDown);
	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);

	void Shutdown();
	bool Frame(bool move1, bool move2);

};
#endif
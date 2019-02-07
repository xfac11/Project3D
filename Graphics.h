#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "D3D.h"
#include "Camera.h"
const bool FULL_SCREEN = false; 
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
class Graphics
{
private:
	D3D* Direct3D;
	Camera* Camera;
	//Model* Model;
	//TextureShader* TextureShader;

	bool render(); //float [4]color
public:
	Graphics();
	~Graphics();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	bool Frame();
};
#endif

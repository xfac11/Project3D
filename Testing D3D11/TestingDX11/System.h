#ifndef SYSTEM_H
#define SYSTEM_H
#include <math.h>
//#include<Keyboard.h>
#include <d3d11.h>
#include"Graphics.h"
#include <DirectXMath.h>
//#include"headerFiles.h"
#include<string>
using namespace DirectX;
//using namespace DirectX::SimpleMath;

#define HEIGHT 768.0f
#define WIDTH 768.0f
//using Microsoft::WRL::ComPtr;
class System
{
public:
	//System();
	System(HINSTANCE hInstance, LPCSTR name, int nCmdShow);
	~System();
	System(const System& obj);
	bool initialize();
	void run();
	void shutDown();
	WPARAM getMsgWParam();
	//LRESULT MessageHandler(HWND,UINT;WPARAM,LPARAM);
private:
	bool frame();
	void initializeWindows(float height, float width);
	HWND InitWindow(HINSTANCE hInstance, float height, float width);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LPCSTR applicationName;
	HINSTANCE hinstance;
	HWND hwnd;
	MSG msg;
	int nCMDShow;
	//Input* input;
	Graphics* graphics;

};
#endif
#include"System.h"

void System::initializeWindows(float height, float width)
{
}
HWND System::InitWindow(HINSTANCE hInstance, float height, float width)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	//wcex.hIcon = NULL;
	//wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//wcex.hbrBackground = NULL;
	wcex.lpszClassName = "Demo";
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, (int)width, (int)height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindow(
		"Demo",
		this->applicationName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		this);
	return hwnd;
}
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lParam);
LRESULT CALLBACK System::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{ //event-handling, event happens to window
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//System::System()
//{
//}

System::System(HINSTANCE hInstance, LPCSTR name, int nCmdShow)
{
	this->hinstance = hInstance;
	this->applicationName = name;
	this->hwnd = InitWindow(this->hinstance, HEIGHT, WIDTH);
	this->nCMDShow = nCmdShow;
	this->msg = { 0 };
	graphics = nullptr;
	graphics = new Graphics;
}

System::~System()
{
}

bool System::initialize()
{
	this->hwnd = InitWindow(this->hinstance, HEIGHT, WIDTH);
	
	return this->hwnd;
}

void System::run()
{
	if (this->hwnd)
	{
		graphics->Initialize(768, 768, this->hwnd);
		ShowWindow(this->hwnd, this->nCMDShow);
		graphics->initImgui(this->hwnd);
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//Game
				graphics->Frame();
			}
		}
		shutDown();
	}
}

void System::shutDown()
{
	DestroyWindow(this->hwnd);
}

WPARAM System::getMsgWParam()
{
	return this->msg.wParam;
}

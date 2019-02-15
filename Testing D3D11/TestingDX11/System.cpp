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
	
	System *sys;
	if (message == WM_NCCREATE)
	{
		OutputDebugStringA("Window Created.\n");
		sys = (System*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(sys));

		//SetLastError(0);
		//if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(sys)))
		//{
		//	if (GetLastError() != 0)
		//		return FALSE;
		//}
	}
	else 
		sys = (System*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	if(message== WM_DESTROY)
		PostQuitMessage(0);
	else if (message == WM_CHAR)
	{
		if (sys != nullptr)
		{
			unsigned char theChar = static_cast<unsigned char>(wParam);
			std::string msg;
			msg.push_back(theChar);
			if (sys->theKeyboard->IsCharsAutoRepeat())
			{
				sys->theKeyboard->OnChar(theChar);
			}
			else
			{
				const bool wasPressed = lParam & 0x40000000;
				if (!wasPressed)
				{
					sys->theKeyboard->OnChar(theChar);
				}
			}
			//OutputDebugStringA(msg.c_str());
			//MessageBox(hWnd, msg.c_str(), "Keyboard Input", MB_OK); //L"", L"", ;
		}
	}
	else if (message == WM_KEYDOWN)
	{
		unsigned char theKey = static_cast<unsigned char>(wParam);
		if (sys->theKeyboard->IsKeysAutoRepeat())
		{
			sys->theKeyboard->OnKeyPressed(theKey);
		}
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed)
			{
				sys->theKeyboard->OnKeyPressed(theKey);
			}
		}
	}
	else if (message == WM_KEYUP)
	{
		unsigned char theKey = static_cast<unsigned char>(wParam);
		sys->theKeyboard->OnKeyReleased(theKey);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void System::inputHandle(const unsigned char key)
{
	//graphics->move();
}




System::System(HINSTANCE hInstance, LPCSTR name, int nCmdShow)
{
	this->hinstance = hInstance;
	this->applicationName = name;
	this->hwnd = InitWindow(this->hinstance, HEIGHT, WIDTH);
	this->nCMDShow = nCmdShow;
	this->msg = { 0 };
	graphics = nullptr;
	graphics = new Graphics;
	theKeyboard = nullptr;
	theKeyboard = new Keyboard;
	//theKeyboard->EnableAutoRepeatChars();

	this->forward = Neutral;
	this->left_right = Neutral;
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
				//make keyboard stuff into private function´?
				
				if (!theKeyboard->CharBufferIsEmpty()) //decide if or while
				{
					unsigned char theChar = theKeyboard->ReadChar();
					std::string theMsg = "Character: ";
					theMsg += theChar;
					theMsg += "\n";
					//OutputDebugStringA(theMsg.c_str());

					//for typing
					//char inputs
				}
				if (!theKeyboard->KeyBufferIsEmpty())
				{
					

					KeyboardEvent keyEvent = theKeyboard->ReadKey();
					unsigned char theKey =keyEvent.GetKeyCode();
					std::string theMsg = "Key ";
					if (keyEvent.IsPress())
					{
						theMsg += "Pressed: ";
						theMsg += theKey;
						
						if (theKey == 'W')
							forward = Positive;
						else if (theKey == 'S')
							forward = Negative;
						if (theKey == 'D')
							left_right = Positive;
						else if (theKey == 'A')
							left_right = Negative;

					}
					if (keyEvent.IsRelease())
					{
						theMsg += "Released: ";
						theMsg += theKey;


						if (theKey == 'W' || theKey == 'S')
							forward = Neutral;
						if (theKey == 'A' || theKey == 'D')
							left_right = Neutral;
						
					}
					theMsg += "\n";
					OutputDebugStringA(theMsg.c_str());
				}

				graphics->move(forward,left_right);
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
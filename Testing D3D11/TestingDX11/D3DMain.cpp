#include <Windows.h>
#include"System.h"
//#pragma comment (lib, "d3d11.lib")
//#pragma comment (lib, "d3dcompiler.lib")
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{

	System sys = System(hInstance, "coolagubben", 1);
	//sys.initialize();
	sys.run();//loop
	return (int)sys.getMsgWParam();
}
#include "Graphics.h"

void Graphics::initImgui(HWND hWnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); // (void)io;
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(this->Direct3D->GetDevice(), this->Direct3D->GetDeviceContext());
	ImGui::StyleColorsDark();
}

void Graphics::renderImgui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
	ImGui::Text("This is some unuseful text.");
	//ImGui::SliderFloat("dist", &dist, -10.0f, 10.0f);
	//ImGui::SliderFloat("rotation", &gIncrement, -10.0f, 20.0f);
	ImGui::ColorEdit3("bg-color", (float*)&this->color);
	//ImGui::ColorEdit4("Triangle data", (float*)gConstantBufferData);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

bool Graphics::render()
{
	this->renderImgui();
	Direct3D->BeginScene(this->color);
	//Direct3D->initialize();
	//campos mappedmemory

	this->theModel->setVertexBuffer(this->Direct3D->GetDeviceContext());
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	Direct3D->EndScene();

	return true;
}

Graphics::Graphics()
{
	this->Direct3D = nullptr;
	this->theCamera = nullptr;
	this->theModel = nullptr;
	this->theColorShader = nullptr;
	this->color[0] = 0.1f;
	this->color[1] = 0.9f;
	this->color[2] = 0.5f;
	this->color[3] = 0.5f;
}

Graphics::~Graphics()
{
	this->Shutdown(); //????
}

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result = false;
	//if (this->Direct3D == nullptr)
	//	throw std::bad_alloc();
	this->Direct3D = new D3D;
	//this->Direct3D = (D3D*)::operator new (sizeof(D3D));
	//this->Direct3D = (D3D*)_aligned_malloc(sizeof(D3D), 16);

	result = Direct3D->initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize Direct3D", "Error", MB_OK); //L"", L"", ;
		result = false;
	}
	theCamera = new Camera;
	//theCamera = (Camera*)_aligned_malloc(sizeof(Camera), 16);
	if (!theCamera)
	{
		MessageBox(hwnd, "My B, Could not initialize Camera", "Error", MB_OK);
		result = false;
	}
	theCamera->SetPosition(0.0f, 0.0f, -5.0f);

	theModel = new Model;
	if (theModel)
	{
		MessageBox(hwnd, "My B, Could not initialize Model", "Error", MB_OK);
		result = false;
	}
	//result = theModel->Initialize(Direct3D->GetDevice());
	//if (!result)
	//{
	//	MessageBox(hwnd, "Could not initialize the model object.", L"Error", MB_OK);
	//	return false;
	//}

	theColorShader = new ColorShader;
	if (!theColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = theColorShader->Initialize(Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the color shader object.", "Error", MB_OK);
		return false;
	}

	Vertex3D TwoTris[6] = {
						-0.5f, -0.5f, 0.f,	//pos
						//0.f, 0.f, 0.f,	//color
						0.0f, 1.0f,			//tex u & v
						1.f, 1.f, 1.f,		//normal
						-0.5f, -0.5f, 0.f,  //point

						-0.5f, 0.5f, 0.f,
						0.f, 0.f,
						1.f, 1.f, 1.f,
						-0.5f, 0.5f, 0.f,

						0.5, 0.5, 0.0,
						1.0, 0.0,
						1.f, 1.f, 1.f,
						0.5, 0.5, 0.0,

						-0.5, -0.5, 0.0,
						0.0, 1.0,
						1.f, 1.f, 1.f,
						-0.5, -0.5, 0.0,

						0.5f, 0.5f, 0.f,
						1.0, 0.0,
						1.f, 1.f, 1.f,
						-0.5, -0.5, 0.0,

						0.5f, -0.5f, 0.f,
						1.f, 1.f,
						1.f, 1.f, 1.f,
						0.5f, -0.5f, 0.f,
	};
	this->theModel->addQuads(TwoTris, this->Direct3D->GetDevice());
	//TextureData* txt = nullptr;
	//txt = new TextureData(BTH_IMAGE_WIDTH, BTH_IMAGE_HEIGHT, BTH_IMAGE_DATA, sizeof(BTH_IMAGE_DATA));
	//unsigned char* image_data = BTH_IMAGE_DATA;
	//txt->IMAGE_DATA= BTH_IMAGE_DATA;
	//txt->IMAGE_HEIGHT = BTH_IMAGE_HEIGHT;
	//txt->IMAGE_WIDTH = BTH_IMAGE_WIDTH;
	this->theModel->setSampler(this->Direct3D->GetDevice());
	this->theModel->setTheTexture(this->Direct3D->GetDevice(), this->Direct3D->GetDeviceContext());
	
	return result;
}

void Graphics::Shutdown()
{/*
	if (Direct3D)
	{
		Direct3D.Shutdown();
		delete Direct3D;
		Direct3D = NULL;
	}*/
	if (Direct3D)
	{
		_aligned_free(Direct3D);
	}
	this->theModel->shutdown();
	Direct3D->Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool Graphics::Frame()
{
	return render();
}
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
	ImGui::SliderFloat("dist", &dist, -10.0f, 10.0f);//W and S Keyboard
	ImGui::SliderFloat("xPos", &xPos, -10.0f, 10.0f);//A and D Keyboard
	ImGui::SliderFloat("yPos", &yPos, -10.0f, 10.0f);//A and D Keyboard
	ImGui::SliderFloat("xRotation", &xRot, -100.0f, 100.0f);//Mouse
	ImGui::SliderFloat("yRotation", &yRot, -365.0f, 365.0f);//Mouse
	ImGui::SliderFloat("rotation", &gIncrement, -10.0f, 20.0f);
	ImGui::ColorEdit3("bg-color", (float*)&this->color);
	ImGui::CaptureKeyboardFromApp(true);
	/*if (ImGui::IsKeyPressed('a'))
	{
		xPos -= 0.1f;
	}
	else if (ImGui::IsKeyPressed('d'))
	{
		xPos += 0.1f;
	}
	if (ImGui::IsKeyPressed('w'))
	{
		dist += 0.1f;
	}
	else if (ImGui::IsKeyPressed('s'))
	{
		dist -= 0.1f;
	}*/
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
	this->theCamera->SetPosition(xPos, yPos, dist);
	this->theCamera->SetRotation(xRot, yRot, 0.0f);
	this->Direct3D->setIncrement(this->gIncrement);
	this->theCamera->Render();
	this->theColorShader->Render(this->Direct3D->GetDeviceContext(), this->theModel->getVertexCount(), this->Direct3D->GetWorldMatrix(), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix());
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
	this->dist = -1.0f;
	this->xPos = 0.0f;
	this->xRot = 0.0f;
	this->yRot = 0.0f;
	this->yPos = 0.0f;
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
	if (this->Direct3D==nullptr)
	{
		MessageBox(hwnd, "Could not create Direct3D", "Error", MB_OK);
	}
	//this->Direct3D = (D3D*)::operator new (sizeof(D3D));
	//this->Direct3D = (D3D*)_aligned_malloc(sizeof(D3D), 16);
	result=Direct3D->initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
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
	theCamera->SetPosition(0.0f, 0.0f, -1.0f);

	theModel = new Model;
	if (!theModel)
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
	this->theModel->addCube(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 1, 0, 1);
	this->theModel->addCube(DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), 1, 0, 1);
	this->theModel->addCube(DirectX::XMFLOAT3(0.0f, 0.0f, 2.0f), 1, 0, 1);
	this->theModel->addCube(DirectX::XMFLOAT3(0.0f, 0.0f, 3.0f), 1, 0, 1);
	/*this->theModel->addQuads(DirectX::XMFLOAT3(2.0f, -0.5f, 4.0f), 1, 0, 1, 0);
	this->theModel->addQuads(DirectX::XMFLOAT3(2.0f, -0.5f, 4.0f), 0, 1, 1, 0);
	this->theModel->addQuads(DirectX::XMFLOAT3(2.0f, -0.5f, 4.0f), 1, 1, 0, 0);
	this->theModel->addQuads(DirectX::XMFLOAT3(2.0f, -0.5f, 4.0f), 1, 0, 1, 1);
	this->theModel->addQuads(DirectX::XMFLOAT3(2.0f, -0.5f, 4.0f), 0, 1, 1, 1);
	this->theModel->addQuads(DirectX::XMFLOAT3(2.0f, -0.5f, 4.0f), 1, 1, 0, 1);*/
	//this->theModel->addTri(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f));


	//this->theModel->addQuads(arr, this->Direct3D->GetDevice());
	//this->theModel->addQuads(triangleVertices7, this->Direct3D->GetDevice());
	//for (int i = 0; i < 6; i++)
	//{
	//	//add += 0.5f;s
	//	for (int j = 0; j < 6; j++)
	//	{
	//		triangleVertices7[j].z += add;
	//	}

	//	this->theModel->addQuads(triangleVertices7, this->Direct3D->GetDevice());

	//}
	//for (int i = 0; i < 6; i++)
	//{
	//	triangleVertices6[i].x += 1.0f;
	//}
	//for (int i = 0; i < 6; i++)
	//{
	//	for (int j = 0; j < 6; j++)
	//	{
	//		triangleVertices6[j].z += add;
	//	}
	//	this->theModel->addQuads(triangleVertices6, this->Direct3D->GetDevice());
	//}
	//for (int i = 0; i < 6; i++)
	//{
	//	triangleVertices6[i].z -= 6.0f;
	//	triangleVertices6[i].x += 1.0f;
	//}
	//for (int i = 0; i < 6; i++)
	//{
	//	for (int j = 0; j < 6; j++)
	//	{
	//		triangleVertices6[j].z += add;
	//	}
	//	this->theModel->addQuads(triangleVertices6, this->Direct3D->GetDevice());
	//}
	//for (int i = 0; i < 6; i++)
	//{
	//	triangleVertices6[i].z -= 6.0f;
	//	triangleVertices6[i].x += 1.0f;
	//}
	//for (int i = 0; i < 6; i++)
	//{
	//	for (int j = 0; j < 6; j++)
	//	{
	//		triangleVertices6[j].z += add;
	//	}
	//	this->theModel->addQuads(triangleVertices6, this->Direct3D->GetDevice());
	//}
	//for (int i = 0; i < 6; i++)
	//{
	//	//add += 0.5f;
	//	for (int j = 0; j < 6; j++)
	//	{
	//		triangleVertices7[j].z += add;
	//	}

	//	this->theModel->addQuads(triangleVertices7, this->Direct3D->GetDevice());

	//}
	//for (int i = 0; i < 6; i++)
	//{
	//	//add += 0.5f;
	//	for (int j = 0; j < 6; j++)
	//	{
	//		triangleVertices7[j].z += add;
	//	}

	//	this->theModel->addQuads(triangleVertices7, this->Direct3D->GetDevice());

	//}
	////this->theModel->addQuads(TwoTris, this->Direct3D->GetDevice());
	//if (this->theModel->addQuads(triangleVertices, this->Direct3D->GetDevice()))
	//{

	//}
	//else
	//{
	//	MessageBox(hwnd, "Could not create another quads", "Error", MB_OK);
	//}
	//this->theModel->addQuads(triangleVertices2,this->Direct3D->GetDevice());
	//this->theModel->addQuads(triangleVertices3, this->Direct3D->GetDevice());
	//this->theModel->addQuads(triangleVertices4, this->Direct3D->GetDevice());
	//this->theModel->addQuads(triangleVertices5, this->Direct3D->GetDevice());
	//this->theModel->addQuads(triangleVertices6, this->Direct3D->GetDevice());

	if (this->theModel->createTheVertexBuffer(this->Direct3D->GetDevice()))
	{

	}
	else
	{
		MessageBox(hwnd, "Could not create vertex quads", "Error", MB_OK);
	}
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
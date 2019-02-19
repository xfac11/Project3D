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
void Graphics::move(Direction forward, Direction left_right, Direction up_down, bool flyMode, int mouseX, int mouseY)
{
	float ground_level = 0;
	float gravity = -9.82f;
	this->height = this->camPos.y - ground_level;
	
	float timePassed = sqrt(2 * height / abs(gravity));
	float velocity = gravity * timePassed;

	float speed = 0.02f*ImGui::GetIO().DeltaTime;
	int sensitivity = 30;
	this->camRot.x += mouseY * sensitivity* ImGui::GetIO().DeltaTime;
	this->camRot.y += mouseX * sensitivity* ImGui::GetIO().DeltaTime;
	
	
	if (this->camPos.y <= ground_level && up_down==Positive) //change this to collision checks with platforms
	{
		this->isJumping = true;
	}

	//flymode
	if (flyMode == true)
	{
		this->camPos.y += (-this->camRot.x) * speed*forward; //up
		this->camPos.y += speed * up_down*50;
	}
	else //no flying
	{
		if (this->camPos.y > ground_level)
		{
			this->camPos.y += velocity*ImGui::GetIO().DeltaTime; //fall
		}

		if (this->isJumping == true)
		{
			this->jumpTimer += ImGui::GetIO().DeltaTime;
			if (this->jumpTimer < 1.f)
			{
				this->camPos.y += (-9.82f * jumpTimer + 11.f)*ImGui::GetIO().DeltaTime; //wierd jump
			}
			else
			{
				this->jumpTimer = 0;
				isJumping = false;
			}
		}
	}
	

	//standard movement
	//forward backward
	if (abs(this->camRot.y) <= 90)
	{
		this->camPos.z += (90 - abs(this->camRot.y)) * speed*forward; //forward 
		this->camPos.x += (this->camRot.y) * speed*forward;  //leftright
	}
	else if (abs(this->camRot.y) >= 90)
	{
		this->camPos.z += (abs(this->camRot.y) - 90) * speed*-forward; //forward
		this->camPos.x += (180 - abs(this->camRot.y))*(abs(this->camRot.y) / this->camRot.y) * speed*forward;  //leftright
	}
	
	//left_right
	if (abs(this->camRot.y) <= 90)
	{
		this->camPos.z += (-this->camRot.y) * speed*left_right;  //forward
		this->camPos.x += (90 - abs(this->camRot.y)) * speed*left_right; //leftright

	}
	else if (abs(this->camRot.y) >= 90)
	{
		this->camPos.z += (180 - abs(this->camRot.y))*(abs(this->camRot.y) / this->camRot.y) * speed*left_right*-1;  //forward
		this->camPos.x += (abs(this->camRot.y) - 90) * speed*left_right*-1; //leftright

	}
	
}
void Graphics::renderImgui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	std::string textUse;
	
	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
	ImGui::Text("This is some unuseful text.");
	ImGui::SliderFloat("Camera X-Position", &camPos.x, -10.0f, 10.0f);//A and D Keyboard
	ImGui::SliderFloat("Camera Y-Position", &camPos.y, -10.0f, 10.0f);//A and D Keyboard
	ImGui::SliderFloat("Camera Z-Position", &camPos.z, -10.0f, 10.0f); //W & S
	ImGui::SliderFloat("Camera X-Rotation", &camRot.x, -90.0f, 90.0f); //Mouse
	ImGui::SliderFloat("Camera Y-Rotation", &camRot.y, -180.0f, 180.0f);//Mouse
	ImGui::SliderFloat("[unused]Camera Z-Rotation", &camRot.z, -360.0f, 360.0f);//Mouse
	ImGui::SliderFloat("World Rotation", &gIncrement, -6.0f, 6.0f);
	ImGui::ColorEdit3("bg-color", (float*)&this->color);
	textUse = "Height from 'Ground': " + std::to_string(this->height)+ "m";
	ImGui::Text(textUse.c_str());
	ImGui::CaptureKeyboardFromApp(true);

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



	
	this->theCamera->SetPosition(this->camPos);
	this->theCamera->SetRotation(this->camRot);
	this->Direct3D->setIncrement(this->gIncrement);
	this->theCamera->Render();
	for (int i = 0; i < cap; i++) {
		this->theModel[i]->setVertexBuffer(this->Direct3D->GetDeviceContext());
		this->theColorShader->Render(this->Direct3D->GetDeviceContext(), this->theModel[i]->getVertexCount(), this->Direct3D->GetWorldMatrix(), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix());
		this->Direct3D->setIncrement(3.14);

	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	Direct3D->EndScene();

	return true;
}

Graphics::Graphics()
{
	this->Direct3D = nullptr;
	this->theCamera = nullptr;
	this->theModel = new Model*[cap];
	for (int i = 0; i < cap; i++)
	{
		this->theModel[i] = nullptr;
	}
	this->theColorShader = nullptr;
	this->color[0] = 0.1f;
	this->color[1] = 0.9f;
	this->color[2] = 0.5f;
	this->color[3] = 0.5f;
	this->camPos = DirectX::XMFLOAT3(0.f, 0.f, -10.f);
	this->camRot = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	this->gIncrement = 0;
	
	this->jumpTimer = 0.f;
	this->isJumping = false;
	this->height = 0.f;
}

Graphics::~Graphics()
{
	//this->Shutdown(); //????
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

	for (int i = 0; i < cap; i++) {
		theModel[i] = new Model;
	}

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
	this->theModel[0]->addCube(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 1, 1, 1);
	this->theModel[1]->addCube(DirectX::XMFLOAT3(0.0f, 2.0f, 3.0f), 1, 1, 1);
	//this->theModel[2]->addCube(DirectX::XMFLOAT3(0.0f, 3.0f, 3.0f), 1, 1, 1);
	//this->theModel->addCube(DirectX::XMFLOAT3(0.0f, 0.0f, 2.0f), 2, 2, 2);
	//this->theModel->addCube(DirectX::XMFLOAT3(0.0f, 0.0f, 3.0f), 2, 2, 2);
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
	for (int i = 0; i < cap; i++) {
		if (theModel[i]->createTheVertexBuffer(this->Direct3D->GetDevice()))
		{

		}
		else
		{
			MessageBox(hwnd, "Could not create vertex quads", "Error", MB_OK);
		}
	}
	//TextureData* txt = nullptr;
	//txt = new TextureData(BTH_IMAGE_WIDTH, BTH_IMAGE_HEIGHT, BTH_IMAGE_DATA, sizeof(BTH_IMAGE_DATA));
	//unsigned char* image_data = BTH_IMAGE_DATA;
	//txt->IMAGE_DATA= BTH_IMAGE_DATA;
	//txt->IMAGE_HEIGHT = BTH_IMAGE_HEIGHT;
	//txt->IMAGE_WIDTH = BTH_IMAGE_WIDTH;

	theModel[0]->setSampler(this->Direct3D->GetDevice());
	theModel[0]->setTheTexture(this->Direct3D->GetDevice(), this->Direct3D->GetDeviceContext(), "textures/rock_01_dif_32bit.tga");
	theModel[1]->setTheTexture(this->Direct3D->GetDevice(), this->Direct3D->GetDeviceContext(), "textures/lovelive.tga");
	//theModel[2]->setTheTexture(this->Direct3D->GetDevice(), this->Direct3D->GetDeviceContext(), "textures/bricks_red_32.tga");
	
	return result;
}

void Graphics::Shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	this->Direct3D->Shutdown();
	this->theModel->shutdown();
	this->theColorShader->Shutdown();

	delete Direct3D;
	delete theCamera;
	//delete theModel; fix deletion of model
	delete theColorShader;
}

bool Graphics::Frame()
{
	return render();
}

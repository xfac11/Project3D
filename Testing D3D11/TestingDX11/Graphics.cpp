#include "Graphics.h"

void Graphics::initImgui(HWND & hWnd)
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
	float ground_level = 0.f;
	float gravity = -9.82f;
	this->height = this->camPos.y - ground_level;
	
	float timePassed = sqrt(2 * height / abs(gravity));
	float velocity = gravity * timePassed;

	float speed = 0.02f*ImGui::GetIO().DeltaTime;
	int sensitivity = 30;

	if (abs(camRot.x) > 87.f)
	{
		mouseY	 = int(abs(mouseY) * camRot.x / -abs(camRot.x));
	}
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
	ImGui::Text("This is some unuseful text."); //W and S Keyboard
	ImGui::SliderFloat("Camera X-Position", &camPos.x, -10.0f, 10.0f);//A and D Keyboard
	ImGui::SliderFloat("Camera Y-Position", &camPos.y, -10.0f, 10.0f);//A and D Keyboard
	ImGui::SliderFloat("Camera Z-Position", &camPos.z, -10.0f, 10.0f); //W & S
	ImGui::SliderFloat("Camera X-Rotation", &camRot.x, -90.0f, 90.0f); //Mouse
	ImGui::SliderFloat("Camera Y-Rotation", &camRot.y, -180.0f, 180.0f);//Mouse
	ImGui::SliderFloat("[unused]Camera Z-Rotation", &camRot.z, -360.0f, 360.0f);//Mouse
	ImGui::SliderFloat("World Rotation", &gIncrement, -6.0f, 6.0f);
	ImGui::ColorEdit3("bg-color", (float*)&this->color);

	//textUse = "Fly - Mode[true / false]: [" + std::to_string();
	//ImGui::Text(textUse);
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
	this->Direct3D->BeginScene(this->color);
	//Direct3D->initialize();
	//campos mappedmemory
	
	//this->theModel->setVertexBuffer(this->Direct3D->GetDeviceContext());
	this->theCamera->SetPosition(this->camPos);
	this->theCamera->SetRotation(this->camRot);
	//this->Direct3D->setIncrement(this->gIncrement);
	this->theCamera->Render();
	//this->theColorShader->Render(this->Direct3D->GetDeviceContext(), this->theModel->getVertexCount(), this->Direct3D->GetWorldMatrix(), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix());
	this->theModel[0]->setWorld(DirectX::XMMatrixIdentity());
	this->theModel[1]->setWorld(DirectX::XMMatrixIdentity());
	this->theModel[2]->rotate(rotaxis, this->theModel[2]->rot);
	this->theModel[2]->rot += 0.01f;
	if (this->theModel[2]->rot > 3.141590f * 2)
	{
		this->theModel[2]->rot = 0;
	}
	this->theModel[2]->setWorld();
	this->theModel[1]->setWorld();
	this->theModel[0]->setWorld();
	this->theColorShader->SetShaderParameters(this->Direct3D->GetDeviceContext(), DirectX::XMLoadFloat4x4(&this->theModel[0]->getId()), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix());
	this->theModel[0]->draw(*this->theColorShader, this->Direct3D->GetDeviceContext());
	this->theColorShader->SetShaderParameters(this->Direct3D->GetDeviceContext(), DirectX::XMLoadFloat4x4(&this->theModel[1]->getId()), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix());
	this->theModel[1]->draw(*this->theColorShader, this->Direct3D->GetDeviceContext());
	this->theColorShader->SetShaderParameters(this->Direct3D->GetDeviceContext(), DirectX::XMLoadFloat4x4(&this->theModel[2]->getId()), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix());
	this->theModel[2]->draw(*this->theColorShader, this->Direct3D->GetDeviceContext());

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	Direct3D->EndScene();


	return true;
}

Graphics::Graphics()
{
	this->Direct3D = nullptr;
	this->theCamera = nullptr;
	this->cap = 4;
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
	this->camPos = DirectX::XMFLOAT3(0.f, 0.f, -1.f);
	this->camRot = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	this->gIncrement = 0;
	
	this->jumpTimer = 0.f;
	this->isJumping = false;
	this->height = 0.f;

	this->rotaxis = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
}

Graphics::~Graphics()
{
	
}

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	//if (this->Direct3D == nullptr)
	//	throw std::bad_alloc();
	this->Direct3D = new D3D;
	if (Direct3D == nullptr)
	{
		MessageBox(hwnd, "Could not create Direct3D", "Error", MB_OK);
		return false;
	}
	result = Direct3D->initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (result==false)
	{
		MessageBox(hwnd, "Could not initialize Direct3D", "Error", MB_OK); //L"", L"", ;
		result = false;
	}
	theCamera = new Camera;
	//theCamera = (Camera*)_aligned_malloc(sizeof(Camera), 16);
	if (theCamera==nullptr)
	{
		MessageBox(hwnd, "My B, Could not initialize Camera", "Error", MB_OK);
		result = false;
	}
	theCamera->SetPosition(0.0f, 0.0f, -1.0f);

	for (int i = 0; i < cap; i++) {
		theModel[i] = new Model;
	}
	if (theModel==nullptr)
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
	if (theColorShader==nullptr)
	{
		MessageBox(hwnd, "Could not create the color shader object.", "Error", MB_OK);
		return false;
	}

	// Initialize the color shader object.
	result = theColorShader->Initialize(Direct3D->GetDevice(), hwnd);
	if (result==false)
	{
		MessageBox(hwnd, "Could not initialize the color shader object.", "Error", MB_OK);
		return false;
	}

	//Vertex3D TwoTris[] = {
	//	0.f, 0.f, 0.f,		//pos
	//	0.0f, 1.0f,			//tex u & v
	//	1.f, 1.f, 1.f,		//normal
	//	-0.5f, -0.5f, 0.f,  //point


	//	0.f, 1.08f, 0.f,
	//	0.f, 0.f,
	//	1.f, 1.f, 1.f,
	//	-0.5f, -0.5f, 0.f,

	//	0.2133f, 1.08f, 0.f,
	//	0.111f, 0.f,
	//	1.f, 1.f, 1.f,
	//	-0.5f, -0.5f, 0.f,

	//

	//	0.f, 0.f,0.f,
	//	0.f, 1.f,
	//	1.f, 1.f, 1.f,
	//	-0.5f, -0.5f, 0.f,

	//	0.2133f, 1.08f, 0.f,
	//	0.111f, 0.f,
	//	1.f, 1.f, 1.f,
	//	-0.5f, -0.5f, 0.f,

	//	0.2133f, 0.f, 0.0f,
	//	0.111f, 1.f,
	//	1.f, 1.f, 1.f,
	//	-0.5f, -0.5f, 0.f,



	//	0.2133f, 0.f, 0.f,
	//	0.1111f, 1.f,
	//	1.f, 1.f, 1.f,
	//	-0.5f, -0.5f, 0.f,

	//	0.2133f, 0.12f, 0.f,
	//	0.111f, 0.8889f,
	//	1.f, 1.f, 1.f,
	//	-0.5f, -0.5f, 0.f,

	//	1.7064f, 0.12f,0.f,
	//	0.8889f, 0.8889f,
	//	1.f, 1.f, 1.f,
	//	-0.5f, -0.5f, 0.f,


	//	0.2133f, 0.f, 0.f,
	//	0.111f, 1.f,
	//	1.f, 1.f, 1.f,
	//	-0.5f, -0.5f, 0.f,

	//	1.7067f, 0.12f, 0.f,
	//	0.8889f, 0.8889f,
	//	1.f, 1.f, 1.f,
	//	-0.5f, -0.5f, 0.f,

	//	1.7067f, 0.f, 0.f,
	//	0.8889f, 1.f,
	//	1.f, 1.f, 1.f,
	//	-0.5f, -0.5f, 0.f,




	//					//-0.5f, -0.5f, 0.f,	//pos
	//					////0.f, 0.f, 0.f,	//color
	//					//0.0f, 1.0f,			//tex u & v
	//					//1.f, 1.f, 1.f,		//normal
	//					//-0.5f, -0.5f, 0.f,  //point

	//					//-0.5f, 0.5f, 0.f,
	//					//0.f, 0.f,
	//					//1.f, 1.f, 1.f,
	//					//-0.5f, 0.5f, 0.f,

	//					//0.5, 0.5, 0.0,
	//					//1.0, 0.0,
	//					//1.f, 1.f, 1.f,
	//					//0.5, 0.5, 0.0,

	//					//-0.5, -0.5, 0.0,
	//					//0.0, 1.0,
	//					//1.f, 1.f, 1.f,
	//					//-0.5, -0.5, 0.0,

	//					//0.5f, 0.5f, 0.f,
	//					//1.0, 0.0,
	//					//1.f, 1.f, 1.f,
	//					//-0.5f, -0.5f, 0.0,

	//					//0.5f, -0.5f, 0.f,
	//					//1.f, 1.f,
	//					//1.f, 1.f, 1.f,
	//					//0.5f, -0.5f, 0.f,
	//};
	//Vertex3D triangleVertices[6] =
	//{
	//	1.92f, 0.f, 3.0f,	//v0 pos
	//	//1.0f, 0.0f, 0.0f,   //v0 color
	//	1.0f, 1.0f,//v0 textureCoord
	//	1.0f,1.0f,1.0f,
	//	0.5f, -0.5f, 0.0f,

	//	0.f, 1.08f, 3.0f,	//v1
	//	//1.0f 0.0f, 0.0f,	//v1 color
	//	0.0f, 0.0f,         //v0 textureCoord 
	//	1.0f,1.0f,1.0f,
	//	-0.5f, 0.5f, 0.0f,

	//	1.92f, 1.08f, 3.0f, //v2
	//	//1.0f, 0.0f, 0.0f,	//v2 color
	//	1.0f, 0.0f,
	//	1.0f,1.0f,1.0f,
	//	0.5f, 0.5f, 0.0f,

	//	0.f, 1.08f, 3.0f,	//v3 pos
	//	//0.0f, 0.0f, 1.0f,	//v0 color
	//	0.0f, 0.0f,
	//	1.0f,1.0f,1.0f,
	//	-0.5f, +0.5f, 0.0f,

	//	1.92f, 0.f, 3.0f,	//v4
	//	//0.0f, 0.0f, 1.0f,	//v1 color
	//	1.0f, 1.0f,
	//	1.0f,1.0f,1.0f,
	//	0.5f, -0.5f, 0.0f,

	//	0.f, 0.f, 3.0f, //v5
	//	//0.0f, 0.0f, 1.0f,	//v2 color
	//	0.0f, 1.0f,
	//	1.0f,1.0f,1.0f,
	//	-0.5f, -0.5f, 0.0f,
	//};
	//size_t nrOfTwoTris = sizeof(TwoTris) / sizeof(TwoTris[0]);
	//this->theModel->addQuads(TwoTris, nrOfTwoTris, this->Direct3D->GetDevice());
	//size_t nrTriVer = sizeof(triangleVertices) / sizeof(triangleVertices[0]);
	//this->theModel->addQuads(triangleVertices, nrTriVer, this->Direct3D->GetDevice());
	//this->theModel->createTheVertexBuffer(this->Direct3D->GetDevice());
	
	this->theModel[0]->loadOBJ("stall.obj", this->Direct3D->GetDevice(), this->Direct3D->GetDeviceContext());
	this->theModel[0]->setTheTexture(this->Direct3D->GetDevice(), this->Direct3D->GetDeviceContext(), "stallTexture.tga");
	this->theModel[0]->createTheVertexBuffer(this->Direct3D->GetDevice());
	this->theModel[1]->loadOBJ("eb_house_plant_01.obj", this->Direct3D->GetDevice(), this->Direct3D->GetDeviceContext());
	this->theModel[1]->createTheVertexBuffer(this->Direct3D->GetDevice());
	this->theModel[2]->addCube(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 2, 2, 2);
	this->theModel[2]->insertCubesInVec();
	this->theModel[2]->setTheTexture(this->Direct3D->GetDevice(), this->Direct3D->GetDeviceContext(), "rock_01_dif_32bit.tga");
	this->theModel[2]->createTheVertexBuffer(this->Direct3D->GetDevice());
	this->theModel[0]->setSampler(this->Direct3D->GetDevice());
	this->theModel[1]->setSampler(this->Direct3D->GetDevice());
	this->theModel[2]->setSampler(this->Direct3D->GetDevice());
	this->theModel[2]->setPosition(3.0f, 3.0f, 0.0f);
	this->theModel[1]->setPosition(-10.0f, 0.0f, 0.0f);
	this->theModel[0]->setPosition(0.0f, 10.0f, 0.0f);

	return result;
}

void Graphics::Shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	this->Direct3D->Shutdown();
	for (int i = 0; i < cap; i++)
	{
		this->theModel[i]->shutdown();
		delete this->theModel[i];
	}
	delete[] theModel;
	this->theColorShader->Shutdown();

	delete Direct3D;
	delete theCamera;
	delete theColorShader;
}

bool Graphics::Frame(bool move1 , bool move2)
{
	float speed = 0.02f*ImGui::GetIO().DeltaTime;

	if (move1==true)
	{
		this->theModel[2]->move(-speed, 0.0f, 0.0f);
	}
	if (move2==true)
	{
		//this->theModel[2].rotate(1.14);
		this->theModel[2]->move(speed, 0.0f, 0.0f);
		//this->theModel[2].createTheVertexBuffer(this->Direct3D->GetDevice());
		//this->Direct3D->setIncrement(3.14);// = this->theModel[2].getId();
	}

	return render();
}
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

	float speed = 0.12f*ImGui::GetIO().DeltaTime;
	int sensitivity = 30;

	if (abs(camRot.x) > 87.f)
	{
		mouseY	 = int(abs(mouseY) * camRot.x / -abs(camRot.x));
	}
	this->camRot.x += mouseY * sensitivity* ImGui::GetIO().DeltaTime;
	this->camRot.y += mouseX * sensitivity* ImGui::GetIO().DeltaTime;
	if (abs(this->camRot.y) >= 360)
		this->camRot.y = 0;
	if (this->camRot.y > 180)
		this->camRot.y = -180;
	if(this->camRot.y < -180)
		this->camRot.y = 180;
	
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
	//campos mappedmemory
	
	//this->theModel->setVertexBuffer(this->Direct3D->GetDeviceContext());
	this->theCamera->SetPosition(this->camPos);
	this->theCamera->SetRotation(this->camRot);
	//this->Direct3D->setIncrement(this->gIncrement);
	this->theCamera->Render();

	this->particles->setWorld();
	this->theTerrain->setWorld();
	this->theModel[4]->setWorld();
	this->theModel[3]->setWorld();
	this->theModel[2]->setWorld();
	this->theModel[1]->setWorld();
	this->theModel[0]->setWorld();


	this->deferredRender();

	//call deferred render{
	
	//this->theColorShader[0]->setCamPosToMatricesPerFrame(this->camPos);//new addition to perFrameMatrix struct DirectX::XMFLOAT3(-2,-1,-2)
	//this->theColorShader[1]->setCamPosToMatricesPerFrame(this->camPos ); //freeze this coords to show of backface-culling

	//this->theColorShader[1]->SetShaderParameters(this->Direct3D->GetDeviceContext(), DirectX::XMLoadFloat4x4(&this->theTerrain->getWorld()), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix());
	//this->theTerrain->Render(*this->theColorShader[1], this->Direct3D->GetDeviceContext());

	/*this->theColorShader[1]->SetShaderParameters(this->Direct3D->GetDeviceContext(), DirectX::XMLoadFloat4x4(&this->theModel[0]->getId()), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix());
	this->theModel[0]->draw(*this->theColorShader[1], this->Direct3D->GetDeviceContext());
	this->theColorShader[1]->SetShaderParameters(this->Direct3D->GetDeviceContext(), DirectX::XMLoadFloat4x4(&this->theModel[1]->getId()), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix());
	this->theModel[1]->draw(*this->theColorShader[1], this->Direct3D->GetDeviceContext());
	this->theColorShader[1]->SetShaderParameters(this->Direct3D->GetDeviceContext(), DirectX::XMLoadFloat4x4(&this->theModel[2]->getId()), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix());
	this->theModel[2]->draw(*this->theColorShader[1], this->Direct3D->GetDeviceContext());
	this->theColorShader[1]->SetShaderParameters(this->Direct3D->GetDeviceContext(), DirectX::XMLoadFloat4x4(&this->theModel[3]->getId()), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix());
	this->theModel[3]->draw(*this->theColorShader[1], this->Direct3D->GetDeviceContext());
	*/

	//billboard
	//this->theModel[4]->billboard(this->camPos);
	//this->theColorShader[1]->SetShaderParameters(this->Direct3D->GetDeviceContext(), DirectX::XMLoadFloat4x4(&this->theModel[4]->getId()), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix());
	//this->theModel[4]->draw(*this->theColorShader[1], this->Direct3D->GetDeviceContext());

	////particle
	//this->Direct3D->EnableAlphaBlending();
	//// Put the particle system vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//this->particles->billboard(this->camPos);
	//// Render the model using the texture shader.
	////for loop for color shader using every world for every particle?
	//this->theColorShader[1]->SetShaderParameters(this->Direct3D->GetDeviceContext(), DirectX::XMLoadFloat4x4(&this->particles->getWorld()), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix());
	//this->particles->Render(*this->theColorShader[1], Direct3D->GetDeviceContext());
	//
	//// Turn off alpha blending.
	//this->Direct3D->DisableAlphaBlending();

	// }end of deferrred render

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	Direct3D->EndScene();

	return true;
}

Graphics::Graphics()
{
	this->Direct3D = nullptr;
	this->theCamera = nullptr;
	this->cap = 5;
	this->theModel = new Model*[cap];
	for (int i = 0; i < cap; i++)
	{
		this->theModel[i] = nullptr;
	}
	this->theColorShader[0] = nullptr; 
	this->theColorShader[1] = nullptr;
	this->theTerrain = nullptr;
	this->particles = nullptr;

	this->dShader = nullptr;
	this->lShader = nullptr;
	this->gBuffer = nullptr;
	this->fullQuad = nullptr;

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
	bool result = false;
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

	theColorShader[0] = new ColorShader;
	//if (theColorShader[0]==nullptr)
	//{
	//	MessageBox(hwnd, "Could not create the color shader object.", "Error", MB_OK);
	//	return false;
	//}
	result = theColorShader[0]->Initialize(Direct3D->GetDevice(), hwnd, L"GeometryGrid.hlsl"); //grid init
	if (result == false)
	{
		MessageBox(hwnd, "Could not initialize the color shader object.", "Error", MB_OK);

	}
	theColorShader[1] = new ColorShader;
	result = theColorShader[1]->Initialize(Direct3D->GetDevice(), hwnd, L"GeometryShader.hlsl");//objects init
	if (result == false)
	{
		MessageBox(hwnd, "Could not initialize the color shader object.", "Error", MB_OK);

	}
	
	//defered render
	this->fullQuad = new OrthoWindow;
	if (!fullQuad)
		return false;
	result = fullQuad->Initialize(this->Direct3D->GetDevice(), screenWidth, screenHeight);
	if(!this->fullQuad){
		MessageBox(hwnd, "Could not initialize the fullQuad object.", "Error", MB_OK);
		return false;
	}
	this->dShader = new DeferedShader;
	if (!this->dShader)
		return false;
	result = this->dShader->Initialize(this->Direct3D->GetDevice(), hwnd);
	if (result == false)
	{
		MessageBox(hwnd, "Could not initialize the deferd shader object.", "Error", MB_OK);
	}
	this->lShader = new LightShader;
	if (!this->lShader)
		return false;
	result = this->lShader->Initialize(this->Direct3D->GetDevice(), hwnd);
	if (result == false)
	{
		MessageBox(hwnd, "Could not initialize the Light shader object.", "Error", MB_OK);
	}
	this->gBuffer = new GBuffer;
	if (!this->gBuffer)
		return false;
	result = this->gBuffer->initialize(this->Direct3D->GetDevice(),screenHeight, screenWidth, SCREEN_NEAR ,SCREEN_DEPTH);
	if (result == false)
	{
		MessageBox(hwnd, "Could not initialize the GBUFFER!!! object.", "Error", MB_OK);
	}


	theTerrain = new Terrain;
	if (theTerrain == nullptr)
	{
		MessageBox(hwnd, "Could not create the terrain.", "Error", MB_OK);
		return false;
	}
	result = theTerrain->Initialize(Direct3D->GetDevice(), "data/setup.txt");
	if (result == false)
	{
		MessageBox(hwnd, "Could not initialize Terrain.", "Error", MB_OK);
		return false;
	}
	theTerrain->setTheTexture(this->Direct3D->GetDevice(), this->Direct3D->GetDeviceContext(), "pebbles_color_1024_32.tga");
	theTerrain->setSampler(this->Direct3D->GetDevice());

	this->theModel[0]->loadOBJ("stall.obj" , this->Direct3D->GetDevice(), this->Direct3D->GetDeviceContext());
	this->theModel[0]->setTheTexture(this->Direct3D->GetDevice(), this->Direct3D->GetDeviceContext(), "stallTexture.tga");
	this->theModel[0]->createTheVertexBuffer(this->Direct3D->GetDevice());
	
	this->theModel[1]->loadOBJ("eb_house_plant_01.obj", this->Direct3D->GetDevice(), this->Direct3D->GetDeviceContext());
	this->theModel[1]->createTheVertexBuffer(this->Direct3D->GetDevice());

	this->theModel[2]->addCube({ 0,0,0 }, 2, 2, 2);
	this->theModel[2]->insertCubesInVec();
	this->theModel[2]->setTheTexture(this->Direct3D->GetDevice(), this->Direct3D->GetDeviceContext(), "rock_01_dif_32bit.tga");
	this->theModel[2]->createTheVertexBuffer(this->Direct3D->GetDevice());

	this->theModel[3]->addCube({ 0,0,0 }, 1, 1, 1);
	this->theModel[3]->insertCubesInVec();
	this->theModel[3]->setTheTexture(this->Direct3D->GetDevice(), this->Direct3D->GetDeviceContext(), "rock_01_dif_32bit.tga");
	this->theModel[3]->createTheVertexBuffer(this->Direct3D->GetDevice());

	//billboarded
	this->theModel[4]->addCube(DirectX::XMFLOAT3(-2.25 / 4,0, 0),2.25/2,3.50/2, 0.00f);
	this->theModel[4]->insertCubesInVec();
	this->theModel[4]->setTheTexture(this->Direct3D->GetDevice(), this->Direct3D->GetDeviceContext(), "chika_takami.tga");
	this->theModel[4]->createTheVertexBuffer(this->Direct3D->GetDevice());


	this->theModel[0]->setSampler(this->Direct3D->GetDevice());
	this->theModel[1]->setSampler(this->Direct3D->GetDevice());
	this->theModel[2]->setSampler(this->Direct3D->GetDevice());
	this->theModel[3]->setSampler(this->Direct3D->GetDevice());
	this->theModel[4]->setSampler(this->Direct3D->GetDevice());
	
	this->theModel[2]->setPosition(0.0f, 3.0f, 9.0f);
	this->theModel[3]->setPosition(0.0f, -3.0f, -5.0f);
	this->theModel[4]->setPosition(20.0f, 10.0f, 10.0f);
	this->theModel[1]->setPosition(24.0f, 0.0f, 64.0f);
	this->theModel[0]->setPosition(7.0f, 0.0f, 7.0f);
	
	this->particles = new ParticleSystem;
	this->particles->Initialize(this->Direct3D->GetDevice(), this->Direct3D->GetDeviceContext(), "chika_takami.tga");
	this->particles->setSampler(this->Direct3D->GetDevice());
	this->particles->setPosition(50.0f, 10.0f, 50.0f);
	return result;
}

void Graphics::Shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();


	for (int i = 0; i < cap; i++)
	{
		this->theModel[i]->shutdown();
		delete this->theModel[i];
	}
	delete[] theModel;
	this->theColorShader[0]->Shutdown();
	this->theColorShader[1]->Shutdown();
	delete theColorShader[0];
	delete theColorShader[1];
	
	this->Direct3D->Shutdown();
	delete Direct3D;
	delete theCamera;

	if (this->theTerrain)
	{
		theTerrain->Shutdown();
		delete theTerrain;
	}
	if (this->particles)
	{
		this->particles->Shutdown();
		delete particles;
		this->particles = nullptr;
	}

	if (this->fullQuad)
	{
		fullQuad->Shutdown();
		delete fullQuad;
		fullQuad = nullptr;
	}
	if (this->gBuffer)
	{
		gBuffer->shutDown();
		delete gBuffer;
	}
	if (this->lShader)
	{
		this->lShader->Shutdown();
		delete this->lShader;
	}
	if (this->dShader)
	{
		this->dShader->Shutdown();
		delete this->dShader;
	}
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

	this->particles->Frame(ImGui::GetIO().DeltaTime, Direct3D->GetDeviceContext());


	return render();
}

void Graphics::renderToTexture()
{

	// Set the render buffers to be the render target.
	ID3D11RenderTargetView* renderTargetsToSet[] = { gBuffer->getRenView(0),
												 gBuffer->getRenView(1),
												 gBuffer->getRenView(2)
	};

	//this->Direct3D->GetDeviceContext()->OMSetRenderTargets(3, renderTargetsToSet, this->gBuffer->getDepthStcView());
	gBuffer->setRenderTargets(Direct3D->GetDevice(), Direct3D->GetDeviceContext());
	// Clear the render buffers.
	float color[] = {
		0.0f, 0.0f, 0.0f, 1.0f
	};
	gBuffer->clear(Direct3D->GetDeviceContext(), color);

	// Get the matrices from the camera and d3d objects.


// Update the rotation variable each frame.

	DirectX::XMMATRIX worldID = DirectX::XMMatrixIdentity();
	ID3D11ShaderResourceView* null[] = { nullptr, nullptr, nullptr };
	//this->Direct3D->GetDeviceContext()->PSSetShaderResources(0, 3, null);
	// Rotate the world matrix by the rotation value so that the cube will spin.
	DirectX::XMFLOAT3 spec(0.1f, 0.1f, 0.0f);

	this->dShader->setCamPosToMatricesPerFrame(this->camPos); //this->camPos

	this->dShader->SetShaderParameters(this->Direct3D->GetDeviceContext(), DirectX::XMLoadFloat4x4(&this->theTerrain->getWorld()), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix(), spec, 0.1f);
	theTerrain->Render(*this->dShader, this->Direct3D->GetDeviceContext());


	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	this->dShader->SetShaderParameters(this->Direct3D->GetDeviceContext(), DirectX::XMLoadFloat4x4(&this->theModel[2]->getId()), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix(), spec, 1.0f);
	this->theModel[2]->draw(*this->dShader, this->Direct3D->GetDeviceContext());
	this->dShader->SetShaderParameters(this->Direct3D->GetDeviceContext(), DirectX::XMLoadFloat4x4(&this->theModel[3]->getId()), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix(), spec, 1.0f);
	this->theModel[3]->draw(*this->dShader, this->Direct3D->GetDeviceContext());
	this->dShader->SetShaderParameters(this->Direct3D->GetDeviceContext(), DirectX::XMLoadFloat4x4(&this->theModel[1]->getId()), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix(), spec, 1.0f);
	this->theModel[1]->draw(*this->dShader, this->Direct3D->GetDeviceContext());
	this->dShader->SetShaderParameters(this->Direct3D->GetDeviceContext(), DirectX::XMLoadFloat4x4(&this->theModel[0]->getId()), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix(), spec, 1.0f);
	this->theModel[0]->draw(*this->dShader, Direct3D->GetDeviceContext());
	// Render the model using the deferred shader.

	//
	this->theModel[4]->billboard(this->camPos);
	this->dShader->SetShaderParameters(this->Direct3D->GetDeviceContext(), DirectX::XMLoadFloat4x4(&this->theModel[4]->getId()), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix(),spec, 1.0f );
	this->theModel[4]->draw(*this->dShader, Direct3D->GetDeviceContext());

	this->Direct3D->EnableAlphaBlending();
	this->particles->billboard(this->camPos);
	this->dShader->SetShaderParameters(this->Direct3D->GetDeviceContext(), DirectX::XMLoadFloat4x4(&this->particles->getWorld()), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix(),spec,1.0f);
	this->particles->draw(*this->dShader, Direct3D->GetDeviceContext());
	this->Direct3D->DisableAlphaBlending();

	ID3D11RenderTargetView* nullRTV = nullptr;
	this->Direct3D->GetDeviceContext()->OMSetRenderTargets(1, &nullRTV, nullptr);
	// Reset the render target back to the original back buffer and not the render buffers anymore.
	this->Direct3D->setBackBuffer();
	this->Direct3D->resetViewPort();
	// Reset the viewport back to the original.
	//nullRTV->Release();

}

void Graphics::deferredRender()
{
	this->renderToTexture();
	Direct3D->BeginScene(this->color);

	Direct3D->turnOffZ();
	fullQuad->Render(this->Direct3D->GetDeviceContext());//Prepares and sets the vertex buffer 

	DirectX::XMVECTOR rotaxis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	this->theModel[2]->rot += 0.01f;
	float movex = sin(this->theModel[2]->rot);
	//this->theModel[2]->move(0.0f, movex, 0.0f);
	if (this->theModel[2]->rot > 3.141590f * 2)
	{
		this->theModel[2]->rot = 0;
	}
	//this->theModel[2]->setWorld();
	//this->theModel[1]->setWorld();

	PointLight light[4];
	DirectX::XMMATRIX worldID = DirectX::XMMatrixIdentity();
	worldID = DirectX::XMMatrixTranslation(this->theModel[2]->getPosition().x,
		this->theModel[2]->getPosition().y, this->theModel[2]->getPosition().z);//Sets the position of the light to the sphere
	DirectX::XMFLOAT4 lPos(0.0f, 0.0f, 0.0f, 10.0f);
	DirectX::XMFLOAT4 lColor(1.f, 0.5f, 1.0f, 0.0f);
	light[0] = { worldID, lPos, lColor };
	//lColor = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
	//worldID = DirectX::XMMatrixTranslation(15.0f, 6.0f, 0.0f);
	//light[1] = { worldID,lPos.x,lPos.y,lPos.z,10.0f,
	//			lColor.x,lColor.y,lColor.z,1.0f };
	//worldID = DirectX::XMMatrixTranslation(7.0f, 6.0f, 10.0f);
	//light[2] = { worldID,lPos.x,lPos.y,lPos.z,10.0f,
	//			lColor.x,lColor.y,lColor.z,1.0f };
	//worldID = DirectX::XMMatrixTranslation(7.0f, 6.0f, 15.0f);
	//light[3] = { worldID,lPos.x,lPos.y,lPos.z,10.0f,
	//			lColor.x,lColor.y,lColor.z,1.0f };
	ID3D11ShaderResourceView* viewsToSet[] = { gBuffer->getShadResView(0),
										   gBuffer->getShadResView(1),
											gBuffer->getShadResView(2)
	};
	ID3D11ShaderResourceView* view = gBuffer->getShadResView(0);
	ID3D11ShaderResourceView* view1 = gBuffer->getShadResView(1);
	ID3D11ShaderResourceView* view2 = gBuffer->getShadResView(2);


	ID3D11ShaderResourceView* null[] = { nullptr, nullptr, nullptr };
	this->Direct3D->GetDeviceContext()->PSSetShaderResources(0, 3, null);
	//}
//	this->Direct3D->GetDeviceContext()->PSSetShaderResources(0, 1, nullptr);
	//this->Direct3D->GetDeviceContext()->PSSetShaderResources(1, 1, nullptr);
	//this->Direct3D->GetDeviceContext()->PSSetShaderResources(2, 1, nullptr);

	this->Direct3D->GetDeviceContext()->PSSetShaderResources(0, 1, &view);
	this->Direct3D->GetDeviceContext()->PSSetShaderResources(1, 1, &view1);
	this->Direct3D->GetDeviceContext()->PSSetShaderResources(2, 1, &view2);

	//this->gBuffer->setShaderResViews(this->Direct3D->GetDeviceContext());
	//this->theColorShader->SetShaderParameters(this->Direct3D->GetDeviceContext(), DirectX::XMLoadFloat4x4(&this->theModel[2]->getId()), this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix());// lPos, lColor, 1.0f, 1.0f);
	this->lShader->SetShaderParameters(this->Direct3D->GetDeviceContext(), worldID, this->theCamera->GetViewMatrix(), this->Direct3D->GetProjectionMatrix(), light, this->camPos);

	//this->lShader->SetShaderParameters(this->Direct3D->GetDeviceContext(), DirectX::XMLoadFloat4x4(&this->theModel[2]->getId()), this->theCamera->GetViewMatrix(), this->Direct3D->GetOrthoMatrix(), this->gBuffer->getShaderResView(0), this->gBuffer->getShaderResView(1), lPos, lColor, 1.0f, 1.0f);
	//Fix properties in the shaders. Entrypoint and shader type
	//Fix properties in the shaders. Entrypoint and shader type

	//this->theModel[2]->draw(*this->theColorShader, this->Direct3D->GetDeviceContext());
	this->lShader->RenderShader(this->Direct3D->GetDeviceContext(), 6);
	this->Direct3D->turnOnZ();

}
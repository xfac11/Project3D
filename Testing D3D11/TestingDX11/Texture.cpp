#include"Texture.h"

Texture::Texture()
{
	//this->texture = nullptr;
	this->textureView = nullptr;
	//this->DATA = nullptr;
}

Texture::~Texture()
{

}

void Texture::setTexture(ID3D11Device*& gDevice, ID3D11DeviceContext *& gDeviceContext, std::string fileName)
{
	std::string fileTexture = "textures/" + fileName;
	const char *file = fileTexture.c_str();
	testTexture.Initialize(gDevice, gDeviceContext, file);
	/*if (!testTexture.Initialize(gDevice, gDeviceContext, fileName))
	{
		std::string out = "\n\nFailed load texture:  ";
		out+=fileName;
		out += "\n\n";
		OutputDebugStringA(out.c_str());
	}*/
	
	ID3D11Texture2D* tex = nullptr;
	D3D11_SUBRESOURCE_DATA texInitData = { 0 };
	ZeroMemory(&texInitData, sizeof(texInitData));
	texInitData.pSysMem = (const void*)testTexture.getTextureCharArray();
	texInitData.SysMemPitch = testTexture.getWidth() * 4;
	texInitData.SysMemSlicePitch = testTexture.getHeight() * testTexture.getWidth() * 4;

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Height = testTexture.getHeight();
	texDesc.Width = testTexture.getWidth();
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	gDevice->CreateTexture2D(&texDesc, &texInitData, &tex);
	gDevice->CreateShaderResourceView(tex, NULL, &this->textureView);
}
ID3D11ShaderResourceView *& Texture::getTexture()
{
	return this->textureView;
}

void Texture::cleanUp()
{
	if (this->textureView)
		this->textureView->Release();
	//this->texture->Release();
	this->testTexture.Shutdown();
}
#include"Texture.h"

Texture::Texture()
{
	//this->targaData = nullptr;
	this->textureView = nullptr;
	this->DATA = nullptr;
}

Texture::~Texture()
{

}

void Texture::setTexture2(ID3D11Device*& gDevice, ID3D11DeviceContext  *&gDeviceContext)
{
	//this->targaData->setDATA(obj->getDATA(),obj->getSize());
	//this->targaData->setWIDTH(obj->getWidth());
	//this->targaData->setHEIGHT(obj->getHeight());
	ID3D11Texture2D* tex = nullptr;
	D3D11_SUBRESOURCE_DATA texInitData = { 0 };
	ZeroMemory(&texInitData, sizeof(texInitData));
	texInitData.pSysMem = (const void*)BTH_IMAGE_DATA;
	texInitData.SysMemPitch = BTH_IMAGE_WIDTH * 4;
	texInitData.SysMemSlicePitch = BTH_IMAGE_HEIGHT * BTH_IMAGE_WIDTH * 4;


	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Height = BTH_IMAGE_HEIGHT;
	texDesc.Width = BTH_IMAGE_WIDTH;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	gDevice->CreateTexture2D(&texDesc, &texInitData, &tex);
	gDevice->CreateShaderResourceView(tex, NULL, &this->textureView);
}

void Texture::setTexture(ID3D11Device*& gDevice, ID3D11DeviceContext *& gDeviceContext, char* filename)
{
	testTexture.Initialize(gDevice, gDeviceContext, filename);
	/*this->textureView = testTexture.GetTexture();

	ID3D11Texture2D* tex = nullptr;
	D3D11_SUBRESOURCE_DATA texInitData = { 0 };
	ZeroMemory(&texInitData, sizeof(texInitData));
	texInitData.pSysMem = (const void*)testTexture.getTextureCharArray();
	texInitData.SysMemPitch = testTexture.getWidth() * 4;
	texInitData.SysMemSlicePitch = testTexture.getHeight() * testTexture.getWidth() * 4;*/

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
	this->textureView->Release();
}
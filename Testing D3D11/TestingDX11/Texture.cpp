#include"Texture.h"

Texture::Texture()
{
	this->targaData = nullptr;
	this->textureView = nullptr;
}

Texture::~Texture()
{

}

void Texture::setTexture(TextureData* obj, ID3D11Device* gDevice)
{
	this->targaData->IMAGE_DATA = obj->IMAGE_DATA;
	this->targaData->IMAGE_HEIGHT = obj->IMAGE_HEIGHT;
	this->targaData->IMAGE_WIDTH = obj->IMAGE_WIDTH;
	ID3D11Texture2D* tex = nullptr;
	D3D11_SUBRESOURCE_DATA texInitData = { 0 };
	ZeroMemory(&texInitData, sizeof(texInitData));
	texInitData.pSysMem = (const void*)obj->IMAGE_DATA;
	texInitData.SysMemPitch = obj->IMAGE_WIDTH * 4;
	texInitData.SysMemSlicePitch = obj->IMAGE_HEIGHT * obj->IMAGE_WIDTH * 4;


	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Height = obj->IMAGE_HEIGHT;
	texDesc.Width = obj->IMAGE_WIDTH;
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

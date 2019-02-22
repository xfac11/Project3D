#ifndef TEXTURE_H
#define TEXTURE_H
#include <d3d11.h>
#include <directxmath.h>
//#include"Structs.h"
#include "bth_image.h"
#include "textureload.h"
//#include "TextureData.h"
class Texture
{
public:
	Texture();
	~Texture();
	void setTexture2(ID3D11Device*& gDevice, ID3D11DeviceContext *& gDeviceContext);
	void setTexture(ID3D11Device*& gDevice, ID3D11DeviceContext *& gDeviceContext, std::string fileName);
	//void changeTextureData(TextureData* obj);
	ID3D11ShaderResourceView*& getTexture();
	void cleanUp();
private:
	//ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* textureView;
	//TextureData* targaData;
	unsigned char* DATA;
	TextureLoad testTexture;

};
#endif // !TEXTURE_H
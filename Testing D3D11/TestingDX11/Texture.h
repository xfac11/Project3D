#ifndef TEXTURE_H
#define TEXTURE_H
#include <d3d11.h>
#include <directxmath.h>
#include"Structs.h"
class Texture
{
public:
	Texture();
	~Texture();
	void setTexture(TextureData* obj, ID3D11Device* gDevice);
	//void changeTextureData(TextureData* obj);
	ID3D11ShaderResourceView*& getTexture();
	void cleanUp();
private:
	//ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* textureView;
	TextureData* targaData;

};
#endif // !TEXTURE_H

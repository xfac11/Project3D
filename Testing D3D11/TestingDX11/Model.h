#ifndef MODEL_H
#define MODEL_H
#include <d3d11.h>
#include <directxmath.h>
#include"QuadHandler.h"
#include"Texture.h"
class Model
{
public:
	Model();
	~Model();
	//This before setVertexBuffer
	void addQuads(Vertex3D newQuad[6], ID3D11Device *& gDevice);
	void shutdown();
	void setVertexBuffer(ID3D11DeviceContext *& gDeviceContext);
	int getVertexCount()const;
	void setTheTexture(TextureData* txt, ID3D11Device* gDevice);
	void setSampler(ID3D11Device* gDevice);
private:
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *indexBuffer;
	ID3D11Buffer *constantBuffer;
	ID3D11SamplerState* SamplerState;

	Texture texture;
	int vertexCount;
	int indexCount;
	QuadHandler quads;
};
#endif // !MODEL_H

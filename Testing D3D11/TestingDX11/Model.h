#ifndef MODEL_H
#define MODEL_H
#include <d3d11.h>
#include <directxmath.h>
class Model
{
private:
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int vertexCount;
	int indexCount;

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
public:
	Model();
	Model(const Model&);
	~Model();


	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
};
#endif
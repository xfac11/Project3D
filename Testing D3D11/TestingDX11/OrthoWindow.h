#ifndef _ORTHOWINDOW_H_
#define _ORTHOWINDOW_H_
#include <d3d11.h>
#include <DirectXMath.h>
class OrthoWindow
{
private:
	struct VertexType
	{
		float x, y, z;
		float u, v;
	};

public:
	OrthoWindow();
	//OrthoWindow(const OrthoWindow&);
	~OrthoWindow();

	bool Initialize(ID3D11Device* device, int width, int height);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

private:
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *indexBuffer;
	int vertexCount;
	int indexCount;


	bool InitializeBuffers(ID3D11Device* device , int width, int height);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

};

#endif
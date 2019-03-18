////////////////////////////////////////////////////////////////////////////////
// Filename: orthowindowclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef ORTHOWINDOW H_
#define ORTHOWINDOW_H


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DirectXMath.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: OrthoWindowClass
////////////////////////////////////////////////////////////////////////////////
class OrthoWindow
{
private:
	struct VertexType
	{
		float x,y, z;
		float u,v;
	};

public:
	OrthoWindow();
	~OrthoWindow();

	bool Initialize(ID3D11Device* device, int windowWidth, int windowHeight);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device* device, int windowWidth, int windowHeight);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *indexBuffer;
	int vertexCount;
	int	indexCount;
};
#endif
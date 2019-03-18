////////////////////////////////////////////////////////////////////////////////
// Filename: OrthoWindow.cpp
////////////////////////////////////////////////////////////////////////////////
#include"OrthoWindow.h"


OrthoWindow::OrthoWindow()
{
	vertexBuffer = 0;
	indexBuffer = 0;
}

OrthoWindow::~OrthoWindow()
{
}


bool OrthoWindow::Initialize(ID3D11Device* device, int windowWidth, int windowHeight)
{
	bool result;


	// Initialize the vertex and index buffer that hold the geometry for the ortho window model.
	result = InitializeBuffers(device, windowWidth, windowHeight);
	if (!result)
	{
		return false;
	}

	return true;
}


void OrthoWindow::Shutdown()
{
	// Release the vertex and index buffers.
	ShutdownBuffers();
}


void OrthoWindow::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);
}


int OrthoWindow::GetIndexCount()
{
	return indexCount;
}


bool OrthoWindow::InitializeBuffers(ID3D11Device* device, int windowWidth, int windowHeight)
{
	float left, right, top, bottom;
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Calculate the screen coordinates of the left side of the window.
	left = (float)((windowWidth / 2) * -1);

	// Calculate the screen coordinates of the right side of the window.
	right = left + (float)windowWidth;

	// Calculate the screen coordinates of the top of the window.
	top = (float)(windowHeight / 2);

	// Calculate the screen coordinates of the bottom of the window.
	bottom = top - (float)windowHeight;

	// Set the number of vertices in the vertex array.
	vertexCount = 6;

	// Set the number of indices in the index array.
	indexCount = vertexCount;

	// Create the vertex array.
	vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[indexCount];
	if (!indices)
	{
		return false;
	}

	 //Load the vertex array with data.
	 //First triangle.

		// Load the vertex array with data.
		// First triangle.
	vertices[0].x = left;
	vertices[0].y = top;
	vertices[0].z = 0.0f; // Top left.
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	vertices[1].x = right;
	vertices[1].y = bottom;
	vertices[1].z = 0.0f; // Bottom right.
	vertices[1].u = 1.0f;
	vertices[1].v = 1.0f;

	vertices[2].x = left;
	vertices[2].y = bottom;
	vertices[2].z = 0.0f; // Bottom left.
	vertices[2].u = 0.0f;
	vertices[2].v = 1.0f;

	vertices[3].x = left;
	vertices[3].y = top;
	vertices[3].z = 0.0f; // Top left.
	vertices[3].u = 0.0f;
	vertices[3].v = 0.0f;

	vertices[4].x = right;
	vertices[4].y = top;
	vertices[4].z = 0.0f; // Top right.
	vertices[4].u = 1.0f;
	vertices[4].v = 0.0f;

	vertices[5].x = right;
	vertices[5].y = bottom;
	vertices[5].z = 0.0f; // Bottom right.
	vertices[5].u = 1.0f;
	vertices[5].v = 1.0f;


	// Load the index array with data.
	for (i = 0; i < indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finally create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void OrthoWindow::ShutdownBuffers()
{
	// Release the index buffer.
	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = 0;
	}
}


void OrthoWindow::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
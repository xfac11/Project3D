#include "Terrain.h"

bool Terrain::InitializeBuffers(ID3D11Device *& device)
{
// Set the index count to the same as the vertex count.
	this->indexCount = this->vertexCount;

	Vertex3D* vertices = new Vertex3D[this->vertexCount];
	unsigned long* indices = new unsigned long[this->indexCount];
	int index = 0; // Initialize the index to the vertex array.
	float positionX = 0.f;
	float positionZ = 0.f;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT result;

	//DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	// Load the vertex array and index array with 3D terrain model data.
	for (int i = 0; i < this->vertexCount; i++)
	{
		//vertices[l].position = XMFLOAT3(m_terrainModel[i].x, m_terrainModel[i].y, m_terrainModel[i].z);
		//vertices[l].color = color;
		vertices[i].x = body.at(i).x;//terrainModel[i].x;
		vertices[i].y = body.at(i).y;//terrainModel[i].y;
		vertices[i].z = body.at(i).z;//terrainModel[i].z;
		vertices[i].u = body.at(i).u; //terrainModel[i].u;
		vertices[i].v = body.at(i).v;//terrainModel[i].v;
		indices[i] = i;
	}
	/*
	//// Load the vertex and index arrays with the terrain data.
	//for (int j = 0; j < (this->terrainHeight - 1); j++)
	//{
	//	for (int i = 0; i < (this->terrainWidth - 1); i++)
	//	{
	//		// LINE 1
	//		// Upper left.
	//		positionX = (float)i;
	//		positionZ = (float)(j + 1);

	//		//vertices[index].position = DirectX::XMFLOAT3(positionX, 0.0f, positionZ);
	//		vertices[index].x = positionX;
	//		vertices[index].y = 0.f;
	//		vertices[index].z = positionZ;
	//		//vertices[index].color = color;
	//		indices[index] = index;
	//		index++;

	//		// Upper right.
	//		positionX = (float)(i + 1);
	//		positionZ = (float)(j + 1);

	//		//vertices[index].position = DirectX::XMFLOAT3(positionX, 0.0f, positionZ);
	//		vertices[index].x = positionX;
	//		vertices[index].y = 0.f;
	//		vertices[index].z = positionZ;
	//		//vertices[index].color = color;
	//		indices[index] = index;
	//		index++;

	//		// LINE 2
	//		// Upper right.
	//		positionX = (float)(i + 1);
	//		positionZ = (float)(j + 1);

	//		//vertices[index].position = DirectX::XMFLOAT3(positionX, 0.0f, positionZ);
	//		vertices[index].x = positionX;
	//		vertices[index].y = 0.f;
	//		vertices[index].z = positionZ;
	//		//vertices[index].color = color;
	//		indices[index] = index;
	//		index++;

	//		// Bottom right.
	//		positionX = (float)(i + 1);
	//		positionZ = (float)j;

	//		//vertices[index].position = DirectX::XMFLOAT3(positionX, 0.0f, positionZ);
	//		vertices[index].x = positionX;
	//		vertices[index].y = 0.f;
	//		vertices[index].z = positionZ;
	//		//vertices[index].color = color;
	//		indices[index] = index;
	//		index++;

	//		// LINE 3
	//		// Bottom right.
	//		positionX = (float)(i + 1);
	//		positionZ = (float)j;

	//		//vertices[index].position = DirectX::XMFLOAT3(positionX, 0.0f, positionZ);
	//		vertices[index].x = positionX;
	//		vertices[index].y = 0.f;
	//		vertices[index].z = positionZ;
	//		//vertices[index].color = color;
	//		indices[index] = index;
	//		index++;

	//		// Bottom left.
	//		positionX = (float)i;
	//		positionZ = (float)j;

	//		//vertices[index].position = DirectX::XMFLOAT3(positionX, 0.0f, positionZ);
	//		vertices[index].x = positionX;
	//		vertices[index].y = 0.f;
	//		vertices[index].z = positionZ;
	//		//vertices[index].color = color;
	//		indices[index] = index;
	//		index++;

	//		// LINE 4
	//		// Bottom left.
	//		positionX = (float)i;
	//		positionZ = (float)j;

	//		//vertices[index].position = DirectX::XMFLOAT3(positionX, 0.0f, positionZ);
	//		vertices[index].x = positionX;
	//		vertices[index].y = 0.f;
	//		vertices[index].z = positionZ;
	//		//vertices[index].color = color;
	//		indices[index] = index;
	//		index++;

	//		// Upper left.
	//		positionX = (float)i;
	//		positionZ = (float)(j + 1);

	//		//vertices[index].position = DirectX::XMFLOAT3(positionX, 0.0f, positionZ);
	//		vertices[index].x = positionX;
	//		vertices[index].y = 0.f;
	//		vertices[index].z = positionZ;
	//		//vertices[index].color = color;
	//		indices[index] = index;
	//		index++;
	//	}
	//}
	*/

	//memset(&vertexBufferDesc, 0, sizeof(vertexBufferDesc));
	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex3D) * this->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &this->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//memset(&indexBufferDesc, 0, sizeof(indexBufferDesc));
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * this->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &this->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

bool Terrain::LoadSetupFile(char * filename)
{
	int stringLength = 256;
	std::ifstream fileInput;
	char input;

	// Initialize the string that will hold the terrain file name.	
	terrainFilename = new char[stringLength];

	// Open the setup file.  If it could not open the file then exit.
	fileInput.open(filename);
	if (fileInput.fail())
	{
		return false;
	}

	// Read up to the terrain file name.
	fileInput.get(input);
	while (input != ':')
	{
		fileInput.get(input);
	}

	// Read in the terrain file name.
	fileInput >> terrainFilename;

	// Read up to the value of terrain height.
	fileInput.get(input);
	while (input != ':')
	{
		fileInput.get(input);
	}

	// Read in the terrain height.
	fileInput >> terrainHeight;

	// Read up to the value of terrain width.
	fileInput.get(input);
	while (input != ':')
	{
		fileInput.get(input);
	}

	// Read in the terrain width.
	fileInput >> terrainWidth;

	// Read up to the value of terrain height scaling.
	fileInput.get(input);
	while (input != ':')
	{
		fileInput.get(input);
	}

	// Read in the terrain height scaling.
	fileInput >> heightScale;

	// Close the setup file.
	fileInput.close();

	return true;
}

bool Terrain::LoadBitmapHeightMap()
{
	int error;
	int imageSize;
	int index;
	int k = 0;// Initialize the position in the image data buffer.
	FILE* filePtr;
	unsigned long long count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	unsigned char* bitmapImage;
	unsigned char height;

	// Start by creating the array structure to hold the height map data.
	this->heightMap = new HeightMapType[this->terrainWidth * this->terrainHeight];
	if (!heightMap)
	{
		return false;
	}

	// Open the bitmap map file in binary.
	error = fopen_s(&filePtr, terrainFilename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the bitmap file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Make sure the height map dimensions are the same as the terrain dimensions for easy 1 to 1 mapping.
	if ((bitmapInfoHeader.biHeight != terrainHeight) || (bitmapInfoHeader.biWidth != terrainWidth))
	{
		return false;
	}

	// Calculate the size of the bitmap image data.  
	// Since we use non-divide by 2 dimensions (eg. 257x257) we need to add an extra byte to each line.
	imageSize = terrainHeight * ((terrainWidth * 3) + 1);

	// Allocate memory for the bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}


	// Read the image data into the height map array.
	for (int j = 0; j < terrainHeight; j++)
	{
		for (int i = 0; i < terrainWidth; i++)
		{
			// Bitmaps are upside down so load bottom to top into the height map array.
			index = (terrainWidth * (terrainHeight - 1 - j)) + i;

			// Get the grey scale pixel value from the bitmap image data at this location.
			height = bitmapImage[k];

			// Store the pixel value as the height at this point in the height map array.
			heightMap[index].y = (float)height;

			// Increment the bitmap image data index.
			k += 3;
		}

		// Compensate for the extra byte at end of each line in non-divide by 2 bitmaps (eg. 257x257).
		k++;
	}

	// Release the bitmap image data now that the height map array has been loaded.
	delete[] bitmapImage;
	bitmapImage = nullptr;

	// Release the terrain filename now that is has been read in.
	delete[] terrainFilename;
	terrainFilename = nullptr;

	return true;
}


void Terrain::SetTerrainCoordinates()
{
	// Loop through all the elements in the height map array and adjust their coordinates correctly.
	for (int j = 0; j < this->terrainHeight; j++)
	{
		for (int i = 0; i < this->terrainWidth; i++)
		{
			int index = (this->terrainWidth * j) + i;

			// Set the X and Z coordinates.
			this->heightMap[index].x = (float)i;
			this->heightMap[index].z = -(float)j;

			// Move the terrain depth into the positive range.  For example from (0, -256) to (256, 0).
			this->heightMap[index].z += (float)(this->terrainHeight - 1);

			// Scale the height.
			this->heightMap[index].y /= this->heightScale;
		}
	}
}

bool Terrain::BuildTerrainModel()
{
	int index = 0; // Initialize the index into the height map array.
	//int index1, index2, index3, index4;

	// Calculate the number of vertices in the 3D terrain model.
	this->vertexCount = (this->terrainHeight - 1) * (this->terrainWidth - 1) * 6;

	// Create the 3D terrain model array.
	//this->terrainModel = new Vertex3D[this->vertexCount];
	//if (!this->terrainModel)
	//	return false;

	// Load the 3D terrain model with the height map terrain data.
	// We will be creating 2 triangles for each of the four points in a quad.
	for (int j = 0; j < (this->terrainHeight - 1); j++)
	{
		for (int i = 0; i < (this->terrainWidth - 1); i++)
		{
			// Get the indexes to the four points of the quad.
			int index1 = (this->terrainWidth * j) + i;          // Upper left.
			int index2 = (this->terrainWidth * j) + (i + 1);      // Upper right.
			int index3 = (this->terrainWidth * (j + 1)) + i;      // Bottom left.
			int index4 = (this->terrainWidth * (j + 1)) + (i + 1);  // Bottom right.

			// Now create two triangles for that quad.
			// Triangle 1 - Upper left.
			//this->terrainModel[index].x = this->heightMap[index1].x;
			//this->terrainModel[index].y = this->heightMap[index1].y;
			//this->terrainModel[index].z = this->heightMap[index1].z;
			////add tex here
			//this->terrainModel[index].u = 0.f;
			//this->terrainModel[index].v = 0.f;
			//index++;
			Vertex3D temp;
			temp.x = this->heightMap[index1].x;
			temp.y = this->heightMap[index1].y;
			temp.z = this->heightMap[index1].z;
			temp.u = 0.f;
			temp.v = 0.f;
			body.push_back(temp);
		

			// Triangle 1 - Upper right.
			temp.x = this->heightMap[index2].x;
			temp.y = this->heightMap[index2].y;
			temp.z = this->heightMap[index2].z;
			temp.u = 1.f;
			temp.v = 0.f;
			body.push_back(temp);

			// Triangle 1 - Bottom left.
			temp.x = this->heightMap[index3].x;
			temp.y = this->heightMap[index3].y;
			temp.z = this->heightMap[index3].z;
			temp.u = 0.f;
			temp.v = 1.f;
			body.push_back(temp);

			// Triangle 2 - Bottom left.
			temp.x = this->heightMap[index3].x;
			temp.y = this->heightMap[index3].y;
			temp.z = this->heightMap[index3].z;
			temp.u = 0.f;
			temp.v = 1.f;
			body.push_back(temp);

			// Triangle 2 - Upper right.
			temp.x = this->heightMap[index2].x;
			temp.y = this->heightMap[index2].y;
			temp.z = this->heightMap[index2].z;
			temp.u = 1.f;
			temp.v = 0.f;
			body.push_back(temp);

			// Triangle 2 - Bottom right.
			temp.x = this->heightMap[index4].x;
			temp.y = this->heightMap[index4].y;
			temp.z = this->heightMap[index4].z;
			temp.u = 1.f;
			temp.v = 1.f;
			body.push_back(temp);
		}
	}
	return true;
}

void Terrain::calculateModelVectors()
{
	int faceCount = vertexCount / 3;
	int index = 0;
	NM_Vertex vertex1;
	NM_Vertex vertex2;
	NM_Vertex vertex3;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 binormal;
	DirectX::XMFLOAT3 normal;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (int i = 0; i < faceCount; i++)
	{
		// Get the three vertices for this face from the model.
		vertex1.x = body.at(index).x;
		vertex1.y = body.at(index).y;
		vertex1.z = body.at(index).z;
		vertex1.u = body.at(index).u;
		vertex1.v = body.at(index).v;
		vertex1.nx = body.at(index).nx;
		vertex1.ny = body.at(index).ny;
		vertex1.nz = body.at(index).nz;
		index++;

		vertex2.x = body.at(index).x;
		vertex2.y = body.at(index).y;
		vertex2.z = body.at(index).z;
		vertex2.u = body.at(index).u;
		vertex2.v = body.at(index).v;
		vertex2.nx = body.at(index).nx;
		vertex2.ny = body.at(index).ny;
		vertex2.nz = body.at(index).nz;
		index++;

		vertex3.x = body.at(index).x;
		vertex3.y = body.at(index).y;
		vertex3.z = body.at(index).z;
		vertex3.u = body.at(index).u;
		vertex3.v = body.at(index).v;
		vertex3.nx = body.at(index).nx;
		vertex3.ny = body.at(index).ny;
		vertex3.nz = body.at(index).nz;
		index++;
		// Calculate the tangent and binormal of that face.
		calculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal, normal);

		// Calculate the new normal using the tangent and binormal.
		calculateNormal(tangent, binormal, normal);

		// Store the normal, tangent, and binormal for this face back in the model structure.
		body.at(index - 1).nx = normal.x;
		body.at(index - 1).ny = normal.y;
		body.at(index - 1).nz = normal.z;
		body.at(index - 1).tx = tangent.x;
		body.at(index - 1).ty = tangent.y;
		body.at(index - 1).tz = tangent.z;
		body.at(index - 1).bx = binormal.x;
		body.at(index - 1).by = binormal.y;
		body.at(index - 1).bz = binormal.z;

		body.at(index - 2).nx = normal.x;
		body.at(index - 2).ny = normal.y;
		body.at(index - 2).nz = normal.z;
		body.at(index - 2).tx = tangent.x;
		body.at(index - 2).ty = tangent.y;
		body.at(index - 2).tz = tangent.z;
		body.at(index - 2).bx = binormal.x;
		body.at(index - 2).by = binormal.y;
		body.at(index - 2).bz = binormal.z;

		body.at(index - 3).nx = normal.x;
		body.at(index - 3).ny = normal.y;
		body.at(index - 3).nz = normal.z;
		body.at(index - 3).tx = tangent.x;
		body.at(index - 3).ty = tangent.y;
		body.at(index - 3).tz = tangent.z;
		body.at(index - 3).bx = binormal.x;
		body.at(index - 3).by = binormal.y;
		body.at(index - 3).bz = binormal.z;
	}
}

void Terrain::calculateTangentBinormal(NM_Vertex vertex1, NM_Vertex vertex2, NM_Vertex vertex3, DirectX::XMFLOAT3 & tangent, DirectX::XMFLOAT3 & binormal, DirectX::XMFLOAT3 & normal)
{
	// Calculate the two vectors for this face.
	float vector1[3];
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;
	float vector2[3];
	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// Calculate the tu and tv texture space vectors.
	float tuVector[2];
	float tvVector[2];
	tuVector[0] = vertex2.u - vertex1.u;
	tvVector[0] = vertex2.v - vertex1.v;
	tuVector[1] = vertex3.u - vertex1.u;
	tvVector[1] = vertex3.v - vertex1.v;

	// Calculate the denominator of the tangent/binormal equation.
	float den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	float length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;
}

void Terrain::calculateNormal(DirectX::XMFLOAT3 tangent, DirectX::XMFLOAT3 binormal, DirectX::XMFLOAT3 & normal)
{
	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	float length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;
}

Terrain::Terrain()
{
	this->vertexBuffer = nullptr;
	this->indexBuffer = nullptr;
	this->SamplerState = nullptr;

	this->terrainFilename = nullptr;
	this->heightMap = nullptr;
	//this->terrainModel = nullptr;

	//all this map stuff really needed?
	DirectX::XMVECTOR rotaxis = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMMATRIX rotTemp = DirectX::XMMatrixRotationAxis(rotaxis, 0);
	DirectX::XMMATRIX scaleTemp = DirectX::XMMatrixScaling(0.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX translTemp = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&this->Rotation, rotTemp);
	DirectX::XMStoreFloat4x4(&this->Scale, scaleTemp);
	DirectX::XMStoreFloat4x4(&this->Translation, translTemp);
	this->position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	scaleTemp = DirectX::XMLoadFloat4x4(&this->Scale);
	translTemp = DirectX::XMLoadFloat4x4(&this->Translation);
}

Terrain::~Terrain()
{
}

bool Terrain::Initialize(ID3D11Device *&device, char* fileName)
{
	bool result = false;
	result = LoadSetupFile(fileName);
	if (!result)
		return false;

	result = LoadBitmapHeightMap();
	if (!result)
		return false;
	SetTerrainCoordinates();

	// Now build the 3D model of the terrain.
	result = BuildTerrainModel();
	if (!result)
		return false;

	if (this->heightMap)
	{
		delete[] heightMap;
		this->heightMap = nullptr;
	}

	// Initialize the vertex and index buffer that hold the geometry for the terrain.
	result = InitializeBuffers(device);
	if (result)
		result = true;

	// Release the terrain model now that the rendering buffers have been loaded.
	this->texture.cleanUp();
	this->normal.cleanUp();
	return result;
}

//void Terrain::Render(ColorShader & shader, ID3D11DeviceContext * deviceContext)
//{
//	// Set vertex buffer stride and offset.
//	unsigned int stride = sizeof(Vertex3D);
//	unsigned int offset = 0;
//
//	deviceContext->PSSetShaderResources(0, 1, &this->texture.getTexture());
//	deviceContext->PSSetShaderResources(1, 1, &this->normal.getTexture());
//	// Set the vertex buffer to active in the input assembler so it can be rendered.
//	deviceContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);
//
//	// Set the index buffer to active in the input assembler so it can be rendered.
//	deviceContext->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
//
//	// Set the type of primitive that should be rendered from this vertex buffer, in this case a line list.
//	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);// D3D11_PRIMITIVE_TOPOLOGY_LINELIST); //D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);// 
//
//	deviceContext->PSSetSamplers(0, 1, &this->SamplerState);
//	shader.RenderShader(deviceContext, indexCount);
//}

void Terrain::Render(DeferedShader & shader, ID3D11DeviceContext * deviceContext)
{
	// Set vertex buffer stride and offset.
	unsigned int stride = sizeof(Vertex3D);
	unsigned int offset = 0;

	deviceContext->PSSetShaderResources(0, 1, &this->texture.getTexture());
	deviceContext->PSSetShaderResources(1, 1, &this->normal.getTexture());
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case a line list.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);// D3D11_PRIMITIVE_TOPOLOGY_LINELIST); //D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);// 

	deviceContext->PSSetSamplers(0, 1, &this->SamplerState);
	shader.RenderShader(deviceContext, indexCount);
	//this->calculateModelVectors();
}

//int Terrain::GetIndexCount()
//{
//	return this->indexCount;
//}

void Terrain::setTheTexture(ID3D11Device *& gDevice, ID3D11DeviceContext *&gDeviceContext, std::string filename, std::string normalFileName)
{
	this->texture.setTexture(gDevice, gDeviceContext, filename);
	this->normal.setTexture(gDevice, gDeviceContext, normalFileName);
}

void Terrain::Shutdown()
{
	// Release the index buffer.
	if (this->indexBuffer)
	{
		this->indexBuffer->Release();
		this->indexBuffer = nullptr;
	}

	// Release the vertex buffer.
	if (this->vertexBuffer)
	{
		this->vertexBuffer->Release();
		this->vertexBuffer = nullptr;
	}

	if (this->SamplerState != nullptr)
	{
		this->SamplerState->Release();
		this->SamplerState = nullptr;
	}
	this->texture.cleanUp();
	this->normal.cleanUp();
	if (this->heightMap)
	{
		delete[] heightMap;
		this->heightMap = nullptr;
	}
}

bool Terrain::checkCollision(DirectX::XMFLOAT3 camPos)
{
	bool result = false;
	bool found = false;
	int faceCount = terrainWidth* 5 * 6;//this->vertexCount/3;
	int index = 0;
	DirectX::XMFLOAT3 vertex1 = {};
	DirectX::XMFLOAT3 vertex2 = {};
	DirectX::XMFLOAT3 vertex3 = {};

	for (int i =0; i < faceCount&&found ==false; i++)
	{	
		vertex1.x = body.at(index).x;
		vertex1.y = body.at(index).y;
		vertex1.z = body.at(index).z;
		index++;
		vertex2.x = body.at(index).x;
		vertex2.y = body.at(index).y;
		vertex2.z = body.at(index).z;
		index++;
		vertex3.x = body.at(index).x;
		vertex3.y = body.at(index).y;
		vertex3.z = body.at(index).z;
		index++;




		//DirectX::XMFLOAT3 u = (vertex2.z - vertex1.z);
		//DirectX::XMFLOAT3 v= (vertex3.z - vertex1.z);
		//DirectX::XMFLOAT3 w = (camPos.z - vertex1);
		

		float w1 = ((vertex1.x*(vertex3.z - vertex1.z)) + ((camPos.z - vertex1.z)*(vertex3.x - vertex1.x)) - (camPos.x*(vertex3.z - vertex1.z))) /
			(((vertex2.z - vertex1.z)*(vertex3.x - vertex1.x)) - ((vertex2.x - vertex1.x)*(vertex3.z - vertex1.z)));
		
		float temp = vertex3.z - vertex1.z;
		if (temp == 0.f)
			temp = 0.001f;
		
		float w2 = (camPos.z - vertex1.z - (w1 * (vertex2.z - vertex1.z))) /
			temp;
		if (w1 >= 0 && w2 >= 0 && (w1 + w2) <= 1.f)
		{
 			found = true;
		}
	}
	// IMPLEMENT HERE
	if (found == true)
	{
		DirectX::XMFLOAT3 rayDir = DirectX::XMFLOAT3(0, -1.f, 0);
		DirectX::XMFLOAT3 w;  // ray vectors
		float r; // params to calc ray-plane intersect

		// tri vectors and normal
		DirectX::XMFLOAT3 u = DirectX::XMFLOAT3(vertex2.x - vertex1.x, vertex2.y - vertex1.y, vertex2.z - vertex1.z);
		DirectX::XMFLOAT3 v = DirectX::XMFLOAT3(vertex3.x - vertex1.x, vertex3.y - vertex1.y, vertex3.z - vertex1.z);
		DirectX::XMFLOAT3 normal;
		DirectX::XMStoreFloat3(&normal, DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&u), DirectX::XMLoadFloat3(&v)));

		DirectX::XMFLOAT3 w0 = DirectX::XMFLOAT3(camPos.x - vertex1.x, camPos.y - vertex1.y, camPos.z - vertex1.z); //ray vector from origin to tri
		float a;// = -dot(normal, w0);
		DirectX::XMStoreFloat(&a, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&normal), DirectX::XMLoadFloat3(&w0)));
		a = -a;

		float b;// = dot(normal, rayDir);
		DirectX::XMStoreFloat(&b, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&normal), DirectX::XMLoadFloat3(&rayDir)));

		if (abs(b) < 0)
			result = false;
		r = a / b;
		if (r < 0.f)
			result = false;

		DirectX::XMFLOAT3 I = DirectX::XMFLOAT3(camPos.x + r * rayDir.x, camPos.y + r * rayDir.y, camPos.z + r * rayDir.z);
		float uu, uv, vv, wu, wv, D;
		//uu = dot(u, u);
		DirectX::XMStoreFloat(&uu, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&u), DirectX::XMLoadFloat3(&u)));
		//uv = dot(u, v);
		DirectX::XMStoreFloat(&uv, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&u), DirectX::XMLoadFloat3(&v)));
		//vv = dot(v, v);
		DirectX::XMStoreFloat(&vv, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&v), DirectX::XMLoadFloat3(&v)));

		w = DirectX::XMFLOAT3(I.x - vertex1.x, I.y - vertex1.y, I.z - vertex1.z);
		//wu = dot(w, u);
		DirectX::XMStoreFloat(&wu, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&w), DirectX::XMLoadFloat3(&u)));
		//wv = dot(w, v);
		DirectX::XMStoreFloat(&wv, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&w), DirectX::XMLoadFloat3(&v)));
		D = uv * uv - uu * vv;

		//parametric coords
		float s = (uv * wv - vv * wu) / D;
		float t = (uv * wu - uu * wv) / D;
		if (s < 0.f || s > 1.f)
			result = false;
		if (t < 0.f || (s + t) > 1.f)
			result = false;
		if (r > 2.f&&r < 4.f)
			result = true;
		else
			result = false;

	}
	return result;
}

void Terrain::setWorld()
{
	DirectX::XMMATRIX rotTemp;
	DirectX::XMMATRIX scaleTemp;
	DirectX::XMMATRIX translTemp;
	rotTemp = DirectX::XMLoadFloat4x4(&this->Rotation);
	scaleTemp = DirectX::XMLoadFloat4x4(&this->Scale);
	translTemp = DirectX::XMLoadFloat4x4(&this->Translation);
	DirectX::XMStoreFloat4x4(&this->world, (translTemp));
}

DirectX::XMFLOAT4X4 Terrain::getWorld()
{
	return this->world;
}

void Terrain::setSampler(ID3D11Device *& gDevice)
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = gDevice->CreateSamplerState(&desc, &this->SamplerState);
	if (FAILED(hr))
	{
//		MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", "error", MB_OK);
	}
}

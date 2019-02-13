#include"CubeHandler.h"
CubeHandler::CubeHandler()
{
	this->cubeCap = 10;
	this->nrOfCubes = 0;
	this->cubes = new Cube[this->cubeCap];
}

CubeHandler::~CubeHandler()
{
}

void CubeHandler::addCube(DirectX::XMFLOAT3 pos, float width, float height, float depth)
{
	if (this->cubeCap == this->nrOfCubes)
	{
		this->expand();
	}
	this->cubes[this->nrOfCubes] = Cube(pos, width, height, depth);
	this->nrOfCubes++;
}

bool CubeHandler::insertVertexBuffer(ID3D11Device *& gDevice, ID3D11Buffer *& gVertexBuffer, int & vertexCount)
{
	int nrOfVertex = this->nrOfCubes * 6 * 6;
	vertexCount = nrOfVertex;
	Vertex3D *temp = new Vertex3D[nrOfVertex];
	int vertices = 0;
	for (int i = 0; i < this->nrOfCubes; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				for (int l = 0; l < 3; l++)
				{
					temp[vertices] = this->cubes[i].getQuad(j).getTri(k).getPoint(l);
					vertices++;
				}
			}
		}
	}
	D3D11_SUBRESOURCE_DATA data;
	HRESULT hr;
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = nrOfVertex * sizeof(Vertex3D);
	data.pSysMem = temp;
	hr = gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

void CubeHandler::expand()
{
	this->cubeCap += 5;
	Cube* temp = new Cube[this->cubeCap];
	for (int i = 0; i < this->nrOfCubes; i++)
	{
		temp[i] = this->cubes[i];
	}
	delete[]this->cubes;
	this->cubes = temp;
}

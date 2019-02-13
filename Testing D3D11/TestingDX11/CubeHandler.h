#ifndef CUBEHANDLER_H
#define CUBEHANDLER_H
#include"Cube.h"
class CubeHandler
{
public:
	CubeHandler();
	~CubeHandler();
	void addCube(DirectX::XMFLOAT3 pos, float width, float height, float depth);
	bool insertVertexBuffer(ID3D11Device *& gDevice, ID3D11Buffer *& gVertexBuffer, int & vertexCount);
private:
	void expand();
	Cube *cubes;
	int cubeCap;
	int nrOfCubes;
};
#endif // !CUBEHANDLER_H

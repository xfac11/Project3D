#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
	this->vertexBuffer = nullptr;
	this->indexBuffer = nullptr;
}

int Model::GetIndexCount()
{
	return this->indexCount;
}

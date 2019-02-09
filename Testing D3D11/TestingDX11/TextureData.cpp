#include "TextureData.h"

TextureData::TextureData(unsigned int width, unsigned int height, unsigned char data[], int size)
{
	this->IMAGE_WIDTH = width;
	this->IMAGE_HEIGHT = height;
	this->IMAGE_DATA = new unsigned char[size];
	for(int i=0;i<size;i++)
		IMAGE_DATA[i] = data[i];
}

TextureData::~TextureData()
{

	delete[]IMAGE_DATA;
}

void TextureData::setWIDTH(unsigned int IMAGE_WIDTH)
{
	this->IMAGE_WIDTH = IMAGE_WIDTH;
}

void TextureData::setHEIGHT(unsigned int IMAGE_HEIGHT)
{
	this->IMAGE_HEIGHT = IMAGE_HEIGHT;
}

void TextureData::setDATA(unsigned char data[], int size)
{
	for (int i = 0; i < size; i++)
		IMAGE_DATA[i] = data[i];
}

unsigned int TextureData::getWidth() const
{
	return this->IMAGE_WIDTH;
}

unsigned int TextureData::getHeight() const
{
	return this->IMAGE_HEIGHT;
}

unsigned char *& TextureData::getDATA()
{
	return this->IMAGE_DATA;
}

int TextureData::getSize() const
{
	return sizeof(IMAGE_DATA);
}

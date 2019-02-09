#ifndef TEXTUREDATA_H
#define TEXTUREDATA_H
class TextureData
{
private:
	unsigned int IMAGE_WIDTH;
	unsigned int IMAGE_HEIGHT;

	//Image data stored in 8-bit RGBA format
	unsigned int size;
	unsigned char* IMAGE_DATA;
public:
	TextureData(unsigned int width, unsigned int height, unsigned char data[], int size);
	~TextureData();

	void setWIDTH(unsigned int IMAGE_WIDTH);
	void setHEIGHT(unsigned int IMAGE_HEIGHT);
	void setDATA(unsigned char data[], int size);

	unsigned int getWidth()const;
	unsigned int getHeight()const;
	unsigned char *&getDATA();
	int getSize()const;
};
#endif
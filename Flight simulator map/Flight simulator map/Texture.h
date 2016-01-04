#pragma once

#include "Environment.h"
#include <atomic>
class Texture
{
public:
	Texture(unsigned char* source, int width, int height);
	Texture();
	~Texture();
	GLuint id;
	static void Decompress(char* source, long unsigned int size, unsigned char*& buffer, int& width, int& height);
protected:
private:
	static std::list<Texture*> Textures;

};


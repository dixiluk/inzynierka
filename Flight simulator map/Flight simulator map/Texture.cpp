#include "Texture.h"
#include "Chunk.h"
#include <libjpeg-turbo\turbojpeg.h>


Texture::Texture(unsigned char* source, int width, int height)
{
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);		//bindowanie i ustawianie parametrow tekstury


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenerateMipmap(GL_TEXTURE_2D);
	free(source);
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->id);
}

void Texture::Decompress(char* source, long unsigned int size, unsigned char*& buffer, int& width, int& height) {
	int jpegSubsamp;

	tjhandle jpegDecompressor = tjInitDecompress();

	char * secondBuffer = (char*)malloc(size);
	memcpy(secondBuffer, source, size);

	tjDecompressHeader2(jpegDecompressor, (unsigned char*)source, size, &width, &height, &jpegSubsamp);

	buffer = (unsigned char*)malloc(width*height * 4); //!< will contain the decompressed image
	
	tjDecompress2(jpegDecompressor, (unsigned char*)secondBuffer, size, buffer, width, 0, height, TJPF_RGBX, TJFLAG_FASTDCT | TJFLAG_BOTTOMUP);
	free(secondBuffer);
	tjDestroy(jpegDecompressor);
}



Texture::Texture()
{
	this->id = 0;
}
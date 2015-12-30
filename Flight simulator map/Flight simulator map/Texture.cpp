#include "Texture.h"
#include "Chunk.h"
#include <libjpeg-turbo\turbojpeg.h>

Texture::Texture(char* source, long unsigned int size)
{
	glGenTextures(1, &this->id);



	int jpegSubsamp, width, height;

	tjhandle jpegDecompressor = tjInitDecompress();

	tjDecompressHeader2(jpegDecompressor, (unsigned char*)source, size, &width, &height, &jpegSubsamp);

	unsigned char* buffer = (unsigned char*)malloc(width*height * 3); //!< will contain the decompressed image

	tjDecompress2(jpegDecompressor, (unsigned char*)source, size, buffer, width, 0/*pitch*/, height, TJPF_RGB, TJFLAG_FASTDCT);

	tjDestroy(jpegDecompressor);

	glBindTexture(GL_TEXTURE_2D, this->id);		//bindowanie i ustawianie parametrow tekstury
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,	//generowanie tekstury
		0, GL_BGRA, GL_UNSIGNED_BYTE, buffer);

}

Texture::~Texture()
{

}
Texture::Texture()
{
	this->id = 0;
}
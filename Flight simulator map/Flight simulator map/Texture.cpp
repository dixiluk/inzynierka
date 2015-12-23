#include "Texture.h"

#include "Chunk.h"

fi_handle Texture::LoadAdress;

inline unsigned _stdcall streamReadProc(void *buffer, unsigned size, unsigned count, fi_handle handle) {
	BYTE *tmp = (BYTE *)buffer;

	for (unsigned c = 0; c < count; c++) {
		memcpy(tmp, Texture::LoadAdress, size);
		Texture::LoadAdress = (BYTE *)Texture::LoadAdress + size;
		tmp += size;
	}
	return count;
}

inline unsigned _stdcall streamWriteProc(void *buffer, unsigned size, unsigned count, fi_handle handle) {
	return size;
}

inline int _stdcall streamSeekProc(fi_handle handle, long offset, int origin) {
	assert(origin != SEEK_END);

	if (origin == SEEK_SET) {
		Texture::LoadAdress = (BYTE *)handle + offset;
	}
	else {
		Texture::LoadAdress = (BYTE *)Texture::LoadAdress + offset;
	}

	return 0;
}

inline long _stdcall streamTellProc(fi_handle handle) {
	assert((int)handle > (int)Texture::LoadAdress);
	return ((int)Texture::LoadAdress - (int)handle);
}

Texture::Texture(char* source, void* tmp)
{
	Chunk* tmp8 = (Chunk*)tmp;
	glGenTextures(1, &this->id);

	FREE_IMAGE_FORMAT fif = FIF_JPEG;
	FIBITMAP *dib(0);
	BYTE* bits(0);
	unsigned int width(0), height(0);
	GLuint gl_texID;

		FreeImageIO io;

		io.read_proc = streamReadProc;
		io.write_proc = streamWriteProc;
		io.tell_proc = streamTellProc;
		io.seek_proc = streamSeekProc;

		char* cpy = (char*)malloc(*(int*)tmp8->satelliteImage->header);
		memcpy(cpy, source, *(int*)tmp8->satelliteImage->header);

		Texture::LoadAdress = (char*)cpy;


		EXCEPTION_POINTERS * eps = 0;
		__try {

		dib = FreeImage_LoadFromHandle(FIF_JPEG, &io, (fi_handle)cpy);
		}

		__except (eps = GetExceptionInformation(), EXCEPTION_EXECUTE_HANDLER) {
			return;


		}


	if (!dib)
		ReportWarning("Could not load texture");

	bits = FreeImage_GetBits(dib); //rozmiar piksela
	width = FreeImage_GetWidth(dib);	//wielkosc tekstury
	height = FreeImage_GetHeight(dib);


	glBindTexture(GL_TEXTURE_2D, this->id);		//bindowanie i ustawianie parametrow tekstury
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,	//generowanie tekstury
		0, GL_BGR, GL_UNSIGNED_BYTE, bits);


	FreeImage_Unload(dib);
	//free((char*)source);
}

Texture::~Texture()
{

}
Texture::Texture()
{
	this->id = 0;
}
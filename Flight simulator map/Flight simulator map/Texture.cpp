#include "Texture.h"

Texture::Texture(const char* filename, GLenum image_format, GLint internal_format, GLint level, GLint border)
{
	glGenTextures(1, &this->id);

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP *dib(0);
	BYTE* bits(0);
	unsigned int width(0), height(0);
	GLuint gl_texID;

	fif = FreeImage_GetFileType(filename, 0);  //pobieranie typu tekstury
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	if (fif == FIF_UNKNOWN){
		ReportWarning("Texture file format undefined");
		return;
	}


	if (FreeImage_FIFSupportsReading(fif))	//sprawdza czy moze odczytac
		dib = FreeImage_Load(fif, filename);	//odczytuje

	if (!dib)
		ReportWarning("Could not load texture");

	bits = FreeImage_GetBits(dib); //rozmiar piksela
	width = FreeImage_GetWidth(dib);	//wielkosc tekstury
	height = FreeImage_GetHeight(dib);

	
	glBindTexture(GL_TEXTURE_2D, this->id);		//bindowanie i ustawianie parametrow tekstury
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,	//generowanie tekstury
		border, image_format, GL_UNSIGNED_BYTE, bits);


	FreeImage_Unload(dib);
}

Texture::~Texture()
{

}


GLuint Texture::getId()
{
	return id;
}

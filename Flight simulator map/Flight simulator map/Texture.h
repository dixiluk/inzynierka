#pragma once

#include "Environment.h"


class Texture
{
public:
	Texture(const char* filename, GLenum image_format = GL_BGRA,
		GLint internal_format = GL_RGB, GLint level = 0, GLint border = 0);
	~Texture();
	void bind();
	GLuint getId();
	GLuint id;
protected:
private:
	static std::list<Texture*> Textures;

};


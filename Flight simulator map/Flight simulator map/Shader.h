#pragma once
#include "Environment.h"
#include <list>

class Shader
{
public:
	GLenum program;
	GLenum vertexShader;
	GLenum pixelShader;
	Shader(char* vsFileName, char* psFileName);
	~Shader();
	virtual void onPrepare(void* invoker);
};

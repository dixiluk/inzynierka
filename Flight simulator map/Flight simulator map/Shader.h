#pragma once
#include "Environment.h"
#include <list>

class Shader
{
public:
	static std::list<Shader*> Shaders;
	GLenum program;
	GLenum vertexShader;
	GLenum pixelShader;
	Shader(char* vsFileName, char* psFileName);
	~Shader();
	virtual void onPrepare(void* invoker);
	virtual void onDraw(void* invoker);
};

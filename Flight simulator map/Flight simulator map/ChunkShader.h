#pragma once
#include "Shader.h"

class ChunkShader : public Shader
{
	friend class Chunk;
private:
	GLuint mvpLink;
	GLuint textureLink;
public:
	void onPrepare(void* ptr);
	ChunkShader();
	~ChunkShader();
};


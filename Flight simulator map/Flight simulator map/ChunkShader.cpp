#include "ChunkShader.h"


ChunkShader::ChunkShader() : Shader ("ChunkShader.vs", "ChunkShader.ps")
{
	this->mvpLink = glGetUniformLocation(this->program, "MVP");
	this->textureLink = glGetUniformLocation(this->program, "Texture");
}


ChunkShader::~ChunkShader()
{
}


void ChunkShader::onPrepare(void* ptr){


}

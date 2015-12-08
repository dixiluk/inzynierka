#include "Chunk.h"
#include "Environment.h"
#include "Camera.h"
#include "ChunkShader.h"

ChunkShader *Chunk::Shader = NULL;

Chunk *Chunk::worldChunk;

Chunk::Chunk(Coordinate southWest, Coordinate northEast)
{
	this->worldChunk = this;
	this->northEast = northEast;
	this->southWest = southWest;
}


Chunk::~Chunk()
{
}

void Chunk::draw() {
	glUseProgram(Shader->program);

	glm::mat4 ModelMatrix;
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));
	//ModelMatrix = glm::rotate(ModelMatrix, (float)(-1.57079633f*object->rotationAxis.x), glm::vec3(1, 0, 0)); //Macierz modelu

	glm::mat4 ModelViewProjMatrix =
		Camera::ActiveCamera->projectionMatrix *
		Camera::ActiveCamera->viewMatrix *
		ModelMatrix; // MVP

	// Przesy³anie macierza MVP do programu shadera


	glUniformMatrix4fv(Shader->mvpLink, 1, GL_FALSE, glm::value_ptr(ModelViewProjMatrix));

	glEnable(GL_TEXTURE);
	// Przesy³anie tekstury do programu shadera
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, this->satelliteImage->texture->id);
	//glUniform1i(Shader->textureLink, 0);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(0, 5, 0);
	glTexCoord2f(1, 1); glVertex3f(5, 5, 0);
	glTexCoord2f(1, 0); glVertex3f(5, 0, 0);
	glEnd();
}

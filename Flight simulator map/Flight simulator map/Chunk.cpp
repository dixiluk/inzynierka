#include "Chunk.h"
#include "Environment.h"
#include "Camera.h"
#include "ChunkShader.h"
#include "GraphicalEngine.h"
#include "HttpRequester.h"
ChunkShader *Chunk::Shader = NULL;


Chunk::Chunk(Coordinate southWest, Coordinate northEast, Chunk* parent)
{
	this->northEast = northEast;
	this->southWest = southWest;
	this->satelliteImage = new SatelliteImage();
	this->parent = parent;
	for (int i = 0; i < 4; i++) {
		this->child[i] = NULL;
	}
	this->childExist = false;
}


Chunk::~Chunk()
{
}


void Chunk::createChild() {
	Coordinate tmpCord = Coordinate::calculateMidle(this->southWest, this->northEast);
	Coordinate tmpCord1 = this->northEast;
	tmpCord1.latitude -= tmpCord.latitude;
	Coordinate tmpCord2 = this->northEast;
	tmpCord2.longtitude -= tmpCord.longtitude;
	Coordinate tmpCord3 = this->southWest;
	tmpCord3.latitude += tmpCord.latitude;
	Coordinate tmpCord4 = this->southWest;
	tmpCord4.longtitude += tmpCord.longtitude;

	this->child[0] = new Chunk(this->southWest, tmpCord,this);
	this->child[1] = new Chunk(tmpCord3, tmpCord2, this);
	this->child[2] = new Chunk(tmpCord4, tmpCord1, this);
	this->child[3] = new Chunk(tmpCord, this->northEast, this);
	this->childExist = true;

}


void Chunk::draw() {
	if (this->childExist) {
		for (int i = 0; i < 4; i++) {
			this->child[i]->draw();
		}
	}
	else {
		glUseProgram(Shader->program);

		glm::mat4 ModelMatrix;
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//ModelMatrix = glm::rotate(ModelMatrix, (float)(-1.57079633f*object->rotationAxis.x), glm::vec3(1, 0, 0)); //Macierz modelu

		glm::mat4 ModelViewProjMatrix =
			Camera::ActiveCamera->projectionMatrix *
			Camera::ActiveCamera->viewMatrix *
			ModelMatrix; // MVP

						 // Przesy?anie macierza MVP do programu shadera


		glUniformMatrix4fv(Shader->mvpLink, 1, GL_FALSE, glm::value_ptr(ModelViewProjMatrix));
		glEnable(GL_TEXTURE);
		// Przesy?anie tekstury do programu shadera
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GraphicalEngine::Instance->worldChunk->satelliteImage->texture->id);
		glUniform1i(Shader->textureLink, 0);
		glColor3f(1, 0, 0);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);	glVertex3f(this->southWest.latitude, this->southWest.longtitude, 0);
		glTexCoord2f(0, 1);	glVertex3f(this->southWest.latitude, this->northEast.longtitude, 0);
		glTexCoord2f(1, 1);	glVertex3f(this->northEast.latitude, this->northEast.longtitude, 0);
		glTexCoord2f(1, 0);	glVertex3f(this->northEast.latitude, this->southWest.longtitude, 0);
		glEnd();
	}

}


void Chunk::loadHttpData() {
	HttpRequester * connection = new HttpRequester("", "");

	//this->satelliteImage = connection->getSatelliteImageSource()
	//return;
}
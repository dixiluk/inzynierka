#include "Chunk.h"
#include "Environment.h"
#include "Camera.h"
#include "ChunkShader.h"
#include "GraphicalEngine.h"
#include "MapLoader.h"

ChunkShader *Chunk::Shader = NULL;




Chunk::Chunk(Coordinate southWest, Coordinate northEast, Chunk* parent)
{
	this->northEast = northEast;
	this->southWest = southWest;
	this->parent = parent;
	for (int i = 0; i < 4; i++) {
		this->child[i] = NULL;
	}
	this->childExist = false;
	this->isDownloaded.store(false);
	this->isLoaded = false;
	this->elevationCols =32;
	this->elevationRows = 32;
	MapLoader::Instance->addLowPriorityTask(this);
}


Chunk::~Chunk()
{
}

void Chunk::test()
{
	if (this->childExist && this->isChildsLoaded()) {
		for (int i = 0; i < 4; i++) {
			this->child[i]->test();
		}
	}
	else {
		this->createChild();
	}
}


void Chunk::createChild() {
	Coordinate tmpCord = Coordinate::calculateMidle(this->southWest, this->northEast);
	Coordinate tmpCord1 = this->northEast;
	tmpCord1.latitude = tmpCord.latitude;
	Coordinate tmpCord2 = this->northEast;
	tmpCord2.longtitude = tmpCord.longtitude;
	Coordinate tmpCord3 = this->southWest;
	tmpCord3.latitude = tmpCord.latitude;
	Coordinate tmpCord4 = this->southWest;
	tmpCord4.longtitude = tmpCord.longtitude;

	this->child[0] = new Chunk(this->southWest, tmpCord,this);
	this->child[1] = new Chunk(tmpCord4, tmpCord1, this);
	this->child[2] = new Chunk(tmpCord3, tmpCord2, this);
	this->child[3] = new Chunk(tmpCord, this->northEast, this);


	this->childExist = true;

}

void Chunk::downloadChunk(HttpRequester* httpRequester)
{
	this->elevationData = httpRequester->getElevationData(this->southWest, this->northEast, this->elevationRows, this->elevationCols, "sealevel");
	

	this->vertices = new Vertex*[this->elevationRows];
	for (int row = 0; row < this->elevationRows; row++)
		this->vertices[row] = new Vertex[this->elevationCols];

	int earthRadius = 10;//6378410;
		for (int i = 0; i < this->elevationRows; i++)
			for (int j = 0; j < this->elevationCols; j++) {
				this->vertices[i][j].position = glm::vec3(
					(float)(earthRadius /*+ this->elevationData->heights[i][j]*/)*sin(this->elevationData->coordinates[i][j].longtitude)*sin(this->elevationData->coordinates[i][j].latitude),
					(float)(earthRadius /*+ this->elevationData->heights[i][j]*/)*cos(this->elevationData->coordinates[i][j].longtitude)*sin(this->elevationData->coordinates[i][j].latitude),
					(float)(earthRadius/* + this->elevationData->heights[i][j]*/)*cos(this->elevationData->coordinates[i][j].latitude));
		}


	this->satelliteImage = httpRequester->getSatelliteImageSource(this);

	float positionY;
	for (int i = 0; i < this->elevationRows; i++) {
		if (i == this->elevationRows - 1)
			positionY = 1;
		else
			positionY = (float)(1-(float)(this->satelliteImage->metadata->markers[1][i]- this->satelliteImage->metadata->markers[1][this->elevationRows-1])/(float)this->satelliteImage->metadata->sizeY());
		for (int j = 0; j < this->elevationCols; j++) {
			this->vertices[i][j].textureCoord = glm::vec2((float)j / (this->elevationCols - 1), positionY);
		}
	}
	this->isDownloaded.store(true);
}


void Chunk::loadChunk() {

	std::atomic_thread_fence(std::memory_order_acquire);
	if (this->childExist) {
		for (int i = 0; i < 4; i++) {
			this->child[i]->loadChunk();
		}
	}
	else {
		if (this->isDownloaded.load() && !this->isLoaded) {
			this->satelliteImage->texture = new Texture(this->satelliteImage->source,this);
			this->isLoaded = true;
		}
	}
}



void Chunk::draw() {
	if (this->childExist && this->isChildsLoaded()) {
		for (int i = 0; i < 4; i++) {
			this->child[i]->draw();
		}
	}
	else {
		if (!this->isLoaded) return;

		glUseProgram(Shader->program);

		glm::mat4 ModelMatrix;
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//ModelMatrix = glm::rotate(ModelMatrix, (float)(-1.57079633f*object->rotationAxis.x), glm::vec3(1, 0, 0)); //Macierz modelu

		glm::mat4 ModelViewProjMatrix =
			Camera::ActiveCamera->projectionMatrix *
			Camera::ActiveCamera->viewMatrix *
			ModelMatrix; // MVP

						 // Przesy³anie macierza MVP do programu shadera


		glUniformMatrix4fv(Shader->mvpLink, 1, GL_FALSE, glm::value_ptr(ModelViewProjMatrix));
		glEnable(GL_TEXTURE);
		// Przesy?anie tekstury do programu shadera
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->satelliteImage->texture->id);
		glUniform1i(Shader->textureLink, 0);
		glColor3f(1, 0, 0);
		glBegin(GL_QUAD_STRIP);
		bool side = true;
		
		for (int i = 1; i < this->elevationRows; i++) {
			for (int j = 0; j < this->elevationCols; j++) {
				glTexCoord2f(this->vertices[i][j].textureCoord.x, this->vertices[i][j].textureCoord.y);	glVertex3f(this->vertices[i][j].position.x, this->vertices[i][j].position.y, this->vertices[i][j].position.z);
				glTexCoord2f(this->vertices[i - 1][j].textureCoord.x, this->vertices[i - 1][j].textureCoord.y);	glVertex3f(this->vertices[i - 1][j].position.x, this->vertices[i - 1][j].position.y, this->vertices[i-1][j].position.z);
			//	std::cout << this->vertices[i][j].position.x << "  " <<this->vertices[i][j].position.y  << "  " << this->vertices[i][j].position.z << std::endl;
			//	std::cout << this->vertices[i - 1][j].position.x << "  " << this->vertices[i - 1][j].position.y << "  " << this->vertices[i - 1][j].position.z << std::endl;

			}
		}
	//	std::cout << std::endl;
			glEnd();

	}
}


void Chunk::loadHttpData() {
	HttpRequester * connection = new HttpRequester("", "");

	//this->satelliteImage = connection->getSatelliteImageSource()
	//return;
}

bool Chunk::isChildsLoaded()
{
	for (int i = 0; i < 4; i++) {
		if (!this->child[i]->isLoaded)
			return false;
	}
	return true;
}

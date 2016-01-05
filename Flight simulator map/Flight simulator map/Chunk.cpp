#include "Chunk.h"
#include "Environment.h"
#include "Camera.h"
#include "ChunkShader.h"
#include "GraphicalEngine.h"
#include "MapLoader.h"


ChunkShader *Chunk::Shader = NULL;

bool Chunk::saveDataOnDrive;
short Chunk::levelOfDetailCheckAccuracy;
short Chunk::levelOfDetailCheckPresent;
short Chunk::LevelOfDetailCount;
double Chunk::LevelOfDetail[20];


Chunk::Chunk(Coordinate southWest, Coordinate northEast, Chunk* parent)
{
	if (parent == NULL) {
		this->detailLevel = 0;
	}
	else {
		this->detailLevel = parent->detailLevel + 1;
	}
	this->levelOfDetailCheck = rand() % this->levelOfDetailCheckAccuracy;
	this->visible = true;
	this->toRemove = false;
	this->northEast = northEast;
	this->southWest = southWest;
	this->parent = parent;
	for (int i = 0; i < 4; i++) {
		this->child[i] = NULL;
	}
	this->childExist = false;
	this->isDownloaded.store(false);
	this->isLoaded = false;
	this->isNowDownloading.store(false);
}

Chunk::~Chunk()
{
	delete this->satelliteImage;
	delete this->elevationData;
	delete[] this->vertices;
}


void Chunk::loadLevelOfDetail()
{
	char * bufferName = (char*)malloc(1024);

	Chunk::LevelOfDetailCount = Config::Instance->takeConfigDouble("LevelOfDetailCount");
	for (int i = 0; i < Chunk::LevelOfDetailCount; i++) {

		sprintf(bufferName, "LevelOfDetail%d", i);
		Chunk::LevelOfDetail[i] = Config::Instance->takeConfigDouble(bufferName);
	}

	free(bufferName);
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

	this->child[0] = new Chunk(this->southWest, tmpCord, this);
	this->child[1] = new Chunk(tmpCord4, tmpCord1, this);
	this->child[2] = new Chunk(tmpCord3, tmpCord2, this);
	this->child[3] = new Chunk(tmpCord, this->northEast, this);


	this->childExist = true;

}

void Chunk::loadChildren() {

	for (int i = 0; i < 4; i++) {

		this->child[i]->isNowDownloading.store(true);
		MapLoader::Instance->addLowPriorityTask(this->child[i]);
	}
}

void Chunk::removeChild()
{
	if (!this->isLoaded) {
		if (!this->isDownloaded && !this->isNowDownloading) {
			this->isNowDownloading.store(true);
			MapLoader::Instance->addHighPriorityTask(this);
		}
	}
	else {

		//delete  this->child[0];
		//delete  this->child[1];
		//delete  this->child[2];
		//delete  this->child[3];
		this->childExist = false;
	}

}

void Chunk::downloadChunk(HttpRequester* httpRequester)
{
	char * bufferName = (char*)malloc(1024);
	sprintf(bufferName, "Data/%f.%f.%f.%f", this->southWest.latitude, this->southWest.longtitude, this->northEast.latitude, this->northEast.longtitude);

	char * buffer = (char*)malloc(100);

	//sprintf(buffer, "%s.%d,%d.elevation", bufferName, this->elevationData->rows, this->elevationData->cols);
	sprintf(buffer, "%s%d.elevation", "Data/", 1);
	this->elevationData = ElevationData::readFromDrive(this->southWest, this->northEast, buffer);

	if (this->elevationData == NULL) {
		this->elevationData = httpRequester->getElevationData(this->southWest, this->northEast, "sealevel");
		if (this->saveDataOnDrive) {
			this->elevationData->saveOnDrive(buffer);
		}


	}

	this->calculateVertexArrayVertices();
	this->calculateVertexArrayIndices();


	free(buffer);
	buffer = (char*)malloc(100);
	//sprintf(buffer, "%s.jpeg", bufferName);
	sprintf(buffer, "Data/%d.jpeg", this->detailLevel);
	char * buffer2 = (char*)malloc(100);
	//sprintf(buffer2, "%s.metadata", bufferName);
	sprintf(buffer2, "Data/%d.metadata", this->detailLevel);

	this->satelliteImage = SatelliteImage::readFromDrive(buffer, buffer2);

	if (this->satelliteImage == NULL) {
		this->satelliteImage = httpRequester->getSatelliteImageSource(this);
		if (this->saveDataOnDrive) {
			this->satelliteImage->saveOnDrive(buffer);
			this->satelliteImage->metadata->saveOnDrive(buffer2);
		}


	}
	unsigned char* decompressedBuffer;

	Texture::Decompress(this->satelliteImage->source, this->satelliteImage->sourceSize, decompressedBuffer,
		this->satelliteImage->width, this->satelliteImage->height);
	free(this->satelliteImage->source);
	this->satelliteImage->source = (char*)decompressedBuffer;

	this->calculateVertexArrayTextureCoordinants();


	this->isDownloaded.store(true);


	free(bufferName);
	free(buffer);
	free(buffer2);

}

int tmp = 0;
void Chunk::loadChunk() {
	std::atomic_thread_fence(std::memory_order_acquire);
	if (this->childExist) {
		for (int i = 0; i < 4; i++) {
			this->child[i]->loadChunk();
		}
		if (this->isDownloaded.load() && !this->isLoaded) {
			std::cout << tmp++ << std::endl;
			this->satelliteImage->texture = new Texture((unsigned char*)this->satelliteImage->source, this->satelliteImage->width, this->satelliteImage->height);
			this->loadVertexArray();
			this->isLoaded = true;
		}
	}
	else {
		if (this->isDownloaded.load() && !this->isLoaded) {
			std::cout << tmp++ << std::endl;
			this->satelliteImage->texture = new Texture((unsigned char*)this->satelliteImage->source, this->satelliteImage->width, this->satelliteImage->height);
			this->loadVertexArray();
			this->isLoaded = true;
		}
	}
}



void Chunk::draw() {
	if (this->isChildrenLoaded()) {
		for (int i = 0; i < 4; i++) {
			this->child[i]->draw();
		}
	}
	else {
		if (!this->isLoaded || !this->visible) return;

		glUseProgram(Shader->program);

		glm::mat4 ModelMatrix;
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		glm::mat4 ModelViewProjMatrix =
			Camera::ActiveCamera->projectionMatrix *
			Camera::ActiveCamera->viewMatrix *
			ModelMatrix;

		glUniformMatrix4fv(Shader->mvpLink, 1, GL_FALSE, glm::value_ptr(ModelViewProjMatrix));
		glEnable(GL_TEXTURE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->satelliteImage->texture->id);
		glUniform1i(Shader->textureLink, 0);

		glBindVertexArray(this->vertexArrayId);
		glDrawElements(GL_QUADS, this->indicesSize, GL_UNSIGNED_INT, 0); //Rysowanie obiektu z bufora wierzcho³ków
		glBindVertexArray(0);
	}
}


void Chunk::loadHttpData() {
	HttpRequester * connection = new HttpRequester("", "");

	//this->satelliteImage = connection->getSatelliteImageSource()
	//return;
}

bool Chunk::isChildrenDowdloaded()
{
	if (!this->childExist)
		return false;

	for (int i = 0; i < 4; i++) {
		if (!this->child[i]->isDownloaded) {//jezeli dziecko nie jest zaladowane			
			if (!this->child[i]->isChildrenDowdloaded())	//jezeli dzieci dziecka nie sa zaladowane
				return false;
		}
	}

	return true;
}
bool Chunk::isChildrenLoaded()
{
	if (!this->childExist)
		return false;

	for (int i = 0; i < 4; i++) {
		if (!this->child[i]->isLoaded) {//jezeli dziecko nie jest zaladowane			
			if (!this->child[i]->isChildrenLoaded())	//jezeli dzieci dziecka nie sa zaladowane
					return false;
		}
	}
	return true;
}

bool Chunk::isChildrenToRemove()
{
	for (int i = 0; i < 4; i++) {
		if (!this->child[i]->toRemove)
			return false;
	}
	return true;
}

void Chunk::saveOnDrive(std::string path)
{

	this->satelliteImage->saveOnDrive(path + ".jpeg");
	this->satelliteImage->metadata->saveOnDrive(path + ".metadata");

	char * buffer = (char*)malloc(30);

	sprintf(buffer, ".%d,%d.elevation", this->elevationData->rows, this->elevationData->cols);
	this->elevationData->saveOnDrive(path + buffer);

	free(buffer);

}

double Chunk::distanceFromCamera()
{
	double distanceCameraFromCenter = glm::distance(Camera::ActiveCamera->position, glm::vec3(0, 0, 0));
	double latitude = acos(Camera::ActiveCamera->position.z / distanceCameraFromCenter);
	double longtitude = atan2(Camera::ActiveCamera->position.x, Camera::ActiveCamera->position.y);
	if (longtitude > 0) longtitude -= PI2 / 2;
	else  longtitude += PI2 / 2;
	Coordinate cameraCoordinates = Coordinate((latitude * 360 / PI2) - 90, (longtitude * 360 / PI2));
	double minDistance;
	double tmpDistance;
	minDistance = glm::distance(glm::vec3(this->vertices[0], this->vertices[1], this->vertices[2]), Camera::ActiveCamera->position);
	if (cameraCoordinates.latitude > this->southWest.latitude && cameraCoordinates.longtitude > this->southWest.longtitude&&this->northEast.latitude > cameraCoordinates.latitude &&this->northEast.longtitude > cameraCoordinates.longtitude) {
		for (int i = 1; i < ElevationData::rows; i++) {
			for (int j = 0; j < ElevationData::cols; j++) {
				tmpDistance = glm::distance(glm::vec3(this->vertices[i*ElevationData::cols * 3 + j * 3], this->vertices[i*ElevationData::cols * 3 + j * 3 + 1], this->vertices[i*ElevationData::cols * 3 + j * 3 + 2]), Camera::ActiveCamera->position);
				if (tmpDistance < minDistance)
					minDistance = tmpDistance;
			}
		}
	}
	else {
		for (int i = 1; i < ElevationData::rows; i++) {
			int j = ElevationData::cols - 1;
			tmpDistance = glm::distance(glm::vec3(this->vertices[i*ElevationData::cols * 3 + j * 3], this->vertices[i*ElevationData::cols * 3 +
				j * 3 + 1], this->vertices[i*ElevationData::cols * 3 + j * 3 + 2]), Camera::ActiveCamera->position);
			if (tmpDistance < minDistance)
				minDistance = tmpDistance;
			j = 0;
			tmpDistance = glm::distance(glm::vec3(this->vertices[i*ElevationData::cols * 3 + j * 3], this->vertices[i*ElevationData::cols * 3 +
				j * 3 + 1], this->vertices[i*ElevationData::cols * 3 + j * 3 + 2]), Camera::ActiveCamera->position);
			if (tmpDistance < minDistance)
				minDistance = tmpDistance;
		}
		for (int j = 0; j < ElevationData::cols; j++) {
			int i = 0;
			tmpDistance = glm::distance(glm::vec3(this->vertices[i*ElevationData::cols * 3 + j * 3], this->vertices[i*ElevationData::cols * 3 +
				j * 3 + 1], this->vertices[i*ElevationData::cols * 3 + j * 3 + 2]), Camera::ActiveCamera->position);
			if (tmpDistance < minDistance)
				minDistance = tmpDistance;
			i = ElevationData::rows - 1;
			tmpDistance = glm::distance(glm::vec3(this->vertices[i*ElevationData::cols * 3 + j * 3], this->vertices[i*ElevationData::cols * 3 +
				j * 3 + 1], this->vertices[i*ElevationData::cols * 3 + j * 3 + 2]), Camera::ActiveCamera->position);
			if (tmpDistance < minDistance)
				minDistance = tmpDistance;
		}


	}

	return minDistance;
}

short Chunk::calculatePrefDetailLevel() {
	double distance = this->distanceFromCamera();
	if(Camera::ActiveCamera->newHight == -1 || Camera::ActiveCamera->newHight >distance)
		Camera::ActiveCamera->newHight = distance;
	double cameraDistance = glm::distance(Camera::ActiveCamera->position, glm::vec3(0, 0, 0));
	double horizont;
	double minHorizont = (20100 / pow(2, this->detailLevel)) / 2 / ElevationData::cols;
	if (cameraDistance <= earthRadius)
		horizont = minHorizont;
	else
		horizont = sqrt(cameraDistance*cameraDistance - earthRadius*earthRadius);
	if (horizont < minHorizont)
		horizont = minHorizont;
	if (distance > horizont)return -1;
	for (int i = Chunk::LevelOfDetailCount - 1; i >= 0; i--) {
		if (distance < this->LevelOfDetail[i])
			return i;
	}
	return 0;
}

void Chunk::calculateAllDetails()
{


	if (this->childExist) {

		if (this->isChildrenToRemove()) {
			this->removeChild();
		}

		if (this->isChildrenDowdloaded())
			for (int i = 0; i < 4; i++) {
				this->child[i]->calculateAllDetails();
			}
		return;
	}

	if (this->levelOfDetailCheck != this->levelOfDetailCheckPresent) return;   // z jkim krokiem sprawdzamy

	if (!this->isDownloaded) return;

	short prefDetailLevel = this->calculatePrefDetailLevel();

	if (this->detailLevel < prefDetailLevel) {	//trzeba powiekszyc jakosc

		this->createChild();
		this->loadChildren();
	}

	if (this->detailLevel > prefDetailLevel + 1)	//chunk do usuniecia
		this->toRemove = true;

	if (prefDetailLevel == -1)		//chunk poza choryzontem
		this->visible = false;
	else {

		if (!this->isDownloaded) {
			if (!this->isNowDownloading) {
				this->isNowDownloading.store(true);
				MapLoader::Instance->addHighPriorityTask(this);
			}
		}
		else {
			this->visible = true;
		}
	}

}

void Chunk::calculateVertexArrayTextureCoordinants() {
	this->textureCoordinants = new float[sizeof(GLfloat)*this->verticesCount * 2];
	float positionY;
	for (int i = 0; i < ElevationData::rows; i++) {
		if (i == ElevationData::rows - 1)
			positionY = 1;
		else
			positionY = (float)(1 - (float)(this->satelliteImage->metadata->markers[1][i] - this->satelliteImage->metadata->markers[1][ElevationData::rows - 1]) / (float)this->satelliteImage->metadata->sizeY());
		for (int j = 0; j < ElevationData::cols; j++) {
			textureCoordinants[i*ElevationData::cols * 2 + j * 2] = (float)j / (ElevationData::cols - 1);
			textureCoordinants[i*ElevationData::cols * 2 + j * 2 + 1] = positionY;
		}
	}

}

void Chunk::calculateVertexArrayVertices() {
		this->verticesCount = this->elevationData->cols*this->elevationData->rows;
		this->vertices = new float[sizeof(GLfloat)*this->verticesCount * 3];
		for (int i = 0; i < ElevationData::rows; i++)
			for (int j = 0; j < ElevationData::cols; j++) {
				vertices[i*ElevationData::cols * 3 + j * 3] = (float)(earthRadius + (this->elevationData->heights[i][j] / 1000))*sin(this->elevationData->coordinates[i][j].longtitude)*sin(this->elevationData->coordinates[i][j].latitude);
				vertices[i*ElevationData::cols * 3 + j * 3 + 1] = (float)(earthRadius + (this->elevationData->heights[i][j] / 1000))*cos(this->elevationData->coordinates[i][j].longtitude)*sin(this->elevationData->coordinates[i][j].latitude);
				vertices[i*ElevationData::cols * 3 + j * 3 + 2] = (float)(earthRadius + (this->elevationData->heights[i][j]/1000))*cos(this->elevationData->coordinates[i][j].latitude);
			}


}


void Chunk::calculateVertexArrayIndices() {
	this->indicesSize = (ElevationData::rows - 1)*(ElevationData::cols - 1) * 4;
	this->indices = new GLuint[sizeof(GLuint) *indicesSize];

	for (int i = 0; i < ElevationData::rows - 1; i++)
		for (int j = 0; j < ElevationData::cols - 1; j++) {
			indices[i*(ElevationData::cols - 1) * 4 + j * 4] = i*(ElevationData::cols) + j;
			indices[i*(ElevationData::cols - 1) * 4 + j * 4 + 1] = (i + 1)*(ElevationData::cols) + j;
			indices[i*(ElevationData::cols - 1) * 4 + j * 4 + 2] = (i + 1)*(ElevationData::cols) + j + 1;
			indices[i*(ElevationData::cols - 1) * 4 + j * 4 + 3] = i*(ElevationData::cols) + j + 1;
		}
}

void Chunk::loadVertexArray() {
	GLuint  vertexLoc = 0, texCoordLoc = 1;
	glGenVertexArrays(1, &this->vertexArrayId);
	glBindVertexArray(this->vertexArrayId);


	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) *indicesSize, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * verticesCount, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * verticesCount, textureCoordinants, GL_STATIC_DRAW);
	glEnableVertexAttribArray(texCoordLoc);
	glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void Chunk::createBeginChunks() {
	this->elevationData = new ElevationData(this->southWest, this->northEast, "", 0);
	this->calculateVertexArrayVertices();

	short prefDetailLevel = this->calculatePrefDetailLevel();

	if (this->detailLevel < prefDetailLevel) {	//trzeba powiekszyc jakosc

		this->createChild();
		for (int i = 0; i < 4; i++) {
			this->child[i]->createBeginChunks();
		}
	}
	if (prefDetailLevel == -1)
		this->visible = false;
	else {
		this->visible = true;	
	}

	if (this->elevationData != NULL) {
		delete this->elevationData;
	}

}

void Chunk::loadBeginChunks() {
	if(this->childExist)
		for (int i = 0; i < 4; i++) {
			this->child[i]->loadBeginChunks();
		}
	else {
		this->isNowDownloading.store(true);
		MapLoader::Instance->addLowPriorityTask(this);
	}
}
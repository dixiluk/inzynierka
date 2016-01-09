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
	delete[] this->textureCoordinants;
	delete[] this->indices;
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
	tmpCord2.longitude = tmpCord.longitude;
	Coordinate tmpCord3 = this->southWest;
	tmpCord3.latitude = tmpCord.latitude;
	Coordinate tmpCord4 = this->southWest;
	tmpCord4.longitude = tmpCord.longitude;

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

		delete  this->child[0];
		this->child[0] = nullptr;
		delete  this->child[1];
		this->child[1] = nullptr;
		delete  this->child[2];
		this->child[2] = nullptr;
		delete  this->child[3];
		this->child[3] = nullptr;
		this->childExist = false;
	}

}

void Chunk::downloadChunk(HttpRequester* httpRequester)
{
	char * bufferName = (char*)malloc(1024);
	sprintf(bufferName, "Data/%f.%f.%f.%f", this->southWest.latitude, this->southWest.longitude, this->northEast.latitude, this->northEast.longitude);

	char * buffer = (char*)malloc(100);

	sprintf(buffer, "%s.%d,%d.elevation", bufferName, this->elevationData->rows, this->elevationData->cols);
	//sprintf(buffer, "%s%d.elevation", "Data/", 1);
	this->elevationData = ElevationData::readFromDrive(this->southWest, this->northEast, buffer);

	if (this->elevationData == NULL) {
		if (httpRequester == NULL) httpRequester = new HttpRequester(Config::Instance->takeConfigString("dataServer"), Config::Instance->takeConfigString("licenceKey"));
		this->elevationData = httpRequester->getElevationData(this->southWest, this->northEast, "sealevel");
		if (this->saveDataOnDrive) {
			this->elevationData->saveOnDrive(buffer);
		}


	}

	this->calculateVertexArrayVertices();
	this->calculateVertexArrayIndices();


	sprintf(buffer, "%s.jpeg", bufferName);
	//sprintf(buffer, "Data/%d.jpeg", this->detailLevel);



	char * buffer2 = (char*)malloc(100);
	sprintf(buffer2, "%s.metadata", bufferName);
	//sprintf(buffer2, "Data/%d.metadata", this->detailLevel);

	this->satelliteImage = SatelliteImage::readFromDrive(buffer, buffer2);

	bool textureAvailable = false;

	int sizeX = HttpRequester::prefImageSizeX;
	int sizeY = HttpRequester::prefImageSizeY;
	while (!textureAvailable) {
		if (this->satelliteImage == NULL) {
			if (httpRequester == NULL) httpRequester = new HttpRequester(Config::Instance->takeConfigString("dataServer"), Config::Instance->takeConfigString("licenceKey"));
			this->satelliteImage = httpRequester->getSatelliteImageSource(this, sizeX, sizeY);
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

		if (!this->satelliteImage->CheckImage()) {
			sizeX = sizeX / 2;
			sizeY = sizeY / 2;
			if (sizeX < 80 || sizeY < 80) {
				textureAvailable = true;
			}
			else {
				delete this->satelliteImage;
				this->satelliteImage = NULL;
			}
		}
		else
			textureAvailable = true;
	}
	this->calculateVertexArrayTextureCoordinants();


	this->isDownloaded.store(true);
	this->isNowDownloading.store(false);

	free(bufferName);
	free(buffer);
	free(buffer2);

}

void Chunk::loadChunk() {
	std::atomic_thread_fence(std::memory_order_acquire);
	if (this->childExist) {
		for (int i = 0; i < 4; i++) {
			this->child[i]->loadChunk();
		}
		if (this->isDownloaded.load() && !this->isLoaded) {
			this->satelliteImage->texture = new Texture((unsigned char*)this->satelliteImage->source, this->satelliteImage->width, this->satelliteImage->height);
			this->loadVertexArray();
			this->isLoaded = true;
		}
	}
	else {
		if (this->isDownloaded.load() && !this->isLoaded) {
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
	double longitude = atan2(Camera::ActiveCamera->position.x, Camera::ActiveCamera->position.y);
	if (longitude > 0) longitude -= PI2 / 2;
	else  longitude += PI2 / 2;
	Coordinate cameraCoordinates = Coordinate((latitude * 360 / PI2) - 90, (longitude * 360 / PI2));
	double minDistance;
	double tmpDistance;
	minDistance = glm::distance(glm::vec3(this->vertices[0], this->vertices[1], this->vertices[2]), Camera::ActiveCamera->position);
	if (cameraCoordinates.latitude >= this->southWest.latitude && cameraCoordinates.longitude >= this->southWest.longitude&&this->northEast.latitude >= cameraCoordinates.latitude &&this->northEast.longitude >= cameraCoordinates.longitude) {
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
	if (Camera::ActiveCamera->newHight == -1 || Camera::ActiveCamera->newHight >distance)
		Camera::ActiveCamera->newHight = distance;
	double cameraDistance = glm::distance(Camera::ActiveCamera->position, glm::vec3(0, 0, 0));
	double horizont;
	double pointDistance = (20100 / pow(2, this->detailLevel)) / ElevationData::cols;
	double minHorizont = sqrt(pointDistance*pointDistance+ (cameraDistance- earthRadius)*(cameraDistance- earthRadius));
	if (cameraDistance <= earthRadius)
		horizont = minHorizont;
	else
		horizont = sqrt(cameraDistance*cameraDistance - earthRadius*earthRadius);
	if (horizont < minHorizont)
		horizont = minHorizont;
	if (distance > horizont)
		return -1;
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
			if (this->isChildrenDowdloaded())
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
	this->textureCoordinants = new float[this->verticesCount * 2];
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
	this->vertices = new float[this->verticesCount * 3];
	for (int i = 0; i < ElevationData::rows; i++)
		for (int j = 0; j < ElevationData::cols; j++) {
			vertices[i*ElevationData::cols * 3 + j * 3] = (float)(earthRadius + (this->elevationData->heights[i][j] / 1000))*sin(this->elevationData->coordinates[i][j].longitude)*sin(this->elevationData->coordinates[i][j].latitude);
			vertices[i*ElevationData::cols * 3 + j * 3 + 1] = (float)(earthRadius + (this->elevationData->heights[i][j] / 1000))*cos(this->elevationData->coordinates[i][j].longitude)*sin(this->elevationData->coordinates[i][j].latitude);
			vertices[i*ElevationData::cols * 3 + j * 3 + 2] = (float)(earthRadius + (this->elevationData->heights[i][j] / 1000))*cos(this->elevationData->coordinates[i][j].latitude);
		}


}


void Chunk::calculateVertexArrayIndices() {
	this->indicesSize = (ElevationData::rows - 1)*(ElevationData::cols - 1) * 4;
	this->indices = new GLuint[indicesSize];

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
	if (this->childExist)
		for (int i = 0; i < 4; i++) {
			this->child[i]->loadBeginChunks();
		}
	else {
		this->isNowDownloading.store(true);
		MapLoader::Instance->addLowPriorityTask(this);
	}
}


void Chunk::downloadModecreateBeginChunks(Coordinate p1, Coordinate p2, short bestLevel) {

	if ((this->southWest.longitude < p1.longitude && this->southWest.latitude < p1.latitude && this->northEast.longitude > p1.longitude && this->northEast.latitude > p1.latitude)
		|| (this->southWest.longitude < p2.longitude && this->southWest.latitude < p2.latitude && this->northEast.longitude > p2.longitude && this->northEast.latitude > p2.latitude)
		|| (p1.longitude < this->southWest.longitude && p1.latitude < this->southWest.latitude && p2.longitude > this->southWest.longitude && p2.latitude > this->southWest.latitude)
		|| (p1.longitude < this->northEast.longitude && p1.latitude < this->northEast.latitude && p2.longitude > this->northEast.longitude && p2.latitude > this->northEast.latitude)
		) {
		if (this->detailLevel <= bestLevel) {
			this->createChild();
			for (int i = 0; i < 4; i++) {
				this->child[i]->downloadModecreateBeginChunks(p1, p2, bestLevel);
			}
		}
	}
}

void Chunk::downloadModeloadBeginChunks() {
	if (this->childExist)
		for (int i = 0; i < 4; i++) {
			this->child[i]->downloadModeloadBeginChunks();
		}
	this->isNowDownloading.store(true);
	MapLoader::Instance->addLowPriorityTask(this);
}

void Chunk::downloadModeloadClear() {
	if (this->childExist)
		for (int i = 0; i < 4; i++) {
			this->child[i]->downloadModeloadClear();
		}
	while (this->isNowDownloading);	//jezeli jeszcze go pobiera to czeka
	delete this;
	COORD pos = { 11, 1 };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
	std::cout << MapLoader::Instance->readTaskListCount() << "     " << std::endl;
}
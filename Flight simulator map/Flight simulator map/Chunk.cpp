#include "Chunk.h"
#include "Environment.h"
#include "Camera.h"
#include "ChunkShader.h"
#include "GraphicalEngine.h"
#include "MapLoader.h"

#define PI2  6.28318531

ChunkShader *Chunk::Shader = NULL;

bool Chunk::saveDataOnDrive;
short Chunk::levelOfDetailCheckAccuracy;
short Chunk::levelOfDetailCheckPresent;
short Chunk::LevelOfDetailCount;
double Chunk::LevelOfDetail[20];

double Chunk::minHorizont;
double earthRadius = 63784.10;

Chunk::Chunk(Coordinate southWest, Coordinate northEast, Chunk* parent)
{
	if (parent == NULL) {
		this->detailLevel = 0;
	}
	else {
		this->detailLevel = parent->detailLevel +1;
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

void Chunk::loadLevelOfDetail()
{
	char * bufferName = (char*)malloc(1024);

	Chunk::LevelOfDetailCount = Config::Instance->takeConfigInt("LevelOfDetailCount");
	for (int i = 0; i < Chunk::LevelOfDetailCount; i++) {

		sprintf(bufferName, "LevelOfDetail%d", i);
		Chunk::LevelOfDetail[i] = Config::Instance->takeConfigInt(bufferName);
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

	this->child[0] = new Chunk(this->southWest, tmpCord,this);
	this->child[1] = new Chunk(tmpCord4, tmpCord1, this);
	this->child[2] = new Chunk(tmpCord3, tmpCord2, this);
	this->child[3] = new Chunk(tmpCord, this->northEast, this);


	this->childExist = true;

}

void Chunk::removeChild()
{
	this->childExist = false;
}

void Chunk::downloadChunk(HttpRequester* httpRequester)
{
	char * bufferName = (char*)malloc(1024);	
	sprintf(bufferName, "Data/%f.%f.%f.%f", this->southWest.latitude, this->southWest.longtitude, this->northEast.latitude, this->northEast.longtitude);

	char * buffer = (char*)malloc(100);

	//sprintf(buffer, "%s.%d,%d.elevation", bufferName, this->elevationData->rows, this->elevationData->cols);

	sprintf(buffer, "%s.%d.elevation", "Data/", 1);
	this->elevationData = ElevationData::readFromDrive(this->southWest, this->northEast, buffer);

	if (this->elevationData == NULL) {
		this->elevationData = httpRequester->getElevationData(this->southWest, this->northEast, "sealevel");
		if (this->saveDataOnDrive) {
			this->elevationData->saveOnDrive(buffer);
		}


	}
	



	this->vertices = new Vertex*[ElevationData::rows];
	for (int row = 0; row < ElevationData::rows; row++)
		this->vertices[row] = new Vertex[ElevationData::cols];

	
		for (int i = 0; i < ElevationData::rows; i++)
			for (int j = 0; j < ElevationData::cols; j++) {
				this->vertices[i][j].position = glm::vec3(
					(float)(earthRadius /*+ this->elevationData->heights[i][j]*/)*sin(this->elevationData->coordinates[i][j].longtitude)*sin(this->elevationData->coordinates[i][j].latitude),
					(float)(earthRadius /*+ this->elevationData->heights[i][j]*/)*cos(this->elevationData->coordinates[i][j].longtitude)*sin(this->elevationData->coordinates[i][j].latitude),
					(float)(earthRadius/* + this->elevationData->heights[i][j]*/)*cos(this->elevationData->coordinates[i][j].latitude));
		}

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



	float positionY;
	for (int i = 0; i < ElevationData::rows; i++) {
		if (i == ElevationData::rows - 1)
			positionY = 1;
		else
			positionY = (float)(1-(float)(this->satelliteImage->metadata->markers[1][i]- this->satelliteImage->metadata->markers[1][ElevationData::rows-1])/(float)this->satelliteImage->metadata->sizeY());
		for (int j = 0; j < ElevationData::cols; j++) {
			this->vertices[i][j].textureCoord = glm::vec2((float)j / (ElevationData::cols - 1), positionY);
		}
	}
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
	}
	else {
		if (this->isDownloaded.load() && !this->isLoaded) {
			std::cout << tmp++ << std::endl;
			this->satelliteImage->texture = new Texture(this->satelliteImage->source,this->satelliteImage->sourceSize);
			this->isLoaded = true;
		}
	}
}



void Chunk::draw() {
	if (this->detailLevel<0 || (this->childExist && this->isChildsLoaded())) {
		for (int i = 0; i < 4; i++) {
			this->child[i]->draw();
		}
	}
	else {
		if (!this->isLoaded || !this->visible) return;


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
		
		for (int i = 1; i < ElevationData::rows; i++) {
		glBegin(GL_QUAD_STRIP);
			for (int j = 0; j < ElevationData::cols; j++) {
				glTexCoord2f(this->vertices[i][j].textureCoord.x, this->vertices[i][j].textureCoord.y);	glVertex3f(this->vertices[i][j].position.x, this->vertices[i][j].position.y, this->vertices[i][j].position.z);
				glTexCoord2f(this->vertices[i - 1][j].textureCoord.x, this->vertices[i - 1][j].textureCoord.y);	glVertex3f(this->vertices[i - 1][j].position.x, this->vertices[i - 1][j].position.y, this->vertices[i-1][j].position.z);
				//std::cout << this->vertices[i][j].position.x << "  " <<this->vertices[i][j].position.y  << "  " << this->vertices[i][j].position.z << std::endl;
			//	std::cout << this->vertices[i - 1][j].position.x << "  " << this->vertices[i - 1][j].position.y << "  " << this->vertices[i - 1][j].position.z << std::endl;

			}
			glEnd();
		}
	//	std::cout << std::endl;

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

void Chunk::saveOnDrive(std::string path)
{

	this->satelliteImage->saveOnDrive(path+".jpeg");
	this->satelliteImage->metadata->saveOnDrive(path + ".metadata");

	char * buffer = (char*)malloc(30);

	sprintf(buffer, ".%d,%d.elevation", this->elevationData->rows, this->elevationData->cols);
	this->elevationData->saveOnDrive(path + buffer);

	free(buffer);

}

double Chunk::distanceFromCamera()
{
	double distanceCameraFromCenter = glm::distance(Camera::ActiveCamera->position,glm::vec3(0,0,0));
	double latitude = acos(Camera::ActiveCamera->position.z / distanceCameraFromCenter);
	double longtitude = atan2(Camera::ActiveCamera->position.x, Camera::ActiveCamera->position.y);
	if (longtitude>0) longtitude -= PI2/2;
	else  longtitude += PI2/2;
	Coordinate cameraCoordinates = Coordinate((latitude * 360 / PI2) - 90, (longtitude * 360 / PI2));
	double minDistance;
	double tmpDistance;

	if (cameraCoordinates.latitude>this->southWest.latitude && cameraCoordinates.longtitude>this->southWest.longtitude&&this->northEast.latitude>cameraCoordinates.latitude &&this->northEast.longtitude > cameraCoordinates.longtitude) {
		minDistance = glm::distance(this->vertices[0][0].position, Camera::ActiveCamera->position);
		for (int i = 1; i < ElevationData::rows; i++) {
			for (int j = 0; j < ElevationData::cols; j++) {
				tmpDistance = glm::distance(this->vertices[i][j].position, Camera::ActiveCamera->position);
				if (tmpDistance < minDistance)
					minDistance = tmpDistance;
			}
		}
	}
	else {
		minDistance = glm::distance(this->vertices[0][0].position, Camera::ActiveCamera->position);
		for (int i = 1; i < ElevationData::rows; i++) {
			tmpDistance = glm::distance(this->vertices[i][ElevationData::cols-1].position, Camera::ActiveCamera->position);
			if (tmpDistance < minDistance)
				minDistance = tmpDistance;
			tmpDistance = glm::distance(this->vertices[i][0].position, Camera::ActiveCamera->position);
			if (tmpDistance < minDistance)
				minDistance = tmpDistance;
		}
		for (int j = 0; j < ElevationData::cols; j++) {
			tmpDistance = glm::distance(this->vertices[0][j].position, Camera::ActiveCamera->position);
			if (tmpDistance < minDistance)
				minDistance = tmpDistance;
			tmpDistance = glm::distance(this->vertices[ElevationData::rows-1][j].position, Camera::ActiveCamera->position);
			if (tmpDistance < minDistance)
				minDistance = tmpDistance;
			}


	}
		
	return minDistance;
}

short Chunk::calculatePrefDetailLevel(){
	double distance= this->distanceFromCamera();
	double cameraDistance = glm::distance(Camera::ActiveCamera->position, glm::vec3(0, 0, 0));
	double horizont;
	if (cameraDistance <= earthRadius)
		horizont = this->minHorizont;
	else
		horizont = sqrt(cameraDistance*cameraDistance- earthRadius*earthRadius);
	if (horizont < this->minHorizont)
		horizont == this->minHorizont;
	if (distance > horizont)return -1;
	for (int i = Chunk::LevelOfDetailCount-1; i >=0 ; i--) {
		if (distance < this->LevelOfDetail[i])
			return i;
	}
	return -1;
}
void Chunk::calculateAllDetails()
{

	if (this->childExist && this->isChildsLoaded()) {
		if (this->child[0]->toRemove && this->child[1]->toRemove && this->child[2]->toRemove && this->child[3]->toRemove)
			this->removeChild();
		else
			for (int i = 0; i < 4; i++) {
				this->child[i]->calculateAllDetails();
			}
	}
	else{
		if (this->levelOfDetailCheck != this->levelOfDetailCheckPresent) return;
		if (!this->isDownloaded) return;
		if (!this->childExist) {
			short prefDetailLevel = this->calculatePrefDetailLevel();
			if (this->detailLevel < prefDetailLevel) {	//trzeba powiekszyc jakosc
				this->createChild();
			}
			if (this->detailLevel > prefDetailLevel + 1)	//chunk do usuniecia
				this->toRemove = true;;
			if (prefDetailLevel == -1)
				this->visible = false;
			else
				this->visible = true;
		}
	}
}

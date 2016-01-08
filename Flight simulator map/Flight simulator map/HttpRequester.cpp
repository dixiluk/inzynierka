#include "HttpRequester.h"
#include <string>
#include <iostream>
#include "Texture.h"
#include "Chunk.h"
using namespace std;

const char ElevationMessage[] = "GET /REST/v1/Elevation/Bounds?bounds=%f,%f,%f,%f&rows=%d&cols=%d&heights=%s&key=%s HTTP/1.1\r\nHost: dev.virtualearth.net\r\nConnection: keep-alive\r\n\r\n";
const char SateliteMessage[] = "GET /REST/v1/Imagery/Map/Aerial/?mapArea=%f,%f,%f,%f&mapSize=%d,%d&format=%s&mapMetadata=%d&key=%s HTTP/1.1\r\nHost: %s\r\nConnection: keep-alive\r\n\r\n";
const char SateliteMetadataMessage[] = "GET /REST/v1/Imagery/Map/Aerial/?mapArea=%f,%f,%f,%f&mapSize=%d,%d&format=%s&mapMetadata=%d&%skey=%s HTTP/1.1\r\nHost: %s\r\nConnection: keep-alive\r\n\r\n";
int HttpRequester::prefImageSizeX;
int HttpRequester::prefImageSizeY;
const char imageFormat[] = "jpeg";

HttpRequester::HttpRequester(std::string server, std::string key)
{
	this->key = key;
	this->server = server;
	while (true) {
		this->connectionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		struct hostent *host;
		host = gethostbyname(this->server.c_str());
		SOCKADDR_IN SockAddr;
		SockAddr.sin_port = htons(80);
		SockAddr.sin_family = AF_INET;
		SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
		if (connect(this->connectionSocket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
			std::cout << "Could not connect! Recconecting..." << GetLastError() << std::endl;
			continue;
		}
		break;
	}
}


HttpRequester::~HttpRequester()
{
}

bool HttpRequester::reconnect(bool force)
{
	while (true) {
		this->connectionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		struct hostent *host;
		host = gethostbyname(this->server.c_str());
		SOCKADDR_IN SockAddr;
		SockAddr.sin_port = htons(80);
		SockAddr.sin_family = AF_INET;
		SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
		if (connect(this->connectionSocket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
			std::cout << "Could not connect! Recconecting..." << GetLastError() << std::endl;
			if (force)
				continue;
			else
				return false;
		}
		break;
	}
	return true;
}

ElevationData* HttpRequester::getElevationData(Coordinate southWest, Coordinate northEast, std::string height)
{
	//http://dev.virtualearth.net/REST/v1/Elevation/Bounds?bounds=45,80,46,81&rows=2&cols=2&heights=sealevel&key=AvLyPxYc5C5cPPAwZdsrhI1c4sT9FJo1AUVym7tgs-IvZzo720jrDdn-ZG-0Jrb9
	while (true) {
		this->source.clear();
		this->header.clear();

		requestBuffer = (char*)malloc(4086);
		sprintf(requestBuffer, ElevationMessage, southWest.latitude, southWest.longitude, northEast.latitude, northEast.longitude, ElevationData::rows, ElevationData::cols, height.c_str(), key.c_str());
		if (send(this->connectionSocket, requestBuffer, strlen(requestBuffer), 0) <= 0)
		{
			this->reconnect();
			free(requestBuffer);
			continue;
		}
		else
			free(requestBuffer);

		requestBuffer = 0;

		memset(last, ' ', 5);

		do {
			last[0] = last[1];
			last[1] = last[2];
			last[2] = last[3];
			if (recv(this->connectionSocket, &last[3], sizeof(char), 0) <= 0)
			{
				this->reconnect();
				continue;
			}

			header += last[3];
		} while (!(last[0] == '\r'&&last[2] == '\r'));
		int beg = header.find("-Length: ") + strlen("-Length: ");
		int end = header.find("\r\n\r\n", beg, 4);
		std::string length = header.substr(beg, end - beg);
		int contentLength = std::stoi(length);
		char* buffer = (char*)malloc(contentLength);
		int sum = 0;
		int receivedLength;
		do {
			receivedLength = recv(this->connectionSocket, buffer, contentLength - sum, 0);
			sum += receivedLength;
			if (receivedLength <= 0)
			{
				this->reconnect();
				free(buffer);
				continue;
			}
			source.append(buffer, receivedLength);
		} while (sum != contentLength);
		return new ElevationData(southWest, northEast, source, sum);
	}
}

bool HttpRequester::receiveHeader() {
	memset(last, ' ', 5);

	do {
		last[0] = last[1]; last[1] = last[2]; last[2] = last[3]; last[3] = last[4];
		if (recv(this->connectionSocket, &last[4], sizeof(char), 0) <= 0)
		{
			return false;
		}

		header += last[4];
	} while (!(last[1] == '\r'&&last[2] == '\n'&&last[3] == '\r'&&last[4] == '\n'));
	do {
		last[3] = last[4];
		if (recv(this->connectionSocket, &last[4], sizeof(char), 0) <= 0)
		{
			return false;
		}

		header += last[4];
	} while (!(last[3] == '\r'&&last[4] == '\n'));
	return true;
}

bool HttpRequester::receiveChunkSource()
{
	int chunkSize = std::strtoul(header.substr(header.find("\r\n\r\n")).c_str(), nullptr, 16);
	int receivedLength;
	char* buffer = (char*)malloc(1024);
	this->sourceSize = 0;
	while (true) {
		if ((receivedLength = recv(this->connectionSocket, buffer, 1024, 0)) <= 0)
			return false;
		this->sourceSize += receivedLength;
		source.append(buffer, receivedLength);
		if (chunkSize <= this->sourceSize - 7)
			break;
	}
	return true;
}

SatelliteImage* HttpRequester::getSatelliteImageSource(Chunk* chunk, int sizeX, int sizeY)
{
	//http://dev.virtualearth.net/REST/v1/Imagery/Map/Aerial/47.619048,-122.35384/5?&mapSize=600,700&format=jpeg&mapMetadata=0&key=AvLyPxYc5C5cPPAwZdsrhI1c4sT9FJo1AUVym7tgs-IvZzo720jrDdn-ZG-0Jrb9

	while (true) {
		string markers;
		char * buffer = (char*)malloc(1024);

		double stepLatitude = (chunk->northEast.latitude - chunk->southWest.latitude) / (ElevationData::rows - 1);
		Coordinate cord = chunk->southWest;

		for (int row = 0; row < ElevationData::rows; row++) {
			sprintf(buffer, "pp=%f,%f;21&", cord.latitude, chunk->southWest.longitude);
			markers += std::string(buffer);
			cord.latitude += stepLatitude;
		}
		sprintf(buffer, "pp=%f,%f;21&", chunk->northEast.latitude, chunk->northEast.longitude);
		markers += std::string(buffer);
		free(buffer);

		SatelliteImageMetadata* metaData = this->getSatelliteImageMetadata(chunk->southWest, chunk->northEast, sizeX, sizeY, imageFormat, markers, ElevationData::rows + 1);
		sizeX = metaData->sizeX();
		sizeY = metaData->sizeY();
		requestBuffer = (char*)malloc(4086);

		source.clear();
		header.clear();

		
		sprintf(requestBuffer, SateliteMessage, chunk->southWest.latitude, chunk->southWest.longitude, chunk->northEast.latitude, chunk->northEast.longitude, sizeX, sizeY, imageFormat, 0, this->key.c_str(), this->server.c_str());

		if (send(this->connectionSocket, requestBuffer, strlen(requestBuffer), 0) <= 0)
		{
			this->reconnect();
			free(requestBuffer);
			continue;
		}

		free(requestBuffer);

		if (!receiveHeader())
			continue;
		if (!receiveChunkSource())
			continue;
		char* copy =(char*)malloc(this->sourceSize);
		source.copy(copy, this->sourceSize, 0);

		return new SatelliteImage(copy, sourceSize, metaData);
	}
}

SatelliteImageMetadata* HttpRequester::getSatelliteImageMetadata(Coordinate southWest, Coordinate northEast, int sizeX, int sizeY, string format, string markers, short markerCount)
{
	//http://dev.virtualearth.net/REST/v1/Imagery/Map/Aerial/47.619048,-122.35384/5?&mapSize=600,700&//format=jpeg&mapMetadata=0&key=AvLyPxYc5C5cPPAwZdsrhI1c4sT9FJo1AUVym7tgs-IvZzo720jrDdn-ZG-0Jrb9
	while (true) {
		requestBuffer = (char*)malloc(4086);
		this->header.clear();
		this->source.clear();
		// lat, long, lat, long, sizex, sizey, format, metadata,markers, key
		sprintf(requestBuffer, SateliteMetadataMessage, southWest.latitude, southWest.longitude, northEast.latitude, northEast.longitude, sizeX, sizeY, format.c_str(), 1, markers.c_str(), this->key.c_str(), this->server.c_str());

		if (send(this->connectionSocket, requestBuffer, strlen(requestBuffer), 0) <= 0)
		{
			this->reconnect();
			free(requestBuffer);
			continue;
		}
		free(requestBuffer);

		if (!receiveHeader())
			continue;
		if (!receiveChunkSource())
			continue;

		/*char* copy = (char*)malloc(this->sourceSize);
		source.copy(copy, this->sourceSize, 0);
		copy[this->sourceSize] = 0;*/
		std::string copy = std::string(source);
		return new SatelliteImageMetadata(copy, copy.size());
	}
}

#include "HttpRequester.h"
#include <string>
#include <iostream>

using namespace std;

const char ElevationMessage[] = "GET /REST/v1/Elevation/Bounds?bounds=%f,%f,%f,%f&rows=%d&cols=%d&heights=%s&key=%s HTTP/1.1\r\nHost: dev.virtualearth.net\r\nConnection: keep-alive\r\n\r\n";
const char SateliteMessage[] = "GET /REST/v1/Imagery/Map/Aerial/%f,%f/%d?&mapSize=%d,%d&format=%s&mapMetadata=%d&key=%s HTTP/1.1\r\nHost: %s\r\nConnection: keep-alive\r\n\r\n";

HttpRequester::HttpRequester(std::string server, std::string key)
{
	this->key = key;
	this->server = server;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "WSAStartup failed.\n";
		system("pause");
		exit(-1);
	}
	this->connectionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct hostent *host;
	host = gethostbyname(this->server.c_str());
	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = htons(80);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
	if (connect(this->connectionSocket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0){
		std::cout << "Could not connect";
		system("pause");
		exit(-1);
	}
}


HttpRequester::~HttpRequester()
{
}

ElevationData* HttpRequester::getElevationData(Coordinate northEast, Coordinate southWest, short rows, short cols, std::string height)
{
	//http://dev.virtualearth.net/REST/v1/Elevation/Bounds?bounds=45,80,46,81&rows=2&cols=2&heights=sealevel&key=AvLyPxYc5C5cPPAwZdsrhI1c4sT9FJo1AUVym7tgs-IvZzo720jrDdn-ZG-0Jrb9
	
	this->source.clear();
	this->header.clear();

	requestBuffer = (char*)malloc(4086);
	sprintf(requestBuffer, ElevationMessage, northEast.latitude, northEast.longtitude, southWest.latitude, southWest.longtitude, rows, cols, height.c_str(), key.c_str());
	if(send(this->connectionSocket, requestBuffer, strlen(requestBuffer), 0)<=0)
	{
		std::cout << "1 Connection error!" << std::endl;
		//system("pause");
	}
	free(requestBuffer);
	requestBuffer = 0;

	memset(last, ' ', 5);
	
	do{
		last[0] = last[1];
		last[1] = last[2];
		last[2] = last[3];
		if (recv(this->connectionSocket, &last[3], sizeof(char), 0) <=0)
		{
			std::cout << "2 Connection error!" << std::endl;
			//system("pause");
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
	do{
		receivedLength = recv(this->connectionSocket, buffer, contentLength-sum, 0);
		sum += receivedLength;
		if (receivedLength <= 0)
		{
			std::cout << "3 Connection error!" << std::endl;
		//	system("pause");
		}
		source.append(buffer, receivedLength);
	} while (sum != contentLength);

	

	//std::cout << "------------------------DATA------------------------\r\n-" << source << "-\r\n---------------------------------------------------- \r\n";

	return new ElevationData(rows,cols,southWest,northEast, source);
}

void HttpRequester::getSatelliteImage(Coordinate center, int zoom, int sizeX, int sizeY, string format, bool metadata)
{
	//http://dev.virtualearth.net/REST/v1/Imagery/Map/Aerial/47.619048,-122.35384/5?&mapSize=600,700&format=jpeg&mapMetadata=0&key=AvLyPxYc5C5cPPAwZdsrhI1c4sT9FJo1AUVym7tgs-IvZzo720jrDdn-ZG-0Jrb9

	requestBuffer = (char*)malloc(4086);

	// lat, long, zoom, sizex, sizey, format, metadata, key
	sprintf(requestBuffer, SateliteMessage, center.latitude, center.longtitude, zoom, sizeX, sizeY, format.c_str(), (int)metadata, this->key.c_str(), this->server.c_str());

	if (send(this->connectionSocket, requestBuffer, strlen(requestBuffer), 0) <= 0)
	{
		std::cout << "Connection error!" << std::endl;
		system("pause");
	}
	free(requestBuffer);
	int receivedLength;

	memset(last, ' ', 5);

	do{
		last[0] = last[1]; last[1] = last[2]; last[2] = last[3]; last[3] = last[4];
		if (recv(this->connectionSocket, &last[4], sizeof(char), 0) <= 0)
		{
			std::cout << "Connection error!" << std::endl;
			system("pause");
		}

		header += last[4];
	} while (!(last[1] == '\r'&&last[2] == '\n'&&last[3] == '\r'&&last[4] == '\n'));
	do{
		last[3] = last[4];
		if (recv(this->connectionSocket, &last[4], sizeof(char), 0) <= 0)
		{
			std::cout << "Connection error!" << std::endl;
			system("pause");
		}

		header += last[4];
	} while (!(last[3] == '\r'&&last[4] == '\n'));

	int chunkSize = std::strtoul(header.substr(header.find("\r\n\r\n")).c_str(), nullptr, 16);

	

	char* buffer = (char*)malloc(1024);
	int sum = 0;
	while ((receivedLength = recv(this->connectionSocket, buffer, 1024, 0))!=0){
		sum += receivedLength;
		source.append(buffer, receivedLength);
		if (chunkSize <= sum-7) 
			break;
	}
	
	FILE* file;
	while((file = fopen("Image.jpeg", "wb"))==NULL);
	fwrite(source.c_str(), 1, sum-7, file);
	fclose(file);
	free(buffer);
	//std::cout << "------------------------DATA------------------------\r\n-" << source << "-\r\n---------------------------------------------------- \r\n";

}
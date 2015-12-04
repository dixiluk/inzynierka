#include "HttpRequester.h"
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")


HttpRequester::HttpRequester()
{
}


HttpRequester::~HttpRequester()
{
}


ElevationData HttpRequester::getElevationData(Coordinate northEast, Coordinate southWest, short rows, short cols, std::string height, std::string key)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "WSAStartup failed.\n";
		system("pause");
		return;
	}
	SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct hostent *host;
	host = gethostbyname("dev.virtualearth.net");
	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = htons(80);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
	std::cout << "Connecting...\n";
	if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
		std::cout << "Could not connect";
		system("pause");
		return;
	}
	std::cout << "Connected.\n";
	//http://dev.virtualearth.net/REST/v1/Elevation/Bounds?bounds=45,80,46,81&rows=2&cols=2&heights=sealevel&key=AvLyPxYc5C5cPPAwZdsrhI1c4sT9FJo1AUVym7tgs-IvZzo720jrDdn-ZG-0Jrb9
	char* msg = "GET /REST/v1/Elevation/Bounds?bounds=%f,%f,%f,%f&rows=%d&cols=%d&heights=%s&key=%s HTTP/1.1\r\nHost: dev.virtualearth.net\r\nConnection: close\r\n\r\n";
	char* requestBuffer = (char*)malloc(4086);
	sprintf(requestBuffer, msg, northEast.latitude, northEast.longitude, westSouth.latitude, westSouth.longitude, rows, cols, height, key);
	printf(requestBuffer);
	send(Socket, msg, strlen(msg), 0);
	char buffer[40000];
	int nDataLength;
	nDataLength = recv(Socket, buffer, 40000, 0);
	for (int i = 0; i < nDataLength; i++) {
		std::cout << buffer[i];

	}

	closesocket(Socket);
	WSACleanup();
	system("pause");
	return;

}
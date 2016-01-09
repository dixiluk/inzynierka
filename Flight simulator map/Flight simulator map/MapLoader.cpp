#include "MapLoader.h"
#include "HttpRequester.h"
#include "Config.h"


MapLoader* MapLoader::Instance = NULL;

void MapLoader::addHighPriorityTask(Chunk * chunk)
{
	bool exp = false;
	while (!taskListLocked.compare_exchange_weak(exp, true)) {
		exp = false;
		Sleep(50);
	}
	std::atomic_thread_fence(std::memory_order_acquire);
	this->taskList.push_front(chunk);
	std::atomic_thread_fence(std::memory_order_release);
	taskListLocked.store(false);
}

void MapLoader::addLowPriorityTask(Chunk * chunk)
{
	bool exp = false;
	while (!taskListLocked.compare_exchange_weak(exp, true)) {
		exp = false;
		Sleep(50);
	}
	std::atomic_thread_fence(std::memory_order_acquire);
	this->taskList.push_back(chunk);
	std::atomic_thread_fence(std::memory_order_release);
	taskListLocked.store(false);
}

int MapLoader::readTaskListCount() 
{
	bool exp = false;
	while (!taskListLocked.compare_exchange_weak(exp, true)) {
		exp = false;
		Sleep(50);
	}
	std::atomic_thread_fence(std::memory_order_acquire);
	int count = this->taskList.size();
	std::atomic_thread_fence(std::memory_order_release);
	for (int i = 0; i < this->threadCount; i++)
		if (!this->threadSleepArray[i])
			count++;
	taskListLocked.store(false);
	return count;
}

void MapLoader::createThread()
{
	this->threadArray = new std::thread[this->threadCount];
	this->threadSleepArray = new std::atomic<bool>[this->threadCount];
	for (int i = 0; i < this->threadCount; i++)
		this->threadArray[i] = std::thread(MapLoader::ThreadFunc,i);
}



void MapLoader::ThreadFunc(int threadNr)
{
	bool exp;
	HttpRequester *httpRequester = NULL;

	while (true) {
		exp = false;
		while (!Instance->taskListLocked.compare_exchange_weak(exp, true)) {
			exp = false;
			Sleep(100);
		}
		std::atomic_thread_fence(std::memory_order_acquire);
		Chunk* chunk = NULL;
		if (Instance->taskList.size()>0) {
			Instance->threadSleepArray[threadNr].store(false);
			chunk = (*Instance->taskList.begin());
			Instance->taskList.remove(chunk);
			std::atomic_thread_fence(std::memory_order_release);			
		}
		Instance->taskListLocked.store(false);

		if (chunk != NULL)
		{
			chunk->downloadChunk(httpRequester);
		}
		else {
			Instance->threadSleepArray[threadNr].store(true);
			Sleep(100);
		}
		std::atomic_thread_fence(std::memory_order_release);
	}
}

void MapLoader::createMap()
{

	GraphicalEngine::Instance->worldChunk1 = new Chunk(Coordinate(-80, 0), Coordinate(80, 180), NULL);
	GraphicalEngine::Instance->worldChunk1->createBeginChunks();
	GraphicalEngine::Instance->worldChunk1->loadBeginChunks();
	GraphicalEngine::Instance->worldChunk2 = new Chunk(Coordinate(-80, -180), Coordinate(80, 0), NULL);
	GraphicalEngine::Instance->worldChunk2->createBeginChunks();
	GraphicalEngine::Instance->worldChunk2->loadBeginChunks();
	GraphicalEngine::Instance->northChunk = new Chunk(Coordinate(90, -180), Coordinate(80, 180), NULL);
	MapLoader::Instance->addLowPriorityTask(GraphicalEngine::Instance->northChunk);
	GraphicalEngine::Instance->southChunk = new Chunk(Coordinate(-90, -180), Coordinate(-80, 180), NULL);
	MapLoader::Instance->addLowPriorityTask(GraphicalEngine::Instance->southChunk);
}

void MapLoader::downloadMapMode(Coordinate p1, Coordinate p2, short bestLevel) {
	system("cls");
	std::cout << "Pobieranie mapy.." << std::endl << "pozostalo: ";
	


	GraphicalEngine::Instance->worldChunk1 = new Chunk(Coordinate(-80, 0), Coordinate(80, 180), NULL);
	GraphicalEngine::Instance->worldChunk1->downloadModecreateBeginChunks(p1,p2,bestLevel);
	GraphicalEngine::Instance->worldChunk1->downloadModeloadBeginChunks();
	GraphicalEngine::Instance->worldChunk2 = new Chunk(Coordinate(-80, -180), Coordinate(80, 0), NULL);
	GraphicalEngine::Instance->worldChunk2->downloadModecreateBeginChunks(p1, p2, bestLevel);
	GraphicalEngine::Instance->worldChunk2->downloadModeloadBeginChunks();
	std::cout << MapLoader::Instance->readTaskListCount() << "     " << std::endl;
	GraphicalEngine::Instance->worldChunk1->downloadModeloadClear();
	GraphicalEngine::Instance->worldChunk2->downloadModeloadClear();

	std::cout << "Pobrano mape.." << std::endl;
}
MapLoader::MapLoader(int count)
{
	this->taskListLocked.store(false);
	this->threadCount = count;
}


MapLoader::~MapLoader()
{
}

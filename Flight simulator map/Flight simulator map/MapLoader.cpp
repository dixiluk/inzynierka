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
	std::cout << this->taskList.size() << std::endl;
	std::atomic_thread_fence(std::memory_order_release);
	taskListLocked.store(false);
}

void MapLoader::createThread(int count)
{
	threadArray = new std::thread[count];
	for (int i = 0; i < count; i++)
		threadArray[i] = std::thread(MapLoader::ThreadFunc);
}



void MapLoader::ThreadFunc()
{
	bool exp;
	HttpRequester *httpRequester = new HttpRequester(Config::Instance->takeConfigString("dataServer"), Config::Instance->takeConfigString("licenceKey"));

	while (true) {
		exp = false;
		while (!Instance->taskListLocked.compare_exchange_weak(exp, true)) {
			exp = false;
			Sleep(100);
		}
		std::atomic_thread_fence(std::memory_order_acquire);
		Chunk* chunk = NULL;
		if (Instance->taskList.size()>0) {
			chunk = (*Instance->taskList.begin());
			Instance->taskList.remove(chunk);
			//Instance->taskList.pop_front();
			//std::cout << 
			std::atomic_thread_fence(std::memory_order_release);			
		}
		Instance->taskListLocked.store(false);

		//std::atomic_thread_fence(std::memory_order_acquire);
		if (chunk != NULL)
		{
			chunk->downloadChunk(httpRequester);
		}
		else {
			Sleep(100);
		}
		std::atomic_thread_fence(std::memory_order_release);
		//Sleep(50);
	}
}

MapLoader::MapLoader()
{
	this->taskListLocked.store(false);
}


MapLoader::~MapLoader()
{
}

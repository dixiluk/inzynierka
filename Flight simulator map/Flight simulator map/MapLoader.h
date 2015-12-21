#pragma once
#include "Chunk.h"
#include <atomic>
#include <thread>

class MapLoader
{
public:
	static MapLoader * Instance;
	std::thread* threadArray;
	std::list<Chunk*> taskList;
	std::atomic<bool> taskListLocked;
	void addHighPriorityTask(Chunk* chunk);
	void addLowPriorityTask(Chunk* chunk);
	void createThread(int count);
	static void ThreadFunc();
	MapLoader();
	~MapLoader();
};


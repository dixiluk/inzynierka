#pragma once
#include "Chunk.h"
#include <atomic>
#include <thread>

class MapLoader
{
public:
	int threadCount;
	static MapLoader * Instance;
	std::thread* threadArray;
	std::atomic<bool>* threadSleepArray;
	std::list<Chunk*> taskList;
	std::atomic<bool> taskListLocked;
	void addHighPriorityTask(Chunk* chunk);
	void addLowPriorityTask(Chunk* chunk);
	void createThread();
	static void ThreadFunc(int threadNr);
	static void createMap();
	static void downloadMapMode(Coordinate p1, Coordinate p2, short bestLevel);
	int readTaskListCount();
	MapLoader(int count);
	~MapLoader();
};


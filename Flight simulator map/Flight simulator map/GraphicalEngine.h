#pragma once
#include "Environment.h"
#include "Config.h"
#include "Chunk.h"

class GraphicalEngine
{
public:
	struct Resolution{
	public:
		Resolution(){}
		Resolution(int Width, int Height){
			this->Width = Width;
			this->Height = Height;
		}
		int Width, Height;

	} resolution;

	Chunk *worldChunk1;
	Chunk *worldChunk2;
	Chunk *northChunk;
	Chunk *southChunk;
	Config *activeConfig;
	PFNGLWINDOWPOS2IPROC glWindowPos2i;

	static GraphicalEngine* Instance;

	bool keyboard[255];

	GraphicalEngine(Resolution resolution);
	~GraphicalEngine();
	static void Init(int argc, char * argv[]);
	static void DisplayFunc();
	static void ReshapeFunc(int width, int height);
	static void KeyboardFunc(unsigned char key, int x, int y);
	static void KeyboardUpFunc(unsigned char key, int x, int y);
	static void SpecialFunc(int key, int x, int y);
	static void SpecialUpFunc(int key, int x, int y);
	static void PassiveMotionFunc(int x, int y);
	static void UpdatePass();
	static void RenderPass();
	
	
};
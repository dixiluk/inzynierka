#include "GraphicalEngine.h"
#include "Camera.h"
#include "DynamicObject.h"
#include "Chunk.h"
#include "HttpRequester.h"
#include <thread>

GraphicalEngine* GraphicalEngine::Instance = nullptr;


GraphicalEngine::GraphicalEngine(Resolution resolution)
{
}


GraphicalEngine::~GraphicalEngine()
{
}

void GraphicalEngine::Init(int argc, char * argv[])						//inicjacja parametrow OpenGl i tworzenie okna
{

	for (int i = 0; i < 255; i++) {
		Instance->keyboard[i] = false;
	}


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(Instance->resolution.Width, Instance->resolution.Height);

	glutCreateWindow(WINDOW_TITLE);
	//glutFullScreen();
	glutKeyboardFunc(GraphicalEngine::KeyboardFunc);
	glutKeyboardUpFunc(GraphicalEngine::KeyboardUpFunc);
	glutSpecialFunc(GraphicalEngine::SpecialFunc);
	glutSpecialUpFunc(GraphicalEngine::SpecialUpFunc);
	glutDisplayFunc(GraphicalEngine::DisplayFunc);
	glutPassiveMotionFunc(GraphicalEngine::PassiveMotionFunc);
	glutReshapeFunc(GraphicalEngine::ReshapeFunc);
	glutIdleFunc(GraphicalEngine::DisplayFunc);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	int major = 0, minor = 0;
	const char * version = (char *)glGetString(GL_VERSION);
	if (sscanf(version, "%d.%d", &major, &minor) != 2)
	{
		printf("Bledny format wersji OpenGL\n");

	}
}

void GraphicalEngine::DisplayFunc()		//glowna petla
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GraphicalEngine::UpdatePass();

	GraphicalEngine::RenderPass();

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

void GraphicalEngine::ReshapeFunc(int width, int height)	//funkcja zmiany rozmiaru okna
{
	Instance->resolution.Height = height;
	Instance->resolution.Width = width;
	glViewport(0, 0, width, height);
	Camera::ActiveCamera->setAspectRatio((GLdouble)width / height);
}

void GraphicalEngine::KeyboardFunc(unsigned char key, int x, int y)
{
	GraphicalEngine::Instance->keyboard[key] = true;

}

void GraphicalEngine::KeyboardUpFunc(unsigned char key, int x, int y)
{
	GraphicalEngine::Instance->keyboard[key] = false;
	GraphicalEngine::Instance->worldChunk->createChild();
}

void GraphicalEngine::SpecialFunc(int key, int x, int y)
{
	GraphicalEngine::Instance->keyboard[key] = true;

}

void GraphicalEngine::SpecialUpFunc(int key, int x, int y)
{
	GraphicalEngine::Instance->keyboard[key] = false;

}


void GraphicalEngine::PassiveMotionFunc(int x, int y) {
	Camera::CameraMotion(x, y);
}

void GraphicalEngine::UpdatePass()	//wykonywanie wszystkich obliczen
{
	Camera::ActiveCamera->setupCamera();

	for (GraphicalObject* obj : GraphicalEngine::Instance->activeScene->graphicalObjects) {
		obj->compute();
	}
}


void GraphicalEngine::RenderPass() {	//funkcja wykonania rysowania wszystich elementow
	glClearColor(0.2, 0.2, 0.2, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Instance->worldChunk->draw();


}
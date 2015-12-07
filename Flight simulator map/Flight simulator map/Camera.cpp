#include "Camera.h"
#include <cmath>
#include <stdio.h>
#include "GraphicalObject.h"

Camera* Camera::ActiveCamera = 0;

#define PI2  6.28318531
#define PIS2 1.57079600

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 axis)
{
	this->position = position;
	this->direction = glm::normalize(direction);
	this->axis = axis;
	this->pitch = 0;
	this->yaw = 0;

	this->setPerspective(45.0,
		(GLfloat) Engine::Instance->resolution.Width
		/ Engine::Instance->resolution.Height, 1, 5000);
	this->setupCamera();
}

Camera::~Camera()
{
}
/*
void Camera::CameraMotion(GLint x, GLint y)	//funkcja obracanie kamery myszka
{


	if (!Engine::Instance->keyboard[Engine::Instance->moveCameraKey]) return;



	if ((x == Engine::Instance->resolution.Width / 2 + 1 || x == Engine::Instance->resolution.Width / 2 - 1 || x == Engine::Instance->resolution.Width / 2)
		&& (y == Engine::Instance->resolution.Height / 2 + 1 || y == Engine::Instance->resolution.Height / 2 - 1 || y == Engine::Instance->resolution.Height / 2)) return;
	ActiveCamera->pitch += ((GLdouble) x - Engine::Instance->resolution.Width / 2) / 1000;
	ActiveCamera->yaw += ((GLdouble) y - Engine::Instance->resolution.Height / 2) / 1000;

	if (ActiveCamera->pitch > PI2) ActiveCamera->pitch = ActiveCamera->pitch - PI2;
	if (ActiveCamera->yaw >= PIS2) ActiveCamera->yaw = PIS2;
	if (ActiveCamera->yaw <= -PIS2) ActiveCamera->yaw = -PIS2;

	ActiveCamera->direction.x = -sin(ActiveCamera->pitch)*cos(ActiveCamera->yaw);
	ActiveCamera->direction.y = -sin(ActiveCamera->yaw);
	ActiveCamera->direction.z = cos(ActiveCamera->pitch)*cos(ActiveCamera->yaw);

	glutWarpPointer(Engine::Instance->resolution.Width / 2, Engine::Instance->resolution.Height / 2);
	ActiveCamera->setupCamera();

}

*/
void Camera::CameraMotion(GLint x, GLint y)	//funkcja obracanie kamery myszka
{


	if (!Engine::Instance->keyboard[Engine::Instance->moveCameraKey]) return;



	if ((x == Engine::Instance->resolution.Width / 2 +1 || x == Engine::Instance->resolution.Width / 2 -1 || x == Engine::Instance->resolution.Width / 2) 
		&& (y == Engine::Instance->resolution.Height / 2 +1 || y == Engine::Instance->resolution.Height / 2 -1 || y == Engine::Instance->resolution.Height / 2)) return;
	ActiveCamera->pitch += ((GLdouble) x - Engine::Instance->resolution.Width / 2) / 1000;
	ActiveCamera->yaw += ((GLdouble) y - Engine::Instance->resolution.Height / 2) / 1000;

	if (ActiveCamera->pitch > PI2) ActiveCamera->pitch = ActiveCamera->pitch - PI2;
	if (ActiveCamera->yaw >= PIS2) ActiveCamera->yaw = PIS2;
	if (ActiveCamera->yaw <= -PIS2) ActiveCamera->yaw = -PIS2;

	ActiveCamera->direction.x = -sin(ActiveCamera->pitch)*cos(ActiveCamera->yaw);
	ActiveCamera->direction.y = -sin(ActiveCamera->yaw);
	ActiveCamera->direction.z = cos(ActiveCamera->pitch)*cos(ActiveCamera->yaw);

	glutWarpPointer(Engine::Instance->resolution.Width / 2, Engine::Instance->resolution.Height / 2);
	ActiveCamera->setupCamera();

}

void Camera::setActive()
{
	ActiveCamera = this;
}

void Camera::setPosition(glm::vec3 position) //ustawia pozycje kamery
{
	this->position = position;
	this->setupCamera();
}


void Camera::setDirection(glm::vec3 direction){ //ustawia kierunek kamery
	this->direction = glm::normalize(direction);
	this->setupCamera();
}

void Camera::setDirectionToPosition(glm::vec3 position){ //ustawia kierunek kamery
	glm::vec3 tmp = position - this->position;
	this->direction = glm::normalize(tmp);
	this->setupCamera();
}



void Camera::setAspectRatio(GLdouble aspectRatio){ // ustawia proporcje ekranu
	this->aspectRatio = aspectRatio;
	this->projectionMatrix = glm::perspective(fovY, aspectRatio, zNear, zFar);
	this->viewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}

void Camera::setFovY(GLdouble fovY){ //katwidzenia
	this->fovY = fovY;
	this->projectionMatrix = glm::perspective(fovY, aspectRatio, zNear, zFar);
	this->viewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}

void Camera::setZFar(GLdouble zFar){ //wartosc maksymalna na ktora patrzymy
	this->zFar = zFar;
	this->projectionMatrix = glm::perspective(fovY, aspectRatio, zNear, zFar);
	this->viewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}

void Camera::setZNear(GLdouble zNear){ //wartosc minimalna z ktorej widac
	this->zNear = zNear;
	this->projectionMatrix = glm::perspective(fovY, aspectRatio, zNear, zFar);
	this->viewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}

void Camera::setPerspective(GLdouble fovY, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar)  //ustawi prespektywe
{
	this->fovY = fovY;
	this->aspectRatio = aspectRatio;
	this->zNear = zNear;
	this->zFar = zFar;
	this->projectionMatrix = glm::perspective(glm::radians(fovY), aspectRatio, zNear, zFar); 
	this->viewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}

void Camera::setupCamera()  //stworzenie macierzy widoku
{
	this->viewMatrix = glm::lookAt(this->position + (this->direction*10.0f),
		this->position,
		this->axis);
	this->viewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}

glm::vec4 Camera::calculateModelViewProjMatrix(glm::vec4 modelViewMatrix){
	return viewProjectionMatrix * modelViewMatrix;
}



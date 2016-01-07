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
		(GLfloat)GraphicalEngine::Instance->resolution.Width
		/ GraphicalEngine::Instance->resolution.Height, 0.05, 50);
	this->setupCamera();
}

Camera::~Camera()
{
}

void Camera::CameraMotion(GLint x, GLint y)	//funkcja obracanie kamery myszka
{
	

	if ((x == GraphicalEngine::Instance->resolution.Width / 2 + 1 || x == GraphicalEngine::Instance->resolution.Width / 2 - 1 || x == GraphicalEngine::Instance->resolution.Width / 2)
		&& (y == GraphicalEngine::Instance->resolution.Height / 2 + 1 || y == GraphicalEngine::Instance->resolution.Height / 2 - 1 || y == GraphicalEngine::Instance->resolution.Height / 2)) return;
	ActiveCamera->pitch += ((GLdouble)x - GraphicalEngine::Instance->resolution.Width / 2) / 1000;
	ActiveCamera->yaw += ((GLdouble)y - GraphicalEngine::Instance->resolution.Height / 2) / 1000;
	if (ActiveCamera->pitch > PI2) ActiveCamera->pitch = ActiveCamera->pitch - PI2;
	if (ActiveCamera->pitch < 0) ActiveCamera->pitch = ActiveCamera->pitch + PI2;

	if (ActiveCamera->yaw > PIS2-0.1) ActiveCamera->yaw = PIS2 - 0.1;
	if (ActiveCamera->yaw < -PIS2+0.1) ActiveCamera->yaw = -PIS2 + 0.1;

	glm::mat4 rotationMatrix1, rotationMatrix2;

	glm::vec3 toMiddle = glm::normalize(Camera::ActiveCamera->position);

	glm::vec3 vec1;

	double longitude = atan2(Camera::ActiveCamera->position.x, Camera::ActiveCamera->position.y);

	if (longitude > 0) longitude -= PI2 / 2;
	else  longitude += PI2 / 2;
	longitude = (longitude * 360 / PI2);
	if(longitude>-135 && longitude<-45 || longitude<135 && longitude>45)
		vec1 = glm::cross(toMiddle, glm::vec3(0, 1, 0));
	else
		vec1 = glm::cross(toMiddle, glm::vec3(1, 0, 0));

	glm::vec3 vec2 = glm::cross(toMiddle, vec1);

	rotationMatrix1 = glm::rotate(glm::mat4(1), (float)ActiveCamera->yaw, vec2);
	rotationMatrix2 = glm::rotate(glm::mat4(1), (float)-ActiveCamera->pitch, toMiddle);

	vec1 = glm::vec3(rotationMatrix1* glm::vec4(vec1, 1));
	vec1 = glm::vec3(rotationMatrix2* glm::vec4(vec1, 1));

	
	
	ActiveCamera->axis = toMiddle;
	ActiveCamera->direction = vec1;

	glutWarpPointer(GraphicalEngine::Instance->resolution.Width / 2, GraphicalEngine::Instance->resolution.Height / 2);
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

void Camera::setPerspective(GLdouble fovY, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar)  //ustawi macierz prespektywy
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
	this->viewMatrix = glm::lookAt(this->position,
		glm::vec3(this->position.x + this->direction.x,
			this->position.y + this->direction.y,
			this->position.z + this->direction.z),
		this->axis);
	this->viewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}

glm::vec4 Camera::calculateModelViewProjMatrix(glm::vec4 modelViewMatrix){
	return viewProjectionMatrix * modelViewMatrix;
}


void Camera::moveForward(float power) {
	glm::vec3 cameraNewPosition;
	float speed = this->speed/60/60/60;
	if (power == 1) 		
		cameraNewPosition = this->position + (this->direction * speed);
	else 
		cameraNewPosition = this->position - (this->direction * speed);

		double distanceNew = glm::distance(cameraNewPosition, glm::vec3(0, 0, 0));
		double distance = glm::distance(this->position, glm::vec3(0, 0, 0));
		

		if (((this->hight + distanceNew - distance) < this->minHight) && distanceNew<distance)
			return;
		if (((this->hight + distanceNew - distance) > this->maxHight) && distanceNew>distance)
			return;
		this->position = cameraNewPosition;
}



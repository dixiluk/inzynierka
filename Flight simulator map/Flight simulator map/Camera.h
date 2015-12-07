#pragma once
#include "Engine.h"
#include <list>
//blah

class Camera
{
	friend class Engine;
public:
	static Camera* ActiveCamera;
	Camera(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 direction = glm::vec3(1, 0, 0), glm::vec3 axis = glm::vec3(0, 1, 0));
	~Camera();

	glm::mat4 projectionMatrix, viewMatrix;
	glm::mat4 viewProjectionMatrix;
	double fovY, zNear, zFar, aspectRatio;
	glm::vec3 position, direction, axis;
	double pitch, yaw;
	

	bool mouseMotion;

	void setActive();
	void setPosition(glm::vec3 position);
	void setDirection(glm::vec3 direction);
	void setFovY(GLdouble fovY);
	void setAspectRatio(GLdouble aspectRatio);
	void setZNear(GLdouble zNear);
	void setZFar(GLdouble zFar);
	void setDirectionToPosition(glm::vec3 position);
	void setPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

	glm::vec4 calculateModelViewProjMatrix(glm::vec4 modelViewMatrix);

private:
	static void CameraMotion(GLint x, GLint y);
	void setupCamera();
};

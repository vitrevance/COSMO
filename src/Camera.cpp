//#pragma once
#ifndef CAMERA_CPP
#define CAMERA_CPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

enum Direction {
	FORTH,
	BACK,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	CLOCKW,
	COUNTERCLOCKW,
};

class Camera {

private:
	GLfloat yaw = 0.;
	GLfloat pitch = 0.;
	mat4 lookMatrix;

	void updateLookMatrix() {
		lookMatrix = glm::lookAt(this->position, this->position + this->lookDir, upNormal);
		
	}

public:

	vec3 upNormal = vec3(0., 1., 0.);
	vec3 position;
	vec3 lookDir;
	mat4 projection;

	GLfloat zoom = 45.f;
	GLfloat movementSpeed = 6.f;
	GLfloat sensitivity = 0.1f;

	Camera() {
		position = vec3(0.);
		lookDir = vec3(0., 0., -1.);
	}

	Camera(vec3 pos, vec3 look) {
		position = pos;
		lookDir = look;
	}

	void setPosition(vec3 p) {
		this->position = p;
		updateLookMatrix();
	}

	void setDirection(vec3 p, vec3 n) {
		this->lookDir = glm::normalize(p);
		this->upNormal = n;
		updateLookMatrix();
	}

	void move(Direction dir, GLfloat delta) {
		GLfloat vel = this->movementSpeed * delta;
		if (dir == FORTH) {
			this->position += this->lookDir * vel;
		}
		if (dir == BACK) {
			this->position -= this->lookDir * vel;
		}
		if (dir == RIGHT) {
			this->position += glm::normalize(glm::cross(this->lookDir, this->upNormal)) * vel;
		}
		if (dir == LEFT) {
			this->position += glm::normalize(-glm::cross(this->lookDir, this->upNormal)) * vel;
		}
		if (dir == UP) {
			this->position.y += vel;
		}
		if (dir == DOWN) {
			this->position.y -= vel;
		}

		updateLookMatrix();
	}

	void processMouse(GLfloat xOff, GLfloat yOff, bool constrain = true) {
		xOff *= sensitivity;
		yOff *= sensitivity;

		yaw += xOff;
		pitch -= yOff;

		if (constrain) {
			if (pitch > 89.f) {
				pitch = 89.f;
			}
			if (pitch < -89.f) {
				pitch = -89.f;
			}
		}

		lookDir = glm::normalize(vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch))));

		updateLookMatrix();
	}

	mat4 look() {
		return lookMatrix;
	}

	vec3 right() {
		return glm::normalize(glm::cross(this->lookDir, this->upNormal));
	}
};

#endif

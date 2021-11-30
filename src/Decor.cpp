//#pragma once
#ifndef DECOR_CPP
#define DECOR_CPP

#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/compatibility.hpp>
#include <GLFW/glfw3.h>

#include "Model.h"
#include "Shader.h"
#include "Entity.h"
#include "ParticleController.h"
#include "Ray.h"

using glm::vec4;

class Decor:public Entity {

public:

	vec3 front;
	vec3 right;
	vec3 normal;
	vec3 scale;

	Decor(vec3 p, vec3 f, vec3 n, GLfloat s, Model * m) {
		this->model = m;
		this->pos = p;
		this->scale = vec3(s);
		this->normal = n;
		this->front = f;
		this->right = glm::normalize(glm::cross(this->front, this->normal));

		this->BS = s;
	}

	Decor(vec3 p, vec3 f, vec3 n, GLfloat bs, vec3 s, Model * m) {
		this->model = m;
		this->pos = p;
		this->scale = s;
		this->normal = n;
		this->front = f;
		this->right = glm::normalize(glm::cross(this->front, this->normal));

		this->BS = bs;
	}

	void render(Shader shader) {
		mat4 model(vec4(-this->front, 0.), vec4(this->normal, 0.), vec4(this->right, 0.), vec4(this->pos, 1.));

		model = glm::scale(model, this->scale * 0.0007f);

		GLint modelLoc = glGetUniformLocation(shader.Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		this->model->renderAll(shader);
	}

	GLfloat sd(vec3 p) {
		return glm::length(glm::max(glm::abs(p - this->pos) - vec3(this->scale.x, this->scale.y / 6.f, this->scale.z / 2.f), 0.f));
	}

	void hitBy(HitType hit, GLfloat amount, vec3 p) {
		ParticleController::instance->push(Particle(4, 10, p, vec3(0.), 0.8f, 0.3f, true));
	}

	GLboolean update(GLfloat delta, GLfloat t2) {
		return true;
	}

private:
	Model * model;
};

#endif

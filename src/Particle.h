#ifndef PARTICLE_H
#define PARTICLE_H

#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include "Texture.h"

#include <iostream>

using glm::vec3;

class Particle {

public:

	vec3 position;
	vec3 velocity;
	GLfloat scale;
	GLfloat rotation;
	GLfloat lifeTime;
	GLfloat lifeLength;
	GLint texAtlasID;
	GLint animateionLen;
	GLboolean additive;

	Particle();

	Particle(GLint t, GLint anilen, vec3 p, vec3 v, GLfloat s, GLfloat l, GLboolean useAdditive);

	void setPosition(vec3 p);

	void setScale(GLfloat s);

	GLboolean update(GLfloat delta);
};

#endif 
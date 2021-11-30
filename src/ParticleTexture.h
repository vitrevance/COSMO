#ifndef PARTICLETEXTURE_H
#define PARTICLETEXTURE_H

#include "Particle.h"
#include "Shader.h"

using glm::vec2;

class ParticleTexture {

public:

	static GLuint ATLAS;

	GLuint texture;
	GLint rows, cols;
	GLuint column;
	GLint index;
	GLfloat blend;
	vec2 offset1;
	GLfloat offset2;

	ParticleTexture();

	ParticleTexture(GLuint textureID, GLint rows, GLint cols);

	void update(Particle p);

	void syncShader(Shader &shader);
};

#endif
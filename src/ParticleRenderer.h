#ifndef PARTICLERENDERER_H
#define PARTICLERENDERER_H

#define GLM_ENABLE_EXPERIMENTAL
#include "Particle.h"
#include "Camera.cpp"
#include <vector>
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include "ParticleTexture.h"

class ParticleRenderer {

private:

	const GLint MaxParticles = 5000;
	GLfloat * part_matrix_array;

	GLuint VBO, VAO, EBO, IVBO;
	ParticleTexture texture;

	GLfloat * VBO_INFO;

public:

	static vec3 cp;

	static Shader particleShader;

	ParticleRenderer();

	void setup();

	void render(mat4 projection, Camera * camera, std::vector<Particle> * particles);

	mat4 getModelViewMatrix(Particle &p, mat4 view, Camera * camera);
	
};

bool sortParticles(const Particle & a, const Particle & b);

#endif
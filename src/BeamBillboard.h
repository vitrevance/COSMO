#ifndef BEAMBILLBOARD_H
#define BEAMBILLBOARD_H

#include "Shader.h"
#include "Camera.cpp"
#include <glm/gtc/type_ptr.hpp>

class BeamBillboard {

public:

	vec3 start;
	vec3 end;
	GLfloat width;
	GLfloat timer;
	GLboolean useOuterNorm;
	vec3 outerNorm;

	GLuint VAO, VBO, EBO, texture;

	BeamBillboard();

	BeamBillboard(vec3 p, vec3 e, GLfloat w, GLfloat lt, GLuint tex);

	void render(Shader & shader, Camera * camera);

	GLboolean update(GLfloat delta);
};

#endif
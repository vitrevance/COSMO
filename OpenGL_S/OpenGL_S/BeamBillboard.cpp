#include "BeamBillboard.h"

BeamBillboard::BeamBillboard() {
}

BeamBillboard::BeamBillboard(vec3 p, vec3 e, GLfloat w, GLfloat lt, GLuint tex) {
	this->start = p;
	this->end = e;
	this->width = w;
	this->texture = tex;
	this->timer = lt;
	this->useOuterNorm = false;

	GLfloat len = glm::length(e - p);

	GLfloat vertices[] =
	{
		// Positions							// Texture Coords
		len,  0.f,  w / 2.f,	0., 1., 0.,		1.0f, 0.0f, // Top Right
		len,  0.f, -w / 2.f,	0., 1., 0.,		1.0f, 1.0f, // Bottom Right
		0.f,  0.f, -w / 2.f,	0., 1., 0.,		0.0f, 1.0f, // Bottom Left
		0.f,  0.f,  w / 2.f,	0., 1., 0.,		0.0f, 0.0f  // Top Left
	};
	GLuint indices[] =
	{
		0, 1, 3,
		1, 2, 3 
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void BeamBillboard::render(Shader & shader, Camera * camera) {
	vec3 front = glm::normalize(this->end - this->start);
	vec3 norm;
	if (this->useOuterNorm)
		norm = this->outerNorm;
	else
		norm = glm::normalize(camera->position + front * glm::dot(camera->position - this->start, front) - this->start);
	mat4 model(vec4(front, 0.), vec4(norm, 0.), vec4(glm::cross(front, norm), 0.), vec4(this->start, 1.));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glBindVertexArray(this->VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

GLboolean BeamBillboard::update(GLfloat delta) {
	this->timer -= delta;
	return this->timer > 0.f;
}

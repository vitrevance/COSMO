#include "ParticleTexture.h"

GLuint ParticleTexture::ATLAS;

ParticleTexture::ParticleTexture() {
}

ParticleTexture::ParticleTexture(GLuint textureID, GLint rows, GLint cols) {
	this->texture = textureID;
	this->rows = rows;
	this->cols = cols;
	this->index = 0;
}

void ParticleTexture::update(Particle p) {
	this->column = p.texAtlasID;
	GLfloat ff = p.lifeTime / p.lifeLength * (p.animateionLen);
	this->index = glm::floor(ff);
	this->blend = glm::fract(ff);
	this->offset1.x = this->column / (GLfloat)this->cols;
	this->offset1.y = this->index / (GLfloat)this->rows;
	this->offset2 = this->index < p.animateionLen - 1 ? (this->index + 1) / (GLfloat)this->rows : offset1.y;
}

void ParticleTexture::syncShader(Shader &shader) {
	glUniform1f(glGetUniformLocation(shader.Program, "blend"), this->blend);
	glUniform1f(glGetUniformLocation(shader.Program, "rows"), (GLfloat)this->rows);
	glUniform1f(glGetUniformLocation(shader.Program, "cols"), (GLfloat)this->cols);
	glUniform2f(glGetUniformLocation(shader.Program, "offset1"), this->offset1.x, this->offset1.y);
	glUniform1f(glGetUniformLocation(shader.Program, "offset2"), this->offset2);
}
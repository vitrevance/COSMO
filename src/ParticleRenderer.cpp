#include "ParticleRenderer.h"
#include <algorithm>

Shader ParticleRenderer::particleShader;

vec3 ParticleRenderer::cp;

ParticleRenderer::ParticleRenderer() {
}

void ParticleRenderer::setup() {

	part_matrix_array = new GLfloat[this->MaxParticles * 22];

	this->texture = ParticleTexture(ParticleTexture::ATLAS, 24, 6);

	GLfloat vertices[] =
	{
		// Positions		// Texture Coords
		0.5, 0.5, 0.0f,		1.0f, 0.0f, // Top Right
		0.5, -0.5, 0.0f,    1.0f, 1.0f, // Bottom Right
		-0.5, -0.5, 0.0f,	0.0f, 1.0f, // Bottom Left
		-0.5,  0.5, 0.0f,	0.0f, 0.0f  // Top Left
	};
	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &IVBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, IVBO);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 16 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glBindVertexArray(0);
}

void ParticleRenderer::render(mat4 projection, Camera * camera, std::vector<Particle> * particles) {
	if (particles->size() > MaxParticles) {
		std::cout << "PARTICLERENDERER: Error: Too many particles" << std::endl;
		return;
	}
	ParticleRenderer::cp = camera->position;
	mat4 view = camera->look();
	ParticleRenderer::particleShader.Use();
	glBindVertexArray(this->VAO);
	glEnable(GL_BLEND);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture.texture);
	glUniformMatrix4fv(glGetUniformLocation(ParticleRenderer::particleShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(ParticleRenderer::particleShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	if (!particles->empty()) {
		std::sort(particles->begin(), particles->end(), sortParticles);
	}
	GLint i = 0;
	for (Particle p : *particles) {
		mat4 model = getModelViewMatrix(p, view, camera);
		this->texture.update(p);

		for (GLint j = 0; j < 16; j++)
			part_matrix_array[i++] = model[j / 4][j % 4];
		part_matrix_array[i++] = this->texture.offset1.x;
		part_matrix_array[i++] = this->texture.offset1.y;
		part_matrix_array[i++] = this->texture.offset2;
		part_matrix_array[i++] = this->texture.rows;
		part_matrix_array[i++] = this->texture.cols;
		part_matrix_array[i++] = this->texture.blend;
	}

	glBindBuffer(GL_ARRAY_BUFFER, IVBO);
	glBufferData(GL_ARRAY_BUFFER, particles->size() * 22 * sizeof(GLfloat), part_matrix_array, GL_STREAM_DRAW);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 22 * sizeof(GLfloat), (GLvoid *)(0 * sizeof(GLfloat)));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 22 * sizeof(GLfloat), (GLvoid *)(4 * sizeof(GLfloat)));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 22 * sizeof(GLfloat), (GLvoid *)(8 * sizeof(GLfloat)));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 22 * sizeof(GLfloat), (GLvoid *)(12 * sizeof(GLfloat)));
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 22 * sizeof(GLfloat), (GLvoid *)(16 * sizeof(GLfloat)));
	glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 22 * sizeof(GLfloat), (GLvoid *)(19 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	glEnableVertexAttribArray(7);

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);
	
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, particles->size());

	glDisable(GL_BLEND);

	glBindVertexArray(0);
}

mat4 ParticleRenderer::getModelViewMatrix(Particle &p, mat4 view, Camera * camera) {
	mat4 model(1.);

	model = glm::translate(model, p.position);
	for (short i = 0; i < 3; i++) {
		for (short j = 0; j < 3; j++) {
			model[i][j] = view[j][i];
		}
	}

	model = glm::rotate(model, p.rotation, vec3(0., 0., 1.));
	model = glm::scale(model, vec3(p.scale));
	return model;
}

bool sortParticles(const Particle & a, const Particle & b) {
	return glm::length(ParticleRenderer::cp - a.position) > glm::length(ParticleRenderer::cp - b.position);
}
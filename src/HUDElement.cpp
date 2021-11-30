#ifndef HUDELEMENT_H
#define HUDELEMENT_H


#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "Shader.h"
#include "Mesh.cpp"

#include <string>

using glm::vec4;

class HUDElement {

public:

	vec2 position;
	vec2 translation;
	vec2 size, resize;
	vec4 offset;
	string dir;
	GLuint texture;
	GLuint VBO, VAO, EBO;

	HUDElement() {}

	HUDElement(string imgDir, vec2 pos, vec2 esize, vec4 texOffset = vec4(1, 1, 0, 0)) {
		this->dir = imgDir;
		this->position = pos * vec2(2., -2.) + vec2(-1., 1.);
		this->translation = vec2(0.);
		this->size = vec2(esize.x * 2.f, esize.y * -2.f);
		this->offset = texOffset;
		this->resize = vec2(1);

		this->texture = TextureLoading::LoadTexture(this->dir.c_str());
		this->setupMesh();
	}

	HUDElement(GLuint img, vec2 pos, vec2 esize, vec4 texOffset = vec4(1, 1, 0, 0)) {
		//this->dir = imgDir;
		this->position = pos * vec2(2., -2.) + vec2(-1., 1.);
		this->translation = vec2(0.);
		this->size = vec2(esize.x * 2.f, esize.y * -2.f);
		this->offset = texOffset;
		this->resize = vec2(1);

		this->texture = img;
		this->setupMesh();
	}

	void render(Shader &shader) {

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDisable(GL_DEPTH_TEST);

		shader.Use();
		glBindTexture(GL_TEXTURE_2D, this->texture);
		glUniform2f(glGetUniformLocation(shader.Program, "translation"), this->translation.x, this->translation.y);
		glUniform2f(glGetUniformLocation(shader.Program, "resize"), this->resize.x, this->resize.y);
		glUniform4f(glGetUniformLocation(shader.Program, "offset"), this->offset.x, this->offset.y, this->offset.z, this->offset.w);

		// Draw container
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glEnable(GL_DEPTH_TEST);
	}

	void translate(vec2 p) {
		this->translation = vec2(p.x * 2.f, p.y * -2.f);
	}

	void translateAdd(vec2 p) {
		this->translation += vec2(p.x * 2.f, p.y * -2.f);
	}

	void rescale(vec2 scale) {
		this->resize = scale;
	}

	void offset4(vec4 o) {
		this->offset = o;
	}
	
	void offset2(vec2 o) {
		this->offset = vec4(this->offset.x, this->offset.y, o.x, o.y);
	}

private:

	void setupMesh() {
		GLfloat vertices[] =
		{
			// Positions								           // Texture Coords
			position.x + size.x, position.y, 0.0f,				1.0f, 0.0f, // Top Right
			position.x + size.x, position.y + size.y, 0.0f,     1.0f, 1.0f, // Bottom Right
			position.x, position.y + size.y, 0.0f,				0.0f, 1.0f, // Bottom Left
			position.x,  position.y, 0.0f,						0.0f, 0.0f  // Top Left
		};
		GLuint indices[] = {
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

		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}
};

#endif
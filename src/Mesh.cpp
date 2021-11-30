//#pragma once
#ifndef MESH_CPP
#define MESH_CPP

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/postprocess.h>

#include "Shader.h"

using namespace std;
using glm::vec2;
using glm::vec3;
using glm::mat4;

struct Vertex {
	vec3 Pos;
	vec3 Normal;
	vec2 TextureCoords;
	Vertex() {}
	Vertex(vec3 p, vec3 n, vec2 t) {
		this->Pos = p;
		this->Normal = n;
		this->TextureCoords = t;
	}
};

struct Texture {
	GLuint index;
	string type;
	aiString path;
};

class Mesh {

public:
	vector<Vertex> verts;
	vector<GLuint> ids;
	vector<Texture> textures;

	Mesh() {};

	Mesh(vector<Vertex> vs, vector<GLuint> is, vector<Texture> ts) {
		this->verts = vs;
		this->ids = is;
		this->textures = ts;
		Setup();
	}

	void render(Shader shader) {

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GLuint difnum = 1;
		GLuint specnum = 1;
		for (GLuint i = 0; i < this->textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			stringstream ss;
			string num;
			string name = this->textures[i].type;

			if (name == "tex_diffuse") {
				ss << difnum++;
			}
			else if (name == "tex_specular") {
				ss << specnum++;
			}

			num = ss.str();

			glUniform1i(glGetUniformLocation(shader.Program, (name + num).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, this->textures[i].index);
		}

		glUniform1f(glGetUniformLocation(shader.Program, "material.shine"), 16.0f);

		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->ids.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glDisable(GL_BLEND);

		for (GLuint i = 0; i < this->textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

private:
	GLuint VAO, VBO, EBO;

	void Setup() {
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

		glBufferData(GL_ARRAY_BUFFER, this->verts.size() * sizeof(Vertex), &this->verts[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->ids.size() * sizeof(GLuint), &this->ids[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, TextureCoords));

		glBindVertexArray(0);

	}
};

#endif

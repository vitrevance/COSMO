//#pragma once
#ifndef MODEL_CPP
#define MODEL_CPP

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "SOIL2/SOIL2.h"

#include "Mesh.cpp"

using namespace std;
using glm::vec2;
using glm::vec3;
using glm::mat4;


GLint textureFromFile(const char * path, string dir) {
	string file = dir + '/' + string(path);
	GLuint id;
	glGenTextures(1, &id);

	int width, height;
	unsigned char * image = SOIL_load_image(file.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(image);
	return id;
}

class Model {

public:
	Model() {

	}

	Model(const GLchar * path) {
		this->loadModel(path);
	}

	void renderAll(Shader shader) {
		for (GLuint i = 0; i < this->meshes.size(); i++) {
			this->meshes[i].render(shader);
		}
	}

private:
	vector<Mesh> meshes;
	vector<Texture> loaded_textures;
	string dir;

	void loadModel(string path) {
		Assimp::Importer importer;
		const aiScene * scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			cout << "ERROR::ASSIMP " << importer.GetErrorString() << endl;
			return;
		}

		this->dir = path.substr(0, path.find_last_of('/'));

		this->processNode(scene->mRootNode, scene);
	}

	void processNode(aiNode * node, const aiScene * scene) {
		for (GLuint i = 0; i < node->mNumMeshes; i++) {
			aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];

			this->meshes.push_back(this->processMesh(mesh, scene));
		}
		for (GLuint i = 0; i < node->mNumChildren; i++) {
			this->processNode(node->mChildren[i], scene);
		}
	}

	Mesh processMesh(aiMesh * mesh, const aiScene * scene) {
		vector<Vertex> verts;
		vector<GLuint> ids;
		vector<Texture> texs;

		for (GLuint i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;
			vec3 vect;

			vect.x = mesh->mVertices[i].x;
			vect.y = mesh->mVertices[i].y;
			vect.z = mesh->mVertices[i].z;
			vertex.Pos = vect;

			vect.x = mesh->mNormals[i].x;
			vect.y = mesh->mNormals[i].y;
			vect.z = mesh->mNormals[i].z;
			vertex.Normal = vect;

			if (mesh->mTextureCoords[0]) {
				vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;

				vertex.TextureCoords = vec;
			}
			else {
				vertex.TextureCoords = vec2(0.);
			}

			verts.push_back(vertex);
		}

		for (GLuint i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (GLuint j = 0; j < face.mNumIndices; j++) {
				ids.push_back(face.mIndices[j]);
			}
		}

		if (mesh->mMaterialIndex >= 0) {
			aiMaterial * mat = scene->mMaterials[mesh->mMaterialIndex];

			vector<Texture> diff = this->loadMaterials(mat, aiTextureType_DIFFUSE, "tex_diffuse");
			texs.insert(texs.end(), diff.begin(), diff.end());

			vector<Texture> spec = this->loadMaterials(mat, aiTextureType_SPECULAR, "tex_specular");
			texs.insert(texs.end(), spec.begin(), spec.end());
		}

		return Mesh(verts, ids, texs);
	}

	vector<Texture> loadMaterials(aiMaterial * mat, aiTextureType type, string name) {
		vector<Texture> ret;

		for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);

			GLboolean skip = false;

			for (GLuint j = 0; j < this->loaded_textures.size(); j++) {
				if (this->loaded_textures[j].path == str) {
					ret.push_back(this->loaded_textures[j]);
					skip = true;
					break;
				}
			}

			if (!skip) {
				Texture tex;
				tex.index = textureFromFile(str.C_Str(), this->dir);
				tex.type = name;
				tex.path = dir;
				ret.push_back(tex);

				this->loaded_textures.push_back(tex);
			}
		}
		return ret;
	}
};

#endif

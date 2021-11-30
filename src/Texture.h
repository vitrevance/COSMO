#ifndef TEXTURELOADING_H
#define TEXTURELOADING_H

#define GLEW_STATIC
#include <GL/glew.h>
#include "SOIL2/SOIL2.h"
#include <vector>
#include <map>


class TextureLoading {

public:

	static std::map<std::string, GLuint> loadedTextures;

	static GLuint LoadTexture(std::string path);

	static GLuint LoadCubemap(std::vector<const GLchar * > faces);

	static GLuint LoadCubemap(std::vector<std::string> faces);

	static GLuint LoadCubemap(std::string dir, std::string type);
};

#endif
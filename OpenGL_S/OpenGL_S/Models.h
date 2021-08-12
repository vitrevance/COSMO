#ifndef MODELS_H
#define MODELS_H

#include "Model.h"
#include "ParticleRenderer.h"

class Models {

public:

	static Model player;
	static Model enemy;
	static Model enemy1;
	static Model enemy2;
	static Model asteroid1;

	static GLuint numbersTexture;
	static GLuint easterTexture;

	static Shader modelShader;
	static Shader hudShader;
	static Shader cubemapShader;
	static Shader beamShader;

	static void load();
};

#endif
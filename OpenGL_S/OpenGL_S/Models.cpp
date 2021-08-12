#include "Models.h"
#include "ParticleTexture.h"

Model Models::player;
Model Models::enemy;
Model Models::enemy1;
Model Models::enemy2;
Model Models::asteroid1;

GLuint Models::numbersTexture;
GLuint Models::easterTexture;

Shader Models::modelShader;
Shader Models::hudShader;
Shader Models::cubemapShader;
Shader Models::beamShader;

void Models::load() {
	Models::player = Model("resources/models/Corvette-F3/Corvette-F31.obj");
	Models::enemy = Model("resources/models/Free_SciFi-Fighter/SciFi_Fighter_AK5_R.obj");
	Models::enemy1 = Model("resources/models/Federation Interceptor HN48/Federation Interceptor HN48 flying_R.obj");
	Models::enemy2 = Model("resources/models/malice/export_R.obj");

	Models::asteroid1 = Model("resources/models/asteroid1/OBJ_R_S.obj");

	ParticleTexture::ATLAS = TextureLoading::LoadTexture("resources/textures/atlas.png");

	Models::numbersTexture = TextureLoading::LoadTexture("resources/textures/numbers.png");
	Models::easterTexture = TextureLoading::LoadTexture("resources/textures/rainbow.png");

	Models::modelShader = Shader("resources/shaders/model.vsource", "resources/shaders/model.fsource");
	Models::hudShader = Shader("resources/shaders/hud.vsource", "resources/shaders/hud.fsource");
	Models::cubemapShader = Shader("resources/shaders/cubemap.vsource", "resources/shaders/cubemap.fsource");
	Models::beamShader = Shader("resources/shaders/beam.vsource", "resources/shaders/beam.fsource");
	ParticleRenderer::particleShader = Shader("resources/shaders/particle.vsource", "resources/shaders/particle.fsource");
}
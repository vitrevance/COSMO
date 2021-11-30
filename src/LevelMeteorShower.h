#ifndef LEVELMETEORSHOWER_H
#define LEVELMETEORSHOWER_H

#include "GameLevel.h"

class LevelMeteorShower :public GameLevel {

public:

	Camera mainCamera;

	ParticleRenderer particleRenderer;

	MainGUI hud;
	Cubemap sky;

	Shader shader, hudShader;
	HUDElement cursor;
	mat4 projection;

	GLint WIDTH, HEIGHT;

	Spaceship * ship;
	GLint shipScreenLoc = 7;


	LevelMeteorShower(GLFWwindow * window);

	GLfloat spawnTimer;

	void setup();

	void mainCycle();

	void handleShipMovement();
};


#endif

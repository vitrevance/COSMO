#ifndef LEVELEASTER_H
#define LEVELEASTER_H

#include "GameLevel.h"

class LevelEaster:public GameLevel {

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
	GLint shipScreenLoc = 4;

	HUDElement intro;

	GLfloat animation;

	LevelEaster(GLFWwindow * window);

	void setup();

	void mainCycle();

	void handleShipMovement();
};


#endif
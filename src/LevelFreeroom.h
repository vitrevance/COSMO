#ifndef LEVELFREEROOM_H
#define LEVELFREEROOM_H

#include "GameLevel.h"

class LevelFreeroom :public GameLevel {

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

	LevelFreeroom(GLFWwindow * window);

	void setup();

	void mainCycle();

	void handleShipMovement();
};

#endif
#ifndef LEVELDEFAULT_H
#define LEVELDEFAULT_H

#include "GameLevel.h"

class LevelDefault:public GameLevel {

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


	LevelDefault(GLFWwindow * window);

	void setup();

	void mainCycle();

	void handleShipMovement();
};


#endif
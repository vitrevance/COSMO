#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Cubemap.cpp"
#include "Spaceship.cpp"
#include "Decor.cpp"
#include "EntityController.h"
#include "Enemy.h"
#include "MainGUI.h"
#include "Input.h"
#include "Asteroid.h"

class GameLevel {

public:

	GLfloat deltaTime = 0.;
	GLfloat lastTime = 0.;

	GLFWwindow * window;

	GameLevel();

	GameLevel(GLFWwindow * window);

	virtual void setup();

	virtual void mainCycle();
};

#endif
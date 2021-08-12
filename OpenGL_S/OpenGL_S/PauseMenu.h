#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "MenuScreen.h"
#include "GameManager.h"

class PauseMenu:public MenuScreen {

public:

	GLint WIDTH, HEIGHT;

	GLFWwindow * window;

	HUDElement overlay;
	HUDElement cursor;
	HUDElement deathScreen;
	NumericHUD player_score;

	Shader hudShader;

	Button start, quit;

	Cubemap bkground;

	mat4 projection, view;

	PauseMenu();

	PauseMenu(GLFWwindow * w);

	void render();
};

#endif
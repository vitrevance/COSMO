#ifndef MAINMENU_H
#define MAINMENU_H

#include "NumericHUD.h"
#include "Cubemap.cpp"
#include "MenuScreen.h"
#include "GameManager.h"

class MainMenu:public MenuScreen {

public:

	GLint WIDTH, HEIGHT;

	GLFWwindow * window;

	HUDElement overlay;
	HUDElement cursor;
	HUDElement level_pre;
	Cubemap bkground;

	mat4 projection, view;

	Shader hudShader;

	Button start, quit, prev, next;

	MainMenu();
	MainMenu(GLFWwindow * w);

	void render();
};

#endif
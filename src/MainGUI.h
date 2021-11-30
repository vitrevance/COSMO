#ifndef MAINGUI_H
#define MAINGUI_H

#include "NumericHUD.h"
#include "Spaceship.cpp"

class MainGUI {

private:

	Spaceship * player;
	HUDElement n, p, l, cd;
	NumericHUD health, score, capacity;
	vec2 cds;
	vec2 cdi[3];

public:

	MainGUI();

	MainGUI(Spaceship * player);

	void render(Shader & shader);
};

#endif
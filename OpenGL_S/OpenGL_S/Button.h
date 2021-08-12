#ifndef BUTTON_H
#define BUTTON_H

#include "HUDElement.cpp"
#include "Input.h"

enum ButtonType {
	START,
	QUIT,
	RESUME,
	MENU,
	PREV,
	NEXT
};

class Button {

public:

	HUDElement texture;
	vec2 pos, size;

	Button();

	Button(vec2 pos, vec2 size, ButtonType type);

	GLboolean update(Shader & shader);
};

#endif
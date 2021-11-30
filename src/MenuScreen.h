#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Button.h"

class MenuScreen {

public:
	MenuScreen();

	virtual void render();
};

#endif
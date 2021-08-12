#ifndef NUMERICHUD_H
#define NUMERICHUD_H

#include "HUDElement.cpp"
#include "Models.h"

class NumericHUD {

private:

	GLint num;
	GLint * nump;
	GLboolean useNump;
	vec2 pos;
	vec2 size;
	GLboolean right;

	HUDElement img;

	vector<GLint> offsets;

	void numToVector(GLint n);

public:

	NumericHUD();

	NumericHUD(vec2 p, vec2 s, GLint n);

	NumericHUD(vec2 p, vec2 s, GLint * n, GLboolean right = false);

	void render(Shader & shader);
};

#endif
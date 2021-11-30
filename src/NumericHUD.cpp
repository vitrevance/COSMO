#include "NumericHUD.h"

void NumericHUD::numToVector(GLint n) {
	this->offsets.clear();
	if (n == 0) {
		this->offsets.push_back(0);
		return;
	}
	for (n; n > 0; n /= 10) {
		this->offsets.insert(this->offsets.begin(), n % 10);
	}
}

NumericHUD::NumericHUD() {

}

NumericHUD::NumericHUD(vec2 p, vec2 s, GLint n) {
	this->pos = p;
	this->size = s;
	this->num = n;
	this->useNump = false;

	this->img = HUDElement(Models::numbersTexture, this->pos, this->size, vec4(10, 1, 0, 0));

	this->numToVector(n);
}

NumericHUD::NumericHUD(vec2 p, vec2 s, GLint * n, GLboolean right) {
	this->pos = p;
	this->size = s;
	this->num = *n;
	this->nump = n;
	this->useNump = true;
	this->right = right;

	this->img = HUDElement(Models::numbersTexture, this->pos, this->size, vec4(10, 1, 0, 0));

	this->numToVector(*n);
}

void NumericHUD::render(Shader & shader) {
	if (this->useNump) {
		if (this->num != *this->nump) {
			this->num = *this->nump;
			this->numToVector(this->num);
		}

		if (this->right)
			this->img.translate(vec2(-this->size.x * this->offsets.size(), 0.));
		else
			this->img.translate(vec2(0.));

		for (GLint i : this->offsets) {
			this->img.offset2(vec2((GLfloat)i / 10.f, 0.f));
			this->img.render(shader);
			this->img.translateAdd(vec2(this->size.x, 0.));
		}
	}
	else {
		this->img.translate(vec2(0.));
		for (GLint i : this->offsets) {
			this->img.offset2(vec2((GLfloat)i / 10.f, 0.f));
			this->img.render(shader);
			this->img.translateAdd(vec2(this->size.x, 0.));
		}
	}
}

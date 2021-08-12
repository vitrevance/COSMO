#include "Button.h"

Button::Button() {
}

Button::Button(vec2 pos, vec2 size, ButtonType type) {
	this->pos = pos;
	this->size = size;
	if (type == START) {
		this->texture = HUDElement("resources/textures/button_START.png", pos, size, vec4(1, 2, 0, 0));
	}
	if (type == QUIT) {
		this->texture = HUDElement("resources/textures/button_QUIT.png", pos, size, vec4(1, 2, 0, 0));
	}
	if (type == RESUME) {
		this->texture = HUDElement("resources/textures/button_RESUME.png", pos, size, vec4(1, 2, 0, 0));
	}
	if (type == MENU) {
		this->texture = HUDElement("resources/textures/button_MENU.png", pos, size, vec4(1, 2, 0, 0));
	}
	if (type == PREV) {
		this->texture = HUDElement("resources/textures/button_NEXT.png", pos + vec2(size.x, 0), vec2(-size.x, size.y), vec4(1, 2, 0, 0));
	}
	if (type == NEXT) {
		this->texture = HUDElement("resources/textures/button_NEXT.png", pos, size, vec4(1, 2, 0, 0));
	}
}

GLboolean Button::update(Shader & shader) {
	if (Input::mousePosNormalized.x > this->pos.x && Input::mousePosNormalized.y > this->pos.y && Input::mousePosNormalized.x < (this->pos + this->size).x && Input::mousePosNormalized.y < (this->pos + this->size).y) {
		this->texture.offset2(vec2(0, 0.5f));
		this->texture.render(shader);
		return true;
	}
	else {
		this->texture.offset2(vec2(0, 0));
		this->texture.render(shader);
		return false;
	}
}

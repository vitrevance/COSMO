#include "MainGUI.h"

MainGUI::MainGUI() {
}

MainGUI::MainGUI(Spaceship * player) {
	this->player = player;
	this->n = HUDElement("resources/textures/main_Normal.png", vec2(0.), vec2(1., 1.));
	this->p = HUDElement("resources/textures/main_Plasma.png", vec2(0.), vec2(1., 1.));
	this->l = HUDElement("resources/textures/main_Laser.png", vec2(0.), vec2(1., 1.));

	this->cdi[0] = vec2(0.42265625f, 0.90694f);
	this->cdi[1] = vec2(0.475f, 0.90694f);
	this->cdi[2] = vec2(0.52734375f, 0.90694f);
	this->cds = vec2(0.05f, 0.0888889f);

	this->cd = HUDElement("resources/textures/cd.png", vec2(0., 0.), vec2(0.05f, 0.0888889f));

	this->health = NumericHUD(vec2(0.01, 0.92), vec2(0.04, 0.07), &this->player->health);
	this->capacity = NumericHUD(vec2(0.99, 0.92), vec2(0.04, 0.07), &this->player->capacity, true);
	this->score = NumericHUD(vec2(1., 0.75), vec2(0.04, 0.07), &this->player->score, true);
}

void MainGUI::render(Shader & shader) {

	if (this->player->getWeapon() == 0 || this->player->getWeapon() == 3)
		n.render(shader);
	if (this->player->getWeapon() == 1)
		p.render(shader);
	if (this->player->getWeapon() == 2)
		l.render(shader);
	for (int i = 0; i < 3; i++) {
		if (this->player->getCooldown(i) / this->player->shootCooldown(i) > 0.f) {
			vec2 res = vec2(0., 0.5 * (this->player->getCooldown(i) / this->player->shootCooldown(i)));
			vec2 tr = vec2(0., this->cds.y * (this->player->getCooldown(i) / this->player->shootCooldown(i)));
			cd.translate(this->cdi[i] - res + tr);
			cd.rescale(vec2(1., 1.f - this->player->getCooldown(i) / this->player->shootCooldown(i)));
			cd.offset4(vec4(1.f, 1.f / ( 1.f - this->player->getCooldown(i) / this->player->shootCooldown(i)), 0.f, (this->player->getCooldown(i) / this->player->shootCooldown(i))));
			cd.render(shader);
		}
	}
	this->health.render(shader);
	this->capacity.render(shader);
	this->score.render(shader);
}

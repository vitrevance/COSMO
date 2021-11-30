#include "BulletController.h"

BulletController::BulletController() {
	this->bullets.clear();
}

void BulletController::push(Bullet b) {
	this->bullets.push_back(b);
}

void BulletController::update(GLfloat delta) {
	vector<Bullet> neww;
	for (int i = 0; i < this->bullets.size(); i++) {
		if (this->bullets[i].update(delta)) {
			neww.push_back(this->bullets[i]);
		}
	}
	this->bullets = neww;
}

BulletController * BulletController::instance = nullptr;

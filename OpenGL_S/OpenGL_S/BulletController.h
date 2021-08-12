#ifndef BULLETCONTROLLER_H
#define BULLETCONTROLLER_H

#include "Bullet.h"

class BulletController {

public:

	static BulletController * instance;

	std::vector<Bullet> bullets;

	BulletController();

	void push(Bullet b);

	void update(GLfloat delta);
};

#endif
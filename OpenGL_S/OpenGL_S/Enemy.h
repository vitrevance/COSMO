#ifndef ENEMY_H
#define ENEMY_H

#include "EntityController.h"
#include "Models.h"
#include <glm/gtc/type_ptr.hpp>
#include "ParticleController.h"
#include "BulletController.h"
#include "Spaceship.cpp"

class Enemy:public Entity {

private:

	vec3 front, norm, right;
	GLfloat scale;
	GLfloat hitTimer;
	GLfloat regenTimer, attackTimer;
	GLint maxHealth, maxCapacity;
	Model * modell;

public:

	GLfloat shield, damage;
	GLint health, capacity;

	Enemy();

	Enemy(GLint hp, GLint scap, GLfloat shiealding, GLfloat damage, Model * model);

	GLboolean update(GLfloat delta, GLfloat t2);

	void render(Shader shader);

	GLfloat sd(vec3 p);

	void hitBy(HitType hit, GLfloat amount, vec3 p);

	void regenHP(GLint a);

	void regenSC(GLint a);

	GLfloat convFun(vec3 l);

	vec3 matrixExp(GLfloat t);

	void setScale(GLfloat s);
};

#endif
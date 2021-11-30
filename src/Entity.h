#ifndef ENTITY_H
#define ENTITY_H

#include "Camera.cpp"
#include "Shader.h"

enum HitType {
	OBJECT,
	WEAPON
};

class Entity {

public:

	vec3 pos;
	GLfloat BS;

	virtual GLfloat sd(vec3 p);

	virtual void hitBy(HitType hit, GLfloat amount, vec3 p);

	virtual GLboolean update(GLfloat delta, GLfloat t2);

	virtual void render(Shader shader);

	virtual GLboolean isPlayer();

	virtual void collide(Entity * e);
};

#endif
#ifndef ASTEROID_H
#define ASTEROID_H

#define GLM_ENABLE_EXPERIMENTAL

#include "EntityController.h"
#include "ParticleController.h"
#include "Model.h"
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"
#include "Models.h"
#include <glm/gtx/compatibility.hpp>

class Asteroid:public Entity {

private:

	Model * mesh;
	GLfloat r;
	vec3 front;
	vec3 right;
	vec3 norm;
	GLint hp;
	GLint maxHp;
	GLint type;
	vec3 velocity;
	GLfloat slowing;

public:

	Asteroid();

	Asteroid(vec3 p, GLfloat radi);

	Asteroid(vec3 p, GLfloat radi, GLint t, GLint hp = 40);

	GLboolean update(GLfloat delta, GLfloat t2);

	void render(Shader shader);

	GLfloat sd(vec3 p);

	void hitBy(HitType hit, GLfloat amount, vec3 p);

	void collide(Entity * e);

	void applyVelocity(vec3 v);

	void setSlowingFactor(GLfloat f);

	void setHP(GLint h);
};

#endif
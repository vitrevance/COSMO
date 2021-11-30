#ifndef BULLET_H
#define BULLET_H

#include <glm/gtc/type_ptr.hpp>

#include "Models.h"
#include "ParticleController.h"
#include "glm/gtc/random.hpp"
#include "Ray.h"
#include "BeamController.h"

using glm::vec4;

enum BulletType {
	NORMAL,
	LASER,
	PLASMA,
	EASTER
};

class Bullet {

public:

	BulletType type;
	GLfloat width, len, maxDist, speed, dist, time, damage;
	vec3 color;
	vec3 front, pos;
	Entity * shooter;

	Bullet();

	Bullet(BulletType t, GLfloat w, GLfloat l, GLfloat m, GLfloat s, vec3 p, vec3 dir, Entity * shooter, GLfloat damage = -10.f);

	GLboolean update(GLfloat delta);
};

#endif
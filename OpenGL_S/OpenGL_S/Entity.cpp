#include "Entity.h"

GLfloat Entity::sd(vec3 p) {
	return 0.f;
}

void Entity::hitBy(HitType hit, GLfloat amount, vec3 p) {
}

GLboolean Entity::update(GLfloat delta, GLfloat t2) {
	return false;
}

void Entity::render(Shader shader) {
}

GLboolean Entity::isPlayer() {
	return false;
}

void Entity::collide(Entity * e) {
}

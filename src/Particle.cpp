#include "Particle.h"

Particle::Particle() {
	this->position = vec3(0.);
	this->velocity = vec3(0.);
	this->scale = 0.f;
	this->lifeTime = 0.f;
	this->rotation = 0.f;
	this->texAtlasID = 0;
}

Particle::Particle(GLint t, GLint anilen, vec3 p, vec3 v, GLfloat s, GLfloat l, GLboolean useAdditive) {
	this->position = p;
	this->velocity = v;
	this->scale = s;
	this->texAtlasID = t;
	this->lifeTime = 0.f;
	this->lifeLength = l;
	this->rotation = 0.f;
	this->animateionLen = anilen;
	this->additive = useAdditive;
}

void Particle::setPosition(vec3 p) {
	this->position = p;
}

void Particle::setScale(GLfloat s) {
	this->scale = s;
}

GLboolean Particle::update(GLfloat delta) {
	this->position += this->velocity * delta;
	this->lifeTime += delta;
	return this->lifeTime < this->lifeLength;
}
#include "Asteroid.h"

Asteroid::Asteroid() {
	
}

Asteroid::Asteroid(vec3 p, GLfloat radi) {
	this->pos = p;
	this->r = radi;
	this->BS = r;

	this->front = vec3(0., 0., 1.);
	this->norm = vec3(0., 1., 0.);
	this->right = glm::cross(this->front, this->norm);
	this->mesh = &Models::asteroid1;
	this->type = rand() % 5;

	if (this->type == 4) {
		this->hp = 150;
	}
	else
		this->hp = 50;

	this->maxHp = hp;
}

Asteroid::Asteroid(vec3 p, GLfloat radi, GLint t, GLint hp) {
	this->pos = p;
	this->r = radi;
	this->BS = r;

	this->slowing = 1.f;

	this->front = vec3(0., 0., 1.);
	this->norm = vec3(0., 1., 0.);
	this->right = glm::cross(this->front, this->norm);
	this->mesh = &Models::asteroid1;

	this->type = t;

	if (this->type == 4) {
		this->hp = hp * 4;
	}
	else
		this->hp = hp;

	this->maxHp = hp;
}

GLboolean Asteroid::update(GLfloat delta, GLfloat t2) {

	this->pos += this->velocity * delta;
	this->velocity = glm::lerp(this->velocity, vec3(0.), delta * this->slowing);

	if (this->hp == 0) {
		GLint ran = rand() % 150 + 25;
		for (GLint i = 0; i < ran; i++) {
			ParticleController::instance->push(Particle(3, 11, this->pos, Ray::randVec3(5.5f, 22.5f), Ray::randFloat(3.f, 12.f), Ray::randFloat(1.5f, 3.6f), false));
		}

		if (this->r > 6.f) {


			
			EntityController::instance->push(new Asteroid(this->pos + vec3(0., 0.01, 0.), std::min(this->r * 0.8f, 6.f), 0, this->maxHp * 0.8f));
			EntityController::instance->push(new Asteroid(this->pos + vec3(0.01, 0., 0.), std::min(this->r * 0.8f, 6.f), 1, this->maxHp * 0.8f));
			EntityController::instance->push(new Asteroid(this->pos + vec3(-0.01, 0., 0.), std::min(this->r * 0.8f, 6.f), 2, this->maxHp * 0.8f));
			EntityController::instance->push(new Asteroid(this->pos + vec3(0., 0., 0.01), std::min(this->r * 0.8f, 6.f), 3, this->maxHp * 0.8f));
		}
	}
	
	return this->hp > 0;
}

void Asteroid::render(Shader shader) {
	mat4 model(vec4(-this->front, 0.), vec4(this->norm, 0.), vec4(this->right, 0.), vec4(this->pos, 1.));

	model = glm::scale(model, vec3(this->r));

	GLint modelLoc = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	if (this->type == 4)
		this->mesh->renderAll(shader);
	else
		this->mesh->renderPart(shader, this->type);
}

GLfloat Asteroid::sd(vec3 p) {
	return glm::length(p - this->pos) - this->r;
}

void Asteroid::hitBy(HitType hit, GLfloat amount, vec3 p) {
	if (hit != OBJECT) {
		this->hp -= amount;
		if (this->hp < 0)
			this->hp = 0;
	}
}

void Asteroid::collide(Entity * e) {
	e->hitBy(OBJECT, 20.f * this->BS, this->pos);
	if (glm::length(this->pos - e->pos) == 0.f)
		this->pos += Ray::randVec3(1.f, 5.f) / 100.f;
	this->velocity = glm::normalize(this->pos - e->pos) * 30.f;
}

void Asteroid::applyVelocity(vec3 v) {
	this->velocity = v;
}

void Asteroid::setSlowingFactor(GLfloat f) {
	this->slowing = f;
}

void Asteroid::setHP(GLint h) {
	this->hp = h;
}

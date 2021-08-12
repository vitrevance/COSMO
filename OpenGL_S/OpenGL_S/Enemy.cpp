#include "Enemy.h"

Enemy::Enemy() {
}

Enemy::Enemy(GLint hp, GLint scap, GLfloat shiealding, GLfloat damage, Model * model) {
	this->health = hp;
	this->capacity = scap;
	this->shield = shiealding;
	this->damage = damage;

	this->pos = vec3(0., 100, 0.);
	this->front = vec3(0., 0., 1.);
	this->norm = vec3(0., 1., 0.);
	this->right = glm::normalize(glm::cross(this->front, this->norm));

	this->scale = 10.f;
	this->BS = this->scale;
	
	this->hitTimer = 0.f;
	this->regenTimer = 0.f;
	this->maxCapacity = scap;
	this->maxHealth = hp;

	this->modell = model;
}

GLboolean Enemy::update(GLfloat delta, GLfloat t2) {

	this->hitTimer -= delta;
	this->regenTimer += delta;
	this->attackTimer -= delta;
	if (this->regenTimer > 2.f) {
		this->regenSC(10);
		this->regenHP(5);
		this->regenTimer = 0.f;

		
	}

	if (this->attackTimer < 0.f) {
		this->attackTimer = 0.4f;
		if (EntityController::instance->thePlayer != nullptr)
			if (glm::length(this->pos - EntityController::instance->thePlayer->pos) < 400.f) {
				BulletController::instance->push(Bullet(NORMAL, 0.7f, 0.05f, 450.f, 1000.f, this->pos, this->front, this, this->damage));
				SoundManager::engine->play3D(SoundManager::pew1, irrklang::vec3df(this->pos.x, this->pos.y, this->pos.z), false);
			}
	}

	if (EntityController::instance->thePlayer != nullptr) {
		this->front = glm::normalize(EntityController::instance->thePlayer->pos - this->pos);
		vec3 tmpn = glm::normalize(glm::cross(this->front, this->right));
		GLfloat tmp1 = (this->norm.x * tmpn.x + this->norm.y * tmpn.y + this->norm.z * tmpn.z);
		GLfloat tmp2 = -tmp1;
		tmp1 = glm::acos(tmp1);
		tmp2 = glm::acos(tmp2);
		if (tmp1 <= tmp2)
			this->norm = tmpn;
		else
			this->norm = -tmpn;
		
		this->right = glm::normalize(glm::cross(this->front, this->norm));
	}

	if (this->health == 0) {
		if (EntityController::instance->thePlayer != nullptr) {
			((Spaceship *)EntityController::instance->thePlayer)->score++;
		}

		ParticleController::instance->push(Particle(1, 24, this->pos, vec3(0.), 16.f, 0.9f, true));
		GLint ran = rand() % 150 + 25;
		for (GLint i = 0; i < ran; i++) {
			ParticleController::instance->push(Particle(2, 11, this->pos, Ray::randVec3(5.5f, 22.5f), Ray::randFloat(3.f, 12.f), Ray::randFloat(1.5f, 3.6f), false));
		}
		SoundManager::engine->play3D(SoundManager::explosion, irrklang::vec3df(this->pos.x, this->pos.y, this->pos.z), false);
	}

	return this->health > 0;
}

void Enemy::render(Shader shader) {
	mat4 model(vec4(-this->front, 0.), vec4(this->norm, 0.), vec4(this->right, 0.), vec4(this->pos, 1.));

	model = glm::scale(model, vec3(this->scale));

	GLint modelLoc = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	this->modell->renderAll(shader);
}

GLfloat Enemy::sd(vec3 p) {
	if (this->capacity > 0)
		return glm::length(p - this->pos) - this->scale - 2.f;
	return glm::length(p - this->pos) - this->scale / 2.f - 2.f;
}

void Enemy::hitBy(HitType hit, GLfloat amount, vec3 p) {
	if (this->capacity > 0)
		for (int i = 1; i <= 30; i++) {
			ParticleController::instance->push(Particle(4, 10, Ray::pointOnSphere(this->pos, p, Ray::randVec3((i % 10 + 1) * 10.f, (i % 10 + 1) * 30.f) / 60.f, this->BS), vec3(0.), 0.8f, 0.3f, true));
		}

	if (hit == WEAPON) {
		if (this->capacity < this->shield * amount) {
			amount -= this->capacity / this->shield;
			this->capacity = 0;
			this->health -= amount;
		}
		else {
			this->capacity -= amount * this->shield;
		}
	}
	if (hit == OBJECT) {
		if (this->capacity == 0) {
			this->health -= amount;
		}
		else {
			this->capacity -= 2;
		}
	}
	if (this->capacity < 0)
		this->capacity = 0;
	if (this->health < 0)
		this->health = 0;

	this->hitTimer = 0.2f;
}

void Enemy::regenHP(GLint a) {
	this->health += a;
	if (this->health > this->maxHealth)
		this->health = this->maxHealth;
}

void Enemy::regenSC(GLint a) {
	this->capacity += a;
	if (this->capacity > this->maxCapacity)
		this->capacity = this->maxCapacity;
}

GLfloat Enemy::convFun(vec3 l) {
	return sqrt(l.x * l.x + l.y * l.y + l.z * l.z);
}

vec3 Enemy::matrixExp(GLfloat t) {
	return vec3(1.);
}

void Enemy::setScale(GLfloat s) {
	this->scale = s;
	this->BS = s;
}

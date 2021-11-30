#include "Bullet.h"

Bullet::Bullet() {
}

Bullet::Bullet(BulletType t, GLfloat w, GLfloat l, GLfloat m, GLfloat s, vec3 p, vec3 dir, Entity * shooter, GLfloat damage) {
	this->type = t;
	this->width = w;
	this->len = l;
	this->maxDist = m;
	this->speed = s;
	this->color = vec3(1., 0., 0.);

	this->front = dir;
	this->pos = p;

	this->time = 0.f;
	this->dist = 0.f;

	this->shooter = shooter;

	this->damage = damage;
}

GLboolean Bullet::update(GLfloat delta) {
	if (this->type == LASER) {
		vec3 end = Ray::castRay(this->pos, this->front, this->maxDist, this->shooter);
		this->time += delta;
		for (int i = glm::length(end - this->pos) / this->width; i >= 0; i--) {
			ParticleController::instance->push(Particle(0, 10, this->pos + this->front * this->width * (GLfloat)i, vec3(0.), 1.f, 0.3f, true));
		}
		Entity * e = EntityController::instance->hitFromSDF(end);
		if (e != nullptr && e != this->shooter) {
			if (this->damage < -1.f)
				e->hitBy(WEAPON, 500.f, end);
			else
				e->hitBy(WEAPON, this->damage, end);
			ParticleController::instance->push(Particle(0, 10, end, vec3(0.), 15.f, 0.5f, true));
		}
		return false;
	}
	if (this->type == NORMAL) {
		for (int i = this->speed * delta / this->width; i >= 0; i--) {
			vec3 p = this->pos + this->front * this->width * (GLfloat)i;
			ParticleController::instance->push(Particle(0, 10, p, vec3(0.), 1.f, this->len, true));
			Entity * e = EntityController::instance->hitFromSDF(p);
			if (e != nullptr && e != this->shooter) {
				if (this->damage < -1.f)
					e->hitBy(WEAPON, 50.f, p);
				else
					e->hitBy(WEAPON, this->damage, p);
				ParticleController::instance->push(Particle(0, 10, p, vec3(0.), 7.f, 0.5f, true));
				return false;
			}
		}

		this->pos += this->front * this->speed * delta;
		this->dist += this->speed * delta;

		return this->dist < this->maxDist;
	}

	if (this->type == PLASMA) {

		for (int i = this->speed * delta / this->width; i >= 0; i--) {
			vec3 p = this->pos + this->front * this->width * (GLfloat)i;
			ParticleController::instance->push(Particle(4, 10, p, vec3(0.), 3.f, this->len, true));
			Entity * e = EntityController::instance->hitFromSDF(p);
			if (e != nullptr && e != this->shooter) {
				if (this->damage < -1.f)
					e->hitBy(WEAPON, 200.f, p);
				else
					e->hitBy(WEAPON, this->damage, p);
				ParticleController::instance->push(Particle(4, 10, p, vec3(0.), 15.f, 0.5f, true));
				return false;
			}
		}

		this->pos += this->front * this->speed * delta;
		this->dist += this->speed * delta;

		return this->dist < this->maxDist;
	}

	if (this->type == EASTER) {
		vec3 end = Ray::castRay(this->pos, this->front, this->maxDist, this->shooter);
		this->time += delta;
		BeamController::instance->push(new BeamBillboard(this->pos, end, 8.f, 0.09f, Models::easterTexture));
		Entity * e = EntityController::instance->hitFromSDF(end);
		if (e != nullptr && e != this->shooter) {
			if (this->damage < -1.f)
				e->hitBy(WEAPON, 70.f, end);
			else
				e->hitBy(WEAPON, this->damage, end);
			for (int i = 0; i < 6; i++) {
				vec3 vel = Ray::randVec3(10.f, 15.f);
				Particle p(5, 1, end, vel, 10.f, 1.f, true);
				p.rotation = glm::acos(glm::dot(vel, vec3(-1., 0., 0.) / glm::length(vel)));
				ParticleController::instance->push(p);
			}
		}
		return false;
	}
	return true;
}

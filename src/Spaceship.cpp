#ifndef SPACESHIP_H
#define SPACESHIP_H
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/compatibility.hpp>
#include <GLFW/glfw3.h>

#include "Models.h"
#include "Camera.cpp"
#include "Shader.h"
#include "BulletController.h"
#include "Entity.h"
#include "Ray.h"
#include "Input.h"

#include "SoundManager.h"

using glm::vec4;

enum ViewType {
	FIRST,
	THIRD,
	AWAY
};

class Spaceship:public Entity {

public:

	GLint health, capacity, maxHealth, maxCapacity, score;

	Spaceship(Camera * camera) {
		this->camera = camera;
		this->pos = vec3(0., 0., 0.);

		this->upNormal = this->camera->upNormal;
		this->front = this->camera->lookDir;
		this->right = glm::normalize(glm::cross(this->front, this->upNormal));

		this->health = 500;
		this->capacity = 700;
		this->maxCapacity = 700;
		this->maxHealth = 500;

		this->BS = 10.f;
		this->score = 0;
	}

	GLboolean update(GLfloat delta, GLfloat t2) {

		if (this->shoot && this->cd[this->weapon] == 0.f) {
			this->cd[this->weapon] = this->shootCooldown();
			BulletController::instance->push(createBullet());
			if (this->weapon == 0)
				SoundManager::engine->play2D(SoundManager::pew1, false);
			if (this->weapon == 1)
				SoundManager::engine->play2D(SoundManager::pew2, false);
			if (this->weapon == 2)
				SoundManager::engine->play2D(SoundManager::pew3, false);
		}

		for (GLint i = 0; i < 4; i++) {
			this->cd[i] -= delta;
			if (this->cd[i] < 0.)
				this->cd[i] = 0.f;
		}

		this->velocity = glm::lerp(this->velocity, vec3(0.), delta * (this->accelSpeed / this->movementSpeed));

		if (glm::length(this->velocity) < 0.0001) {
			this->velocity *= 0.;
		}

		this->pos += this->velocity * delta + this->acceleration / 2.f * (t2 * t2 - (delta - t2) * (delta - t2));

		this->velocity += this->acceleration;
		this->acceleration *= 0.;

		if (this->FP) {
			this->camera->setPosition(this->pos + this->front * 1.5f + this->upNormal * 2.5f);
		}
		else {
			if (this->cameraFollowMode == 0)
				this->camera->setPosition(glm::lerp(this->camera->position, (this->pos - (this->front * 35.f) + this->upNormal * 10.f), min(delta * 10.f, 1.f)));
			if (this->cameraFollowMode == 1)
				this->camera->setPosition(glm::lerp(this->camera->position, (vec3(0., 0., this->pos.z) - (this->front * 35.f) + this->upNormal * 10.f), min(delta * 10.f, 1.f)));
			if (this->cameraFollowMode == 2)
				this->camera->setPosition(glm::lerp(this->camera->position, (vec3(0., this->pos.y / 2.f, this->pos.z) - (this->front * 60.f)), min(delta * 10.f, 1.f)));
		}

		this->camera->setDirection(this->front, this->upNormal);

		//PARTICLES
		this->particleTimer -= delta;
		if (this->particleTimer < 0.f && glm::dot(this->velocity, this->front) > 10.f) {
			this->particleTimer = 0.05f;
			vec3 partp = this->camera->position + Ray::randFloat(-30.f, 30.f) * this->camera->upNormal + Ray::randFloat(-40.f, 40.f) * this->camera->right() + Ray::randFloat(30.f, 60.f) * this->camera->lookDir;
			ParticleController::instance->push(Particle(4, 10, partp, this->camera->lookDir * -100.f, Ray::randFloat(1.f, 5.f) / 4.f, 1.f, true));
		}

		this->regenTime -= delta;
		if (this->regenTime < 0.f) {
			this->regenTime = 2.f;
			this->capacity += 10;
			this->health += 5;

			if (this->capacity > this->maxCapacity)
				this->capacity = this->maxCapacity;
			if (this->health > this->maxHealth)
				this->health = this->maxHealth;
		}

		this->hitTimer -= delta;
		if (this->hitTimer < 0.f)
			this->hitTimer = 0.f;


		SoundManager::engine->setListenerPosition(irrklang::vec3df(this->pos.x, this->pos.y, this->pos.z), irrklang::vec3df(this->front.x, this->front.y, this->front.z));

		return true;
	}

	void render(Shader shader) {

		mat4 model(vec4(-this->front, 0.), vec4(this->upNormal, 0.), vec4(this->right, 0.), vec4(this->pos, 1.));
		
		model = glm::scale(model, vec3(0.007));

		GLint modelLoc = glGetUniformLocation(shader.Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Models::player.renderAll(shader);
	}

	void handleMovement(Direction dir, GLfloat delta) {
		GLfloat vel = this->accelSpeed * delta;

		if (dir == FORTH) {
			this->acceleration += this->front * vel * movementModifier.z;
			
			this->engineTimer -= delta;
			if (this->engineTimer < 0.f) {
				this->engineTimer = 0.001f;
				ParticleController::instance->push(Particle(4, 10, this->pos - this->front * 10.5f + this->upNormal * 2.f + this->right * 3.2f + this->right * (Ray::randFloat(1.f, 6.f) * 0.5f - 2.f) * 0.6f, this->velocity * 0.5f, 1.f, 0.1f, true));
				ParticleController::instance->push(Particle(4, 10, this->pos - this->front * 10.f + this->upNormal * -1.5f + this->right * 3.2f + this->right * (Ray::randFloat(1.f, 6.f) * 0.5f - 2.f) * 0.6f, this->velocity * 0.5f, 1.f, 0.1f, true));
				ParticleController::instance->push(Particle(4, 10, this->pos - this->front * 10.5f + this->upNormal * 2.f + this->right * -3.2f + this->right * (Ray::randFloat(1.f, 6.f) * 0.5f - 2.f) * -0.6f, this->velocity * 0.5f, 1.f, 0.1f, true));
				ParticleController::instance->push(Particle(4, 10, this->pos - this->front * 10.f + this->upNormal * -1.5f + this->right * -3.2f + this->right * (Ray::randFloat(1.f, 6.f) * 0.5f - 2.f) * -0.6f, this->velocity * 0.5f, 1.f, 0.1f, true));
			}
		}
		if (dir == BACK) {
			this->acceleration += -this->front * vel * movementModifier.z;
		}
		if (dir == RIGHT) {
			this->acceleration += this->right * vel * movementModifier.x;
		}
		if (dir == LEFT) {
			this->acceleration += -this->right * vel * movementModifier.x;//glm::normalize(-glm::cross(this->front, this->upNormal)) * vel * movementModifier.x;
		}
		if (dir == UP) {
			this->acceleration += this->upNormal * vel * movementModifier.y;
		}
		if (dir == DOWN) {
			this->acceleration += -this->upNormal * vel * movementModifier.y;
		}
		if (dir == CLOCKW) {
			this->upNormal = glm::normalize(glm::rotate(this->upNormal, this->rotationSpeed * delta, this->front));
			this->right = this->right = glm::normalize(glm::cross(this->front, this->upNormal));
		}
		if (dir == COUNTERCLOCKW) {
			this->upNormal = glm::normalize(glm::rotate(this->upNormal, -this->rotationSpeed * delta, this->front));
			this->right = this->right = glm::normalize(glm::cross(this->front, this->upNormal));
		}
	}

	void handleMouse(vec2 dir, GLboolean RMB, GLfloat delta) {
		this->shoot = RMB;
		if (RMB) {
			this->shootDir = glm::normalize(Ray::castRay(this->camera, vec2(dir.x / Input::WIDTH * 2.f, dir.y / Input::HEIGHT * 2.f), this->shootRange(), this) - this->pos);
		}

		GLfloat len = glm::length(dir);
		len = sqrt(sqrt(len)) / sqrt(40) - 0.2;
		if (len < 0.1)
			return;
		dir = glm::normalize(dir) * len;

		this->upNormal = glm::normalize(glm::rotate(this->upNormal, dir.y * delta * this->rotationSpeed, this->right));
		this->front = glm::normalize(glm::rotate(this->front, dir.y * delta * this->rotationSpeed, this->right));
		this->front = glm::normalize(glm::rotate(this->front, -dir.x * delta * this->rotationSpeed, this->upNormal));
		this->right = glm::normalize(glm::cross(this->front, this->upNormal));
	}

	Bullet createBullet() {
		if (this->weapon == 0)
			return Bullet(NORMAL, 0.7f, 0.05f, shootRange(), 1000.f, this->pos + this->front * (this->BS * 0.9f), this->shootDir, this);
		if (this->weapon == 1)
			return Bullet(PLASMA, 1.f, 0.05f, shootRange(), 500.f, this->pos + this->front * (this->BS * 0.9f), this->shootDir, this);
		if (this->weapon == 2)
			return Bullet(LASER, 1.f, 0.05f, shootRange(), 1000.f, this->pos + this->front * (this->BS * 0.9f), this->shootDir, this);
		if (this->weapon == 3)
			return Bullet(EASTER, 0.7f, 0.05f, shootRange(), 1000.f, this->pos + this->front * (this->BS * 1.1f), this->shootDir, this);
		return Bullet(NORMAL, 0.f, 0.f, 0.f, 0.f, this->pos + this->front * (this->BS * 0.9f), this->shootDir, this);
	}

	GLfloat shootRange() {
		if (this->weapon == 0)
			return 450.f;
		if (this->weapon == 1)
			return 300.f;
		if (this->weapon == 2)
			return 500.f;
		if (this->weapon == 3)
			return 800.f;
		return 450.f;
	}

	GLfloat shootCooldown() {
		if (this->weapon == 0)
			return 0.15f;
		if (this->weapon == 1)
			return 1.f;
		if (this->weapon == 2)
			return 5.f;
		if (this->weapon == 3)
			return 0.15f;
		return 0.15f;
	}

	GLfloat shootCooldown(GLint w) {
		if (w == 0)
			return 0.15f;
		if (w == 1)
			return 1.f;
		if (w == 2)
			return 5.f;
		if (w == 3)
			return 0.15f;
		return 0.15f;
	}

	GLfloat getCooldown(GLint w) {
		return this->cd[w];
	}

	GLboolean isPlayer() {
		return true;
	}

	void swapWeapon(GLint w) {
		this->weapon = w;
	}

	GLint getWeapon() {
		return this->weapon;
	}

	void hitBy(HitType hit, GLfloat amount, vec3 p) {
		if (this->capacity > 0) {
			for (int i = 1; i <= 30; i++) {
				ParticleController::instance->push(Particle(4, 10, Ray::pointOnSphere(this->pos, p, Ray::randVec3((i % 10 + 1) * 10.f, (i % 10 + 1) * 30.f) / 120.f, this->BS), this->velocity, 0.8f, 0.3f, true));
			}
			SoundManager::engine->play2D(SoundManager::shield, false);
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
		if (hit == OBJECT && this->hitTimer == 0.f) {
			if (this->capacity < this->shield * amount) {
				amount -= this->capacity / this->shield;
				this->capacity = 0;
				this->health -= amount;
			}
			else {
				this->capacity -= amount * this->shield;
			}
		}
		if (this->capacity < 0)
			this->capacity = 0;
		if (this->health < 0)
			this->health = 0;

		this->hitTimer = 0.2f;
	}

	GLfloat sd(vec3 p) {
		if (this->capacity > 0)
			return glm::length(p - this->pos) - 12.f;
		return glm::length(p - this->pos) - 7.f;
	}

	void setFP(GLboolean a) {
		this->FP = a;
	}

	void collide(Entity * e) {
		if (this->cameraFollowMode == 0) {
			GLfloat l = glm::length(this->velocity);
			this->velocity *= 0.f;
			this->velocity += glm::normalize(this->pos - e->pos) * l + 1.f;
			this->hitBy(OBJECT, 0.f, this->pos);
			e->hitBy(OBJECT, 0., this->pos);
		}
		if (this->cameraFollowMode == 1 || this->cameraFollowMode == 2) {
			this->hitBy(WEAPON, 50.f, this->pos);
			e->hitBy(WEAPON, 500000.f, e->pos);
		}
	}

	void setCameraFollowMode(GLint m) {
		this->cameraFollowMode = m;
	}

	void setSpeedP(GLfloat maxS, GLfloat accel) {
		this->movementSpeed = maxS;
		this->accelSpeed = accel;
	}

private:
	Camera * camera;
	vec3 front;
	vec3 right;
	vec3 upNormal;

	vec3 scale;
	vec3 velocity;
	vec3 acceleration;
	
	GLfloat movementSpeed = 45.f;
	GLfloat accelSpeed = 15.f;
	const vec3 movementModifier = vec3(0.6, 0.5, 1.);

	const GLfloat rotationSpeed = 1.3f;

	vec3 shootDir;
	GLboolean shoot;
	GLint weapon = 0;
	GLfloat cd[4];

	GLfloat engineTimer = 0.1f;

	GLfloat shield = 0.4f;
	GLfloat regenTime = 2.f;
	GLfloat hitTimer = 0.f;
	GLfloat particleTimer = 0.f;

	GLboolean FP = false;

	GLint cameraFollowMode = 0;
};

#endif

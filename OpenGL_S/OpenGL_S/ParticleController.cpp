#include "ParticleController.h"

ParticleController::ParticleController() {
	this->particles.clear();
}

ParticleController::ParticleController(mat4 p, Camera * c) {
	this->camera = c;
	this->projection = p;
	this->particles.clear();
}

void ParticleController::push(Particle p) {
	this->particles.push_back(p);
}

void ParticleController::update(GLfloat delta) {
	std::vector<Particle> neww;
	for (int i = 0; i < this->particles.size() && i > -2; i++) {
		if (this->particles[i].update(delta)) {
			neww.push_back(this->particles[i]);
		}
	}
	this->particles = neww;
}

void ParticleController::render(ParticleRenderer * r) {
	r->render(this->projection, this->camera, &this->particles);
}

void ParticleController::setCamera(Camera * c) {
	this->camera = c;
}

void ParticleController::setProjection(mat4 p) {
	this->projection = p;
}

ParticleController * ParticleController::instance = &ParticleController();
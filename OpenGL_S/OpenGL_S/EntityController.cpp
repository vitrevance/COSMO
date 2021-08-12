#include "EntityController.h"

EntityController * EntityController::instance;

EntityController::EntityController() {
	this->entities.clear();
	this->push_queue.clear();
	this->thePlayer = nullptr;
}

GLfloat EntityController::sdf(vec3 p) {
	GLfloat ret = 1000.f;
	for (Entity * e : this->entities) {
		ret = glm::min(ret, e->sd(p));
	}
	return ret;
}

GLfloat EntityController::sdf(vec3 p, Entity * ig) {
	GLfloat ret = 10000.f;
	for (Entity * e : this->entities) {
		if (e != ig)
			ret = glm::min(ret, e->sd(p));
	}
	return ret;
}

std::vector<Entity *> EntityController::collisionSdf(Entity * e) {
	std::vector<Entity *> ret;
	for (Entity * ce : this->entities) {
		if (ce != e && ce->sd(e->pos) < e->BS)
			ret.push_back(ce);
			
	}
	return ret;
}

void EntityController::push(Entity * e) {
	this->push_queue.push_back(e);
	if (e->isPlayer())
		this->thePlayer = e;
}

void EntityController::update(GLfloat delta, GLfloat t2) {
	if (!this->push_queue.empty()) {
		this->entities.insert(this->entities.end(), this->push_queue.begin(), this->push_queue.end());
		this->push_queue.clear();
	}
	std::vector<Entity *> neww;
	for (GLint i = 0; i < this->entities.size(); i++) {
		Entity * e = this->entities[i];
		if (e->update(delta, t2) && glm::length(e->pos - this->thePlayer->pos) < 2000.f) {
			neww.push_back(e);
			for (GLint j = 0; j < this->entities.size(); j++) {
				Entity * ce = this->entities[j];
				if (ce != nullptr) {
					if (ce != e && ce->sd(e->pos) < e->BS)
						e->collide(ce);
				}
			}
		}
		else {
			delete e;
			this->entities[i] = nullptr;
		}
	}
	this->entities = neww;
}

void EntityController::render(Shader shader) {
	for (Entity * e : this->entities) {
		if (e != this->thePlayer)
			e->render(shader);
	}
	this->thePlayer->render(shader);
}

Entity * EntityController::hitFromSDF(vec3 p) {
	for (Entity * e : this->entities) {
		if (e->sd(p) <= Ray::eps)
			return e;
	}
	return nullptr;
}

#ifndef ENTIYCONTROLLER_H
#define ENTIYCONTROLLER_H

#include "Entity.h"
#include <vector>
#include "Ray.h"

class EntityController {

public:

	static EntityController * instance;
	Entity * thePlayer;

	std::vector<Entity *> entities;
	std::vector<Entity *> push_queue;

	EntityController();

	GLfloat sdf(vec3 p);

	GLfloat sdf(vec3 p, Entity * ig);

	std::vector<Entity *> collisionSdf(Entity * e);

	void push(Entity * e);

	void update(GLfloat delta, GLfloat t2);

	void render(Shader shader);

	Entity * hitFromSDF(vec3 p);
};

#endif
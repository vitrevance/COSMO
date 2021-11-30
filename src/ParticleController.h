#ifndef PARTICLECONTROLLER_H
#define PARTICLECONTROLLER_H

#include "ParticleRenderer.h"

class ParticleController {

public:

	static ParticleController * instance;
	std::vector<Particle> particles;
	Camera * camera;
	mat4 projection;

	ParticleController();

	ParticleController(mat4 p, Camera * c);

	void push(Particle p);

	void update(GLfloat delta);

	void render(ParticleRenderer * r);

	void setCamera(Camera * c);
	void setProjection(mat4 p);
};

#endif 
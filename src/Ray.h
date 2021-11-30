#ifndef RAY_H
#define RAY_H

#include "EntityController.h"
#include <cstdlib>
#include <ctime>

using glm::vec2;

class Ray {

public:

	static GLfloat eps;

	static vec3 castRay(vec3 ro, vec3 rd, GLfloat maxL, Entity * ig = nullptr);

	static vec3 castRay(Camera * camera, vec2 mouse, GLfloat maxL, Entity * ig = nullptr);

	static vec3 randVec3(GLfloat mn, GLfloat mx);

	static GLfloat randFloat(GLfloat mn, GLfloat mx);

	static vec3 pointOnSphere(vec3 pos, vec3 p, vec3 m, GLfloat r);

};

#endif
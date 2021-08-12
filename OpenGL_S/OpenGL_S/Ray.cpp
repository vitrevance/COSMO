#include "Ray.h"

GLfloat Ray::eps = 0.01;

vec3 Ray::castRay(vec3 ro, vec3 rd, GLfloat maxL, Entity * ig) {
	GLfloat t = 0.f;
	GLfloat d = 0.;
	for (int i = 0; i < 200 && t < maxL; i++) {
		d = EntityController::instance->sdf(ro + rd * t, ig);
		if (d < Ray::eps) {
			return ro + rd * (t);
		}
		t += d;
	}
	return ro + rd * t;
}

vec3 Ray::castRay(Camera * camera, vec2 mouse, GLfloat maxL, Entity * ig) {
	vec3 right = glm::cross(camera->lookDir, camera->upNormal);

	GLfloat mouseX = mouse.x / 640.f;
	GLfloat mouseY = mouse.y / 360.f;

	mat4 view = glm::lookAt(vec3(0.), camera->lookDir, camera->upNormal);
	mat4 inv = glm::inverse(camera->projection * view);
	vec4 screenPos = vec4(mouse.x, mouse.y, 1.0f, 1.0f);
	vec4 worldPos = inv * screenPos;
	vec3 rd = glm::normalize(vec3(worldPos));

	vec3 ro = camera->position;
	return Ray::castRay(ro, rd, maxL, ig);
}

vec3 Ray::randVec3(GLfloat mn, GLfloat mx) {
	vec3 randVec;

	randVec.x = (rand() % (GLint)glm::floor(mx - mn) + rand() % 10 / 10.f + rand() % 10 / 100.f + mn - 1) * pow((-1), (rand() % 2));
	randVec.y = (rand() % (GLint)glm::floor(mx - mn) + rand() % 10 / 10.f + rand() % 10 / 100.f + mn - 1) * pow((-1), (rand() % 2));
	randVec.z = (rand() % (GLint)glm::floor(mx - mn) + rand() % 10 / 10.f + rand() % 10 / 100.f + mn - 1) * pow((-1), (rand() % 2));

	return randVec;
}

GLfloat Ray::randFloat(GLfloat mn, GLfloat mx) {
	return rand() % ((GLint)glm::floor(mx - mn) + 1) + rand() % 10 / 10.f + rand() % 10 / 100.f + mn - 2.f;
}

vec3 Ray::pointOnSphere(vec3 pos, vec3 p, vec3 m, GLfloat r) {
	return glm::normalize(p + m - pos) * r + pos;
}

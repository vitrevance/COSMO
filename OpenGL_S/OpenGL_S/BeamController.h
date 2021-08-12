#ifndef BEAMCONTROLLER_H
#define BEAMCONTROLLER_H

#include "BeamBillboard.h"
#include <vector>

class BeamController {

public:

	static BeamController * instance;

	std::vector<BeamBillboard *> beams;

	BeamController();

	void update(GLfloat delta);

	void render(Shader & shader, Camera * camera);

	void push(BeamBillboard * b);
};

#endif
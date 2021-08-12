#include "BeamController.h"

BeamController * BeamController::instance = new BeamController();

BeamController::BeamController() {
	this->beams.clear();
}

void BeamController::update(GLfloat delta) {
	if (!this->beams.empty()) {
		std::vector<BeamBillboard *> neww;
		for (BeamBillboard * b : this->beams) {
			if (b->update(delta)) {
				neww.push_back(b);
			}
			else {
				delete(b);
			}
		}
		this->beams = neww;
	}
}

void BeamController::render(Shader & shader, Camera * camera) {
	for (BeamBillboard * b : this->beams) {
		b->render(shader, camera);
	}
}

void BeamController::push(BeamBillboard * b) {
	this->beams.push_back(b);
}

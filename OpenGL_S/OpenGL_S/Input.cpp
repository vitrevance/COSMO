#include "Input.h"

bool Input::cursorMode;
bool Input::firstMouse;

bool Input::keys[1024];
bool Input::mouseButtons[5];
glm::vec2 Input::prevMousePos;
glm::vec2 Input::mousePos;
glm::vec2 Input::mousePosNormalized;

GLint Input::WIDTH;
GLint Input::HEIGHT;

void Input::KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode) {
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			keys[key] = true;
		}
		if (action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}
}

void Input::MousePositionCallback(GLFWwindow * window, double x, double y) {
	glm::vec2 off(x - prevMousePos.x, -y - prevMousePos.y);
	prevMousePos = glm::vec2(x, -y);

	mousePos.x = glm::min(glm::max(mousePos.x + off.x, -WIDTH / 2.f), WIDTH / 2.f);
	mousePos.y = glm::min(glm::max(mousePos.y + off.y, -HEIGHT / 2.f), HEIGHT / 2.f);

	mousePosNormalized.x = (mousePos.x + (GLfloat)WIDTH / 2.f) / (GLfloat)WIDTH;
	mousePosNormalized.y = (-mousePos.y + (GLfloat)HEIGHT / 2.f) / (GLfloat)HEIGHT;
}

void Input::MouseInputCallback(GLFWwindow * window, GLint button, GLint action, GLint mod) {
	if (action == GLFW_PRESS) {
		mouseButtons[button] = true;
	}
	if (action == GLFW_RELEASE) {
		mouseButtons[button] = false;
	}
}

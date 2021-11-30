#ifndef INPUT_H
#define INPUT_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Input {

private:

	static glm::vec2 prevMousePos;

public:

	static bool cursorMode;
	static bool firstMouse;

	static bool keys[1024];
	static bool mouseButtons[5];
	static glm::vec2 mousePos;
	static glm::vec2 mousePosNormalized;

	static GLint WIDTH, HEIGHT;

	static void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode);

	static void MousePositionCallback(GLFWwindow * window, double x, double y);

	static void MouseInputCallback(GLFWwindow * window, GLint button, GLint action, GLint mod);
};

#endif
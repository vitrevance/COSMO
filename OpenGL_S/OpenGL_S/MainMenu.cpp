#include "MainMenu.h"

MainMenu::MainMenu() {
}

MainMenu::MainMenu(GLFWwindow * w) {
	this->window = w;
	glfwGetWindowSize(this->window, &this->WIDTH, &this->HEIGHT);

	this->hudShader = Models::hudShader;
	this->start = Button(vec2(0.4, 0.8), vec2(0.2, 0.1), START);
	this->quit = Button(vec2(0.02, 0.88), vec2(0.2, 0.1), QUIT);
	this->prev = Button(vec2(0.28, 0.525), vec2(0.06, 0.15), PREV);
	this->next = Button(vec2(0.66, 0.525), vec2(0.06, 0.15), NEXT);

	this->overlay = HUDElement("resources/textures/main_Menu.png", vec2(0.), vec2(1.));
	this->cursor = HUDElement("resources/textures/cursor_menu.png", vec2(0.), vec2(0.025, 0.04));
	this->bkground = Cubemap("resources/skybox/tyro/", ".png");
	this->bkground.Setup();

	this->level_pre = HUDElement("resources/textures/levels.png", vec2(0.35, 0.45), vec2(0.3, 0.3), vec4(1, 3, 0, 0));

	this->projection = this->projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 1000.0f);
	this->view = glm::lookAt(vec3(0.), vec3(0., 0., -0.1), vec3(0., 1., 0.));
}

void MainMenu::render() {
	glfwPollEvents();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->view = glm::lookAt(vec3(0.), vec3(Input::mousePos.x, Input::mousePos.y, 10000.f), vec3(0., 1., 0.));
	this->bkground.Use(this->view, this->projection);

	this->overlay.render(this->hudShader);

	if (GameManager::level_type == 3)
		GameManager::level_type = 0;
	this->level_pre.offset2(vec2(0., GameManager::level_type / 3.f));
	this->level_pre.render(this->hudShader);

	if (this->start.update(this->hudShader)) {
		if (Input::mouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
			GameManager::StartGame();
			Input::mouseButtons[GLFW_MOUSE_BUTTON_LEFT] = false;
		}
	}
	if (this->quit.update(this->hudShader)) {
		if (Input::mouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
			GameManager::ExitGame();
			Input::mouseButtons[GLFW_MOUSE_BUTTON_LEFT] = false;
		}
	}

	if (this->prev.update(this->hudShader)) {
		if (Input::mouseButtons[GLFW_MOUSE_BUTTON_LEFT] && GameManager::level_type > 0) {
			GameManager::level_type--;
			Input::mouseButtons[GLFW_MOUSE_BUTTON_LEFT] = false;
		}
	}
	if (this->next.update(this->hudShader)) {
		if (Input::mouseButtons[GLFW_MOUSE_BUTTON_LEFT] && GameManager::level_type < 2) {
			GameManager::level_type++;
			Input::mouseButtons[GLFW_MOUSE_BUTTON_LEFT] = false;
		}
	}

	if (Input::keys[GLFW_KEY_LEFT] && GameManager::level_type > 0) {
		GameManager::level_type--;
		Input::keys[GLFW_KEY_LEFT] = false;
	}
	if (Input::keys[GLFW_KEY_RIGHT] && GameManager::level_type < 2) {
		GameManager::level_type++;
		Input::keys[GLFW_KEY_RIGHT] = false;
	}

	this->cursor.translate(Input::mousePosNormalized);
	this->cursor.render(this->hudShader);

	glfwSwapBuffers(this->window);
}

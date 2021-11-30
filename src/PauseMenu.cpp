#include "PauseMenu.h"

PauseMenu::PauseMenu() {
}

PauseMenu::PauseMenu(GLFWwindow * w) {
	this->window = w;
	glfwGetWindowSize(this->window, &this->WIDTH, &this->HEIGHT);

	this->hudShader = Models::hudShader;
	this->start = Button(vec2(0.35, 0.6), vec2(0.3, 0.1), RESUME);
	this->quit = Button(vec2(0.4, 0.8), vec2(0.2, 0.1), MENU);

	this->bkground = Cubemap("resources/skybox/tyro/", ".png");
	this->bkground.Setup();
	this->overlay = HUDElement("resources/textures/main_Pause.png", vec2(0.), vec2(1.));
	this->cursor = HUDElement("resources/textures/cursor_menu.png", vec2(0.), vec2(0.025, 0.04));

	this->projection = this->projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 1000.0f);
	this->view = glm::lookAt(vec3(0.), vec3(0., 0., -0.1), vec3(0., 1., 0.));

	this->deathScreen = HUDElement("resources/textures/you_died.png", vec2(0.), vec2(1.));

	this->player_score = NumericHUD(vec2(0.5, 0.5), vec2(0.05, 0.08), &GameManager::player_score);
}

void PauseMenu::render() {
	glfwPollEvents();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (((Spaceship *)EntityController::instance->thePlayer)->health == 0) {
		this->deathScreen.render(this->hudShader);
		this->player_score.render(this->hudShader);
	}
	else {
		this->view = glm::lookAt(vec3(0.), vec3(Input::mousePos.x, Input::mousePos.y, -10000.f), vec3(0., 1., 0.));
		this->bkground.Use(this->view, this->projection);

		this->overlay.render(this->hudShader);

		if (this->start.update(this->hudShader)) {
			if (Input::mouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
				GameManager::PauseGame();
				Input::mouseButtons[GLFW_MOUSE_BUTTON_LEFT] = false;
			}
		}
	}
	if (this->quit.update(this->hudShader)) {
		if (Input::mouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
			GameManager::OpenMainMenu();
			Input::mouseButtons[GLFW_MOUSE_BUTTON_LEFT] = false;
		}
	}

	this->cursor.translate(Input::mousePosNormalized);
	this->cursor.render(this->hudShader);

	glfwSwapBuffers(window);
}

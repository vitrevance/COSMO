#include "LevelFreeroom.h"

LevelFreeroom::LevelFreeroom(GLFWwindow * window) {
	this->window = window;
}

void LevelFreeroom::setup() {
	glfwGetWindowSize(this->window, &this->WIDTH, &this->HEIGHT);

	this->mainCamera = Camera(vec3(0., 0., -3.), vec3(0., 0., 1.));

	this->ship = new Spaceship(&this->mainCamera);

	this->shader = Models::modelShader;
	this->hudShader = Models::hudShader;

	this->sky = Cubemap("resources/skybox/se_1/", ".png");
	sky.Setup();
	this->hud = MainGUI(ship);
	this->cursor = HUDElement("resources/textures/cursor.png", vec2(0.), vec2(0.025, 0.04));

	this->particleRenderer.setup();

	mainCamera.processMouse(0., 0.);

	this->projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 1000.0f);
	this->mainCamera.projection = this->projection;

	if (ParticleController::instance != nullptr) {
                try {
                        delete ParticleController::instance;
                }
                catch(...) {
                        std::cout << "ERROR: Delete ParticleController:instance in LevelDefault.cpp"<< std::endl;
                }
        }

        ParticleController::instance = new ParticleController(projection, &mainCamera);
        if (BulletController::instance != nullptr) {
                try {
                        delete BulletController::instance;
                }
                catch(...) {
                        std::cout << "ERROR: Delete BulletController:instance in LevelDefault.cpp"<< std::endl;
                }
        }

        BulletController::instance = new BulletController();
        if (EntityController::instance != nullptr) {
                try {
                        delete EntityController::instance;
                }
                catch(...) {
                        std::cout << "ERROR: Delete EntityController:instance in LevelDefault.cpp"<< std::endl;
                }
        }

        EntityController::instance = new EntityController();
	
	EntityController::instance->push(this->ship);

	EntityController::instance->push(new Decor(vec3(0., 50., 250.), vec3(1., 0., 0.), vec3(0., 1., 0.), 50.f, vec3(300, 200, 100), &Models::player));
}

void LevelFreeroom::mainCycle() {

	GLfloat currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	glfwPollEvents();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->handleShipMovement();

	BeamController::instance->update(deltaTime);
	BulletController::instance->update(deltaTime);
	ParticleController::instance->update(deltaTime);
	EntityController::instance->update(deltaTime, lastTime);

	mat4 view;

	//Skybox

	glBindVertexArray(0);

	view = glm::mat4(glm::mat3(mainCamera.look()));

	sky.Use(view, projection);

	//Entities
	shader.Use();

	view = mainCamera.look();

	GLint viewLoc = glGetUniformLocation(shader.Program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	GLint projLoc = glGetUniformLocation(shader.Program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


	glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), mainCamera.position.x, mainCamera.position.y, mainCamera.position.z);
	glUniform3f(glGetUniformLocation(shader.Program, "light.pos"), 0.8, -0.5, 1.);
	glUniform3f(glGetUniformLocation(shader.Program, "light.ambient"), 0.2, 0.2, 0.2);
	glUniform3f(glGetUniformLocation(shader.Program, "light.diffuse"), 1., 1., 1.);
	glUniform3f(glGetUniformLocation(shader.Program, "light.specular"), 1., 1., 1.);

	EntityController::instance->render(shader);

	//Beams
	Models::beamShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(Models::beamShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(Models::beamShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	BeamController::instance->render(Models::beamShader, &this->mainCamera);

	//Particles
	ParticleController::instance->render(&particleRenderer);
	

	//HUD
	hud.render(hudShader);
	cursor.translate(Input::mousePosNormalized - vec2(0.0125, 0.02));
	cursor.render(hudShader);

	glfwSwapBuffers(window);
}

void LevelFreeroom::handleShipMovement() {
	if (Input::keys[GLFW_KEY_1]) {
		this->ship->swapWeapon(0);
	}
	if (Input::keys[GLFW_KEY_2]) {
		this->ship->swapWeapon(1);
	}
	if (Input::keys[GLFW_KEY_3]) {
		this->ship->swapWeapon(2);
	}
	if (Input::keys[GLFW_KEY_F2]) {
		ship->setFP(true);
	}
	if (Input::keys[GLFW_KEY_F3]) {
		ship->setFP(false);
	}
	this->ship->handleMouse(Input::mousePos, Input::mouseButtons[GLFW_MOUSE_BUTTON_RIGHT], this->deltaTime);
	if (Input::keys[GLFW_KEY_W]) {
		ship->handleMovement(FORTH, deltaTime);
	}
	if (Input::keys[GLFW_KEY_S]) {
		ship->handleMovement(BACK, deltaTime);
	}
	if (Input::keys[GLFW_KEY_A]) {
		ship->handleMovement(LEFT, deltaTime);
	}
	if (Input::keys[GLFW_KEY_D]) {
		ship->handleMovement(RIGHT, deltaTime);
	}
	if (Input::keys[GLFW_KEY_SPACE]) {
		ship->handleMovement(UP, deltaTime);
	}
	if (Input::keys[GLFW_KEY_LEFT_ALT]) {
		ship->handleMovement(DOWN, deltaTime);
	}
	if (Input::keys[GLFW_KEY_E]) {
		ship->handleMovement(CLOCKW, deltaTime);
	}
	if (Input::keys[GLFW_KEY_Q]) {
		ship->handleMovement(COUNTERCLOCKW, deltaTime);
	}
}

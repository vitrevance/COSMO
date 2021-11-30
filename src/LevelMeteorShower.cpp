#include "LevelMeteorShower.h"

LevelMeteorShower::LevelMeteorShower(GLFWwindow * window) {
	this->window = window;
}

void LevelMeteorShower::setup() {
	glfwGetWindowSize(this->window, &this->WIDTH, &this->HEIGHT);

	this->mainCamera = Camera(vec3(0., 0., -3.), vec3(0., 0., 1.));

	this->ship = new Spaceship(&this->mainCamera);
	this->ship->setCameraFollowMode(2);

	this->shader = Models::modelShader;
	this->hudShader = Models::hudShader;

	this->sky = Cubemap("resources/skybox/tyro1/", ".png");
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

	this->spawnTimer = 0.f;
}

void LevelMeteorShower::mainCycle() {

	GLfloat currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	glfwPollEvents();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	this->handleShipMovement();
	GLint randi = rand();
	this->spawnTimer += deltaTime;
	if (this->spawnTimer > 0.5f) {
		this->spawnTimer = 0.f;
		GLint tlocx = randi % 9 - 4;
		GLint tlocy = randi % 4 - 1;
		vec3 reqpos = vec3(tlocx * -20.f, tlocy * -20.f, ship->pos.z + Ray::randFloat(400.f, 1000.f));
		Asteroid * asteroid = new Asteroid(reqpos, Ray::randFloat(5.f, 20.f), rand() % 5, 70);
		asteroid->setHP(1);
		asteroid->setSlowingFactor(0.1f);
		asteroid->applyVelocity(vec3(0., 0., -500.f));
		EntityController::instance->push(asteroid);

	}

	BulletController::instance->update(deltaTime);
	ParticleController::instance->update(deltaTime);
	EntityController::instance->update(deltaTime, lastTime);

	ship->score = (GLint)(ship->pos.z / 300.f);

	mat4 view;

	//Skybox

	glBindVertexArray(0);

	view = glm::mat4(glm::mat3(mainCamera.look()));

	sky.Use(view, projection);


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

	//Particles
	ParticleController::instance->render(&particleRenderer);

	//HUD
	hud.render(hudShader);
	cursor.translate(Input::mousePosNormalized - vec2(0.0125, 0.02));
	cursor.render(hudShader);

	glfwSwapBuffers(window);
}

void LevelMeteorShower::handleShipMovement() {
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

	if (Input::keys[GLFW_KEY_A] && this->shipScreenLoc % 5 != 0) {
		Input::keys[GLFW_KEY_A] = false;
		this->shipScreenLoc--;
	}
	if (Input::keys[GLFW_KEY_D] && this->shipScreenLoc % 5 != 4) {
		Input::keys[GLFW_KEY_D] = false;
		this->shipScreenLoc++;
	}
	if (Input::keys[GLFW_KEY_W] && this->shipScreenLoc >= 5) {
		Input::keys[GLFW_KEY_W] = false;
		this->shipScreenLoc -= 5;
	}
	if (Input::keys[GLFW_KEY_S] && this->shipScreenLoc <= 14) {
		Input::keys[GLFW_KEY_S] = false;
		this->shipScreenLoc += 5;
	}
	this->ship->handleMouse(Input::mousePos, Input::mouseButtons[GLFW_MOUSE_BUTTON_RIGHT], 0.f);
	this->ship->handleMovement(FORTH, this->deltaTime);
	
	GLint tlocx = this->shipScreenLoc % 5 - 2;
	GLint tlocy = this->shipScreenLoc / 5 - 1;
	vec3 reqposoff = vec3(tlocx * -20.f, tlocy * -20.f, ship->pos.z);

	this->ship->pos = glm::lerp(ship->pos, reqposoff, this->deltaTime * 5.f);
}

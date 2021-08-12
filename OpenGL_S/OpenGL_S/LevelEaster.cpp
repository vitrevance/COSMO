#include "LevelEaster.h"

LevelEaster::LevelEaster(GLFWwindow * window) {
	this->window = window;
}

void LevelEaster::setup() {
	glfwGetWindowSize(this->window, &this->WIDTH, &this->HEIGHT);

	this->mainCamera = Camera(vec3(0., 0., -3.), vec3(0., 0., 1.));

	this->ship = new Spaceship(&this->mainCamera);
	this->ship->setCameraFollowMode(1);

	this->shader = Models::modelShader;
	this->hudShader = Models::hudShader;

	this->sky = Cubemap("resources/skybox/easter/", ".png");
	sky.Setup();
	this->hud = MainGUI(ship);
	this->cursor = HUDElement("resources/textures/cursor.png", vec2(0.), vec2(0.025, 0.04));
	this->intro = HUDElement("resources/textures/intro.png", vec2(0.), vec2(2., 1.));
	this->animation = -1.f;

	this->particleRenderer.setup();

	mainCamera.processMouse(0., 0.);

	this->projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 1000.0f);
	this->mainCamera.projection = this->projection;

	ParticleController::instance = new ParticleController(projection, &mainCamera);
	BulletController::instance = new BulletController();
	EntityController::instance = new EntityController();

	EntityController::instance->push(this->ship);
	this->ship->swapWeapon(3);
	this->ship->setSpeedP(60.f, 20.f);

	SoundManager::engine->play2D(SoundManager::easterAmbient, true);
}

void LevelEaster::mainCycle() {

	GLfloat currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	glfwPollEvents();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->handleShipMovement();
	GLint randi = rand();
	if (randi < 300) {
		if (randi % 4 == 0) {
			Enemy * enemy = new Enemy(40, 50, 0.8f, 30.f, &Models::enemy);
			enemy->pos = ship->pos + vec3(Ray::randFloat(-100.f, 100.f), Ray::randFloat(-100.f, 50.f), Ray::randFloat(500.f, 1000.f));
			EntityController::instance->push(enemy);
		}
		if (randi % 4 == 1) {
			Enemy * enemy = new Enemy(40, 50, 0.8f, 20.f, &Models::enemy1);
			enemy->pos = ship->pos + vec3(Ray::randFloat(-100.f, 100.f), Ray::randFloat(-100.f, 50.f), Ray::randFloat(500.f, 1000.f));
			EntityController::instance->push(enemy);
		}
		if (randi % 4 == 2) {
			Asteroid * asteroid = new Asteroid(ship->pos + vec3(Ray::randFloat(-100.f, 100.f), Ray::randFloat(-100.f, 50.f), Ray::randFloat(500.f, 1000.f)), Ray::randFloat(5.f, 20.f));
			EntityController::instance->push(asteroid);
		}
		if (randi % 4 == 3) {
			Enemy * enemy = new Enemy(40, 100, 0.8f, 15.f, &Models::enemy2);
			enemy->pos = ship->pos + vec3(Ray::randFloat(-100.f, 100.f), Ray::randFloat(-100.f, 50.f), Ray::randFloat(500.f, 1000.f));
			EntityController::instance->push(enemy);
		}

	}

	BeamController::instance->update(deltaTime);
	BulletController::instance->update(deltaTime);
	ParticleController::instance->update(deltaTime);
	EntityController::instance->update(deltaTime, lastTime);

	mat4 view;

	//Skybox

	glBindVertexArray(0);

	view = glm::mat4(glm::mat3(mainCamera.look()));

	sky.Use(view, projection);

	view = mainCamera.look();

	//Beams
	Models::beamShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(Models::beamShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(Models::beamShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	BeamController::instance->render(Models::beamShader, &this->mainCamera);

	//Models
	shader.Use();
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

	if (this->animation < 1.f) {
		this->animation += deltaTime;
		this->intro.translate(vec2(this->animation, 0.));
		this->intro.render(this->hudShader);
	}

	glfwSwapBuffers(window);
}

void LevelEaster::handleShipMovement() {
	if (Input::keys[GLFW_KEY_F2]) {
		ship->setFP(true);
	}
	if (Input::keys[GLFW_KEY_F3]) {
		ship->setFP(false);
	}

	if (Input::keys[GLFW_KEY_A] && this->shipScreenLoc != 0 && this->shipScreenLoc != 3) {
		Input::keys[GLFW_KEY_A] = false;
		this->shipScreenLoc--;
	}
	if (Input::keys[GLFW_KEY_D] && this->shipScreenLoc != 2 && this->shipScreenLoc != 5) {
		Input::keys[GLFW_KEY_D] = false;
		this->shipScreenLoc++;
	}
	if (Input::keys[GLFW_KEY_W] && this->shipScreenLoc >= 3 && this->shipScreenLoc <= 5) {
		this->shipScreenLoc -= 3;
	}
	if (Input::keys[GLFW_KEY_S] && this->shipScreenLoc >= 0 && this->shipScreenLoc <= 2) {
		this->shipScreenLoc += 3;
	}
	this->ship->handleMouse(Input::mousePos, Input::mouseButtons[GLFW_MOUSE_BUTTON_RIGHT], 0.f);
	this->ship->handleMovement(FORTH, this->deltaTime);

	vec3 reqposoff = vec3(0., 0., ship->pos.z);
	if (this->shipScreenLoc == 0)
		reqposoff += vec3(20.f, 20.f, 0.);
	if (this->shipScreenLoc == 1)
		reqposoff += vec3(0.f, 20.f, 0.);
	if (this->shipScreenLoc == 2)
		reqposoff += vec3(-20.f, 20.f, 0.);
	if (this->shipScreenLoc == 3)
		reqposoff += vec3(20.f, 0.f, 0.);
	if (this->shipScreenLoc == 5)
		reqposoff += vec3(-20.f, 0.f, 0.);
	this->ship->pos = glm::lerp(ship->pos, reqposoff, this->deltaTime * 5.f);
}
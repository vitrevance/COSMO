#include <iostream>

#include <chrono>
#include <thread>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "MainMenu.h"
#include "PauseMenu.h"
#include "GameManager.h"

#include <FreeImage.h>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;


// Window dimensions
const GLuint WIDTH = 1280, HEIGHT = 720;

int setupWindow(GLFWwindow * window) {
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	Input::WIDTH = WIDTH;
	Input::HEIGHT = HEIGHT;

	glfwSetKeyCallback(window, Input::KeyCallback);
	glfwSetCursorPosCallback(window, Input::MousePositionCallback);
	glfwSetMouseButtonCallback(window, Input::MouseInputCallback);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return 0;
}

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "COSMO!", nullptr, nullptr);
	//GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "COSMO!", glfwGetPrimaryMonitor(), nullptr);

	int windowState = setupWindow(window);
	if (windowState != 0)
		return windowState;

	Models::load();

	GameManager::window = window;
	GameManager::setup();
	GameManager::main_menu = new MainMenu(window);
	GameManager::pause_menu = new PauseMenu(window);

	irrklang::ISoundEngine * engine = irrklang::createIrrKlangDevice();
	if (!engine)
		return 1;
	SoundManager::engine = engine;

	SoundManager::engine->setDefault3DSoundMinDistance(50.f);
	SoundManager::pew1 = SoundManager::engine->addSoundSourceFromFile("resources/sounds/pew1.mp3");
	SoundManager::pew2 = SoundManager::engine->addSoundSourceFromFile("resources/sounds/pew2.mp3");
	SoundManager::pew3 = SoundManager::engine->addSoundSourceFromFile("resources/sounds/pew3.mp3");
	SoundManager::explosion = SoundManager::engine->addSoundSourceFromFile("resources/sounds/exp.mp3");
	SoundManager::shield = SoundManager::engine->addSoundSourceFromFile("resources/sounds/shield.mp3");
	SoundManager::easterAmbient = SoundManager::engine->addSoundSourceFromFile("resources/sounds/nyan-cat.mp3");

	SoundManager::easterAmbient->setDefaultVolume(0.3);

	// Game loop
	while (!glfwWindowShouldClose(window) && !GameManager::exitGame)
	{
		if (Input::keys[GLFW_KEY_7]) {
			Input::keys[GLFW_KEY_7] = false;

			BYTE * pixels = new BYTE[3 * WIDTH * HEIGHT];

			glReadPixels(0, 0, WIDTH, HEIGHT, GL_BGR, GL_UNSIGNED_BYTE, pixels);
			FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, WIDTH, HEIGHT, 3 * WIDTH, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
			std::string file = "screenshots/shot_";
			int i = 1;
			for (i = 1; i < 1000; i++) {
				std::ifstream ifile(file + std::to_string(i) + ".png");
				if (!(bool)ifile)
					break;
			}
			FreeImage_Save(FIF_PNG, image, (file + std::to_string(i) + ".png").c_str(), 0);

			FreeImage_Unload(image);
			delete[] pixels;
		}
		GameManager::cycle();

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	engine->drop();
	glfwTerminate();

	return 0;
}

#include "GameManager.h"

GLFWwindow * GameManager::window;

GameLevel * GameManager::cur_level;
MenuScreen * GameManager::main_menu;
MenuScreen * GameManager::pause_menu;

GLint GameManager::level_type;

GLint GameManager::player_score = 0;

GLboolean GameManager::mainMenuActive;
GLboolean GameManager::exitGame;
GLboolean GameManager::onPause;

void GameManager::setup() {
	GameManager::mainMenuActive = true;
	GameManager::exitGame = false;
}

void GameManager::cycle() {
	if (GameManager::onPause) {
		GameManager::pause_menu->render();
	}
	else
	if (GameManager::mainMenuActive) {
		GameManager::main_menu->render();
	}
	else {
		if (((Spaceship *)EntityController::instance->thePlayer)->health == 0) {
			GameManager::PauseGame();
		}
		else {
			if ((GameManager::level_type == 3 || GameManager::level_type == 0) && Input::keys[GLFW_KEY_8]) {
				GameManager::OpenMainMenu();

				Input::keys[GLFW_KEY_8] = false;
				if (GameManager::level_type == 0)
					GameManager::level_type = 3;
				else if (GameManager::level_type == 3)
					GameManager::level_type = 0;

				GameManager::StartGame();
			}
			else {
				GameManager::cur_level->mainCycle();
				if (EntityController::instance != NULL) {
					if (EntityController::instance->thePlayer != nullptr) {
						GameManager::player_score = ((Spaceship *)EntityController::instance->thePlayer)->score;
					}
				}
			}
		}
	}


	if (Input::keys[GLFW_KEY_ESCAPE] && !GameManager::mainMenuActive) {
		GameManager::PauseGame();
		Input::keys[GLFW_KEY_ESCAPE] = false;
	}
}

void GameManager::StartGame() {
	if (GameManager::level_type == 0)
		GameManager::cur_level = new LevelDefault(GameManager::window);
	if (GameManager::level_type == 1)
		GameManager::cur_level = new LevelFreeroom(GameManager::window);
	if (GameManager::level_type == 2)
		GameManager::cur_level = new LevelMeteorShower(GameManager::window);
	if (GameManager::level_type == 3)
		GameManager::cur_level = new LevelEaster(GameManager::window);
	GameManager::cur_level->setup();
	GameManager::cur_level->lastTime = glfwGetTime();
	GameManager::mainMenuActive = false;
	GameManager::player_score = 0;
}

void GameManager::OpenMainMenu() {
	SoundManager::engine->stopAllSounds();
	delete(GameManager::cur_level);
	GameManager::mainMenuActive = true;
	GameManager::onPause = false;
}

void GameManager::ExitGame() {
	GameManager::exitGame = true;
}

void GameManager::PauseGame() {
	GameManager::onPause = !GameManager::onPause;
	if (!GameManager::onPause) {
		GameManager::cur_level->lastTime = glfwGetTime();
	}
	SoundManager::engine->setAllSoundsPaused(GameManager::onPause);

}

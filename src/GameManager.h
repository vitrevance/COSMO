#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "LevelDefault.h"
#include "LevelFreeroom.h"
#include "LevelMeteorShower.h"
#include "LevelEaster.h"
#include "MenuScreen.h"

class GameManager {

public:

	static GLFWwindow * window;

	static GameLevel * cur_level;
	static MenuScreen * main_menu;
	static MenuScreen * pause_menu;
	static GLint level_type;


	static GLboolean mainMenuActive;
	static GLboolean exitGame;
	static GLboolean onPause;
	static GLint player_score;

	static void setup();
	static void cycle();
	static void StartGame();
	static void OpenMainMenu();
	static void ExitGame();
	static void PauseGame();
};

#endif

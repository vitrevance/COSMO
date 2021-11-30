#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <irrKlang.h>

#pragma comment(lib, "irrKlang.lib")

class SoundManager {

public:

	static irrklang::ISoundEngine * engine;
	static irrklang::ISoundSource * pew1;
	static irrklang::ISoundSource * pew2;
	static irrklang::ISoundSource * pew3;
	static irrklang::ISoundSource * explosion;
	static irrklang::ISoundSource * shield;
	static irrklang::ISoundSource * easterAmbient;
};

#endif
#pragma once
#include <irrklang/irrKlang.h>
#include <iostream>
#include <string>

using namespace std;

class SoundManager
{
public:
	static SoundManager& getInstance()
	{
		static SoundManager instance;
		return instance;
	}

	irrklang::ISoundEngine* getSoundEngine()
	{
		return soundEngine;
	}

private:
	SoundManager()
	{
		soundEngine = irrklang::createIrrKlangDevice();
	}

	~SoundManager()
	{
		if (soundEngine)
		{
			soundEngine->drop();
		}
	}

	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;

	irrklang::ISoundEngine* soundEngine;
};


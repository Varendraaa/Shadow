#include "SoundEngine.h"

SoundEngine::SoundEngine()
{
	engine = irrklang::createIrrKlangDevice();
	if (!engine)
	{
		cout << "Error: Could not create sound engine" << endl;
	}
}

SoundEngine::~SoundEngine()
{
	engine->drop();
}

void SoundEngine::playBackgroundMusic(const string& musicFile, bool loop)
{
	engine->play2D(musicFile.c_str(), loop);
}

void SoundEngine::PlaySound(const string& soundFile, bool loop)
{
	engine->play2D(soundFile.c_str(), loop);
}
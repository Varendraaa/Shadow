#pragma once
#include <irrklang/irrKlang.h>
#include <iostream>
#include <string>

using namespace std;

class SoundEngine
{
public:
	SoundEngine();
	~SoundEngine();

	void playBackgroundMusic(const string& musicFile, bool loop = true);
	void PlaySound(const string& soundFile, bool loop = false);

private:
	irrklang::ISoundEngine* engine;
};


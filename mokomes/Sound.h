//#include "C:/Program Files/FMOD SoundSystem/FMOD Programmers API Windows/api/inc/fmod.hpp"
//#include "fmod.h"
#ifndef _SOUND_H
#define _SOUND_H
#include "fmod.hpp"

#pragma comment (lib, "fmodex_vc.lib")

#include <iostream>
#include <map>
#include <string>

using namespace std;

struct SoundInfo
{
	FMOD::Sound *sound;
};

class Sound
{
	private:
		FMOD_RESULT result;
		FMOD::System *system;
		FMOD::ChannelGroup *masterGroup;		
		map<string, SoundInfo> sounds;
	public:
		Sound();
		~Sound();
		void loadSound(char *kelias, char *pavadinimas);
		void loadStream(char *kelias, char *pavadinimas);
		void playSound(char *pavadinimas, bool loop = false, float garsumas = 1.0f);
		void pause();
		void play();
		void setVolume(float garsumas);

		void Sound::update()
		{
		 system->update();
		}
};

#endif
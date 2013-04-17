#include "Sound.h"

Sound::Sound()
{
	result = FMOD::System_Create(&system);		// Create the main system object.
	if (result != FMOD_OK)
	{
		cout << "Klaida" << endl;
	}
	
	result = system->init(100, FMOD_INIT_NORMAL, 0);	// Initialize FMOD.
	if (result != FMOD_OK)
	{
		cout << "Klaida" << endl;
	}
	system->createChannelGroup("masterGroup", &masterGroup);
}

Sound::~Sound()
{

	map <string,SoundInfo>::iterator itr;
	for(itr = sounds.begin();itr!=sounds.end();itr++)
	{
		itr->second.sound->release();
	}
	masterGroup->release();
	system->release();

}

void Sound::loadSound(char *kelias, char *pavadinimas)
{
	SoundInfo s;
	system->createSound(kelias, FMOD_DEFAULT, 0, &s.sound);
	sounds[pavadinimas] = s;
}

void Sound::loadStream(char *kelias, char *pavadinimas)
{
	SoundInfo s;
	system->createStream(kelias, FMOD_DEFAULT, 0, &s.sound);
	sounds[pavadinimas] = s;
}

void Sound::playSound(char *pavadinimas, bool loop, float garsumas )
{
	FMOD::Channel *channel;
	result = system->playSound(FMOD_CHANNEL_FREE, sounds[pavadinimas].sound, false, &channel);
		channel->setChannelGroup(masterGroup);
	if (loop == true)
		channel->setMode(FMOD_LOOP_NORMAL);
	else
		channel->setMode(FMOD_DEFAULT);
	channel->setVolume(garsumas);
	
}

void Sound::pause()
{
	masterGroup->setPaused(true);
}

void Sound::play()
{
	masterGroup->setPaused(false);
}


// garsuma lygis nuo 0 iki 1
void Sound::setVolume(float garsumas)
{
	masterGroup->setVolume(garsumas);

}
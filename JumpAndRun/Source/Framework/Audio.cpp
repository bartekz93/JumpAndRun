#include<allegro.h>
#include"Messages.h"
#include"Audio.h"

using namespace APPLICATION;

void AudioManager::Init()
{
	AudioDevice = OpenDevice();
	if (!AudioDevice) allegro_message("nie udalo sie utworzyc urzadzenia audio");
	SetSettings(Settings);
}

void AudioManager::SetSettings(const AudioSettings& as) 
{ 
	Settings = as; 
	UpdateVolume();
}

void AudioManager::UpdateVolume()
{
	for (int i=0;i<Streams.size();i++) Streams[i]->setVolume(Settings.MusicVolume);
	for (int i=0;i<Sounds.size();i++) Sounds[i]->setVolume(Settings.SoundVolume);
}

void AudioManager::LoadStream(const string& file, OutputStreamPtr& stream)
{
	stream = OpenSound(AudioDevice, file.c_str(), true);
	if (!stream) 
	{
		allegro_message(string("Nie udalo sie otworzyc "+file+".").c_str());
	}
	else Streams.push_back(stream);
}

void AudioManager::LoadSound(const string& file, OutputStreamPtr& sound)
{
	sound = OpenSound(AudioDevice, file.c_str(), false);
	if (!sound) 
	{
		allegro_message(string("Nie udalo sie otworzyc "+file+".").c_str());
	}
	Sounds.push_back(sound);
}

void AudioManager::Release()
{
	AudioDevice = 0;
	for (int i=0;i<Streams.size();i++) 
	{
		Streams[i]->stop();
		Streams[i] = 0;
	}
		
	for (int i=0;i<Sounds.size();i++) 
	{
		Sounds[i]->stop();
		Sounds[i] = 0;
	}
}
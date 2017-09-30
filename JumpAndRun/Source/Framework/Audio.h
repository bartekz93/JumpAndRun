#pragma once
#include<audiere.h>
#include<vector>
#include<string>

using namespace audiere;

namespace APPLICATION
{

struct AudioSettings
{
	float MusicVolume;
	float SoundVolume;

	AudioSettings() : MusicVolume(0.3), SoundVolume(0.3) {}
};



class AudioManager
{
private:
	AudioDevicePtr AudioDevice;
	AudioSettings Settings;

	std::vector<OutputStreamPtr> Streams;
	std::vector<OutputStreamPtr> Sounds;
public:
	AudioManager() {}
	AudioDevicePtr& GetAudioDevice() { return AudioDevice; }
	void Init();
	void SetSettings(const AudioSettings& as) ;
	void UpdateVolume();
	AudioSettings GetSettings() { return Settings; }
	void LoadStream(const std::string& file, OutputStreamPtr& stream);
	void LoadSound(const std::string& file, OutputStreamPtr& sound);
	void Release();
};

class AudioUser
{
protected:
	static AudioManager Audio;
};

};
#include "AudioManager.h"
#include "Audio.h"


AudioManager* AudioManager::GetInstance()
{
	static AudioManager instance;

	return &instance;
}

//非同期前に読み込むもの
void AudioManager::StartLoadAudio() {
	Audio::GetInstance()->LoadSound(AUDIO_TITLE, "Resources/Sound/BGM/BGM_title.wav");
	Audio::GetInstance()->LoadSound(AUDIO_LOAD, "Resources/Sound/BGM/BGM_load.wav");
}

void AudioManager::SecondLoadAudio() {
	Audio::GetInstance()->LoadSound(AUDIO_TUTORIAL, "Resources/Sound/BGM/BGM_tutorial.wav");
	Audio::GetInstance()->LoadSound(AUDIO_BATTLE, "Resources/Sound/BGM/BGM_boss.wav");
}
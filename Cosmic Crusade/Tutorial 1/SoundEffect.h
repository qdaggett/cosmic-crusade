#pragma once
#include "SoundEngine.h"

class SoundEffect
{
public:
	void initializeSounds();

	void updateSounds();

	void playSound(FMOD::Sound *soundName, FMOD::Channel **channelName);

	void playMusic(FMOD::Sound, FMOD::Channel);

	void stopSound(FMOD::Channel **channelName);

	void setPitchShift();

	bool hasPlayed;
	bool introHasPlayed;
	bool enemyHasShot;

	SoundEngine se;
	FMOD::Sound *failsound;
	FMOD::Sound *music;
	FMOD::Sound *shoot;
	FMOD::Sound *shoot2;
	FMOD::Sound *enemyHit;
	FMOD::Sound *playerHit;
	FMOD::Sound *enemyShot;
	FMOD::Sound *monologue;
	FMOD::Channel *channel1, *channel2, *channel3, *channel4, *channel5, *channel6, *channel7, *channel8, *channel9 = 0;
	FMOD_VECTOR soundPos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR soundVel = { 0.0f, 0.0f, 0.0f };
	FMOD_RESULT result;

	// DSP effects
	FMOD::DSP *pitchShift;

};
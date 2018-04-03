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
	bool introHasPlayed = false;
	bool winHasPlayed = false;
	bool loseHasPlayed = false;
	bool enemyHasShot;

	SoundEngine se;
	FMOD::Sound *failSound;
	FMOD::Sound *music;
	FMOD::Sound *shoot;
	FMOD::Sound *shoot2;
	FMOD::Sound *enemyHit;
	FMOD::Sound *playerHit;
	FMOD::Sound *enemyShot;
	FMOD::Sound *monologue;
	FMOD::Sound *speedUp;
	FMOD::Sound *speedDown;
	FMOD::Sound *ammoUp;
	FMOD::Sound *timeUp;
	FMOD::Sound *fuelUp;
	FMOD::Sound *winSound;
	FMOD::Channel *speedUpChannel, *speedDownChannel, *ammoUpChannel, *timeUpChannel, *fuelUpChannel, *winChannel, *failChannel, *monologueChannel, *musicChannel, *playerShootChannel, *shotgunShootChannel, *enemyHitChannel, *playerHitChannel = 0;
	FMOD_VECTOR soundPos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR soundVel = { 0.0f, 0.0f, 0.0f };
	FMOD_RESULT result;

	// DSP effects
	FMOD::DSP *pitchShift;
	FMOD::DSP *lowPass;
	FMOD::DSP *echo;
	FMOD::DSP *highPass;
	FMOD::DSP *distort;

};
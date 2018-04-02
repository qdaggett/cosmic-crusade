#include "SoundEffect.h"

void SoundEffect::initializeSounds()
{
	// Initializing sound engine
	se.Init();

	// Loading in sounds from .wav files
	se.system->createSound("sounds/failsound.wav", FMOD_3D, 0, &failsound);
	se.system->createSound("sounds/music.wav", FMOD_3D, 0, &music);
	se.system->createSound("sounds/playerShootBasic.wav", FMOD_3D, 0, &shoot);
	se.system->createSound("sounds/enemyHit.wav", FMOD_3D, 0, &enemyHit);
	se.system->createSound("sounds/playerHurt.wav", FMOD_3D, 0, &playerHit);
	se.system->createSound("sounds/enemyShoot.wav", FMOD_3D, 0, &enemyShot);
	se.system->createSound("sounds/shotgun.wav", FMOD_3D, 0, &shoot2);
	se.system->createSound("sounds/introDialogue.wav", FMOD_3D, 0, &monologue);
	se.system->createSound("sounds/ammoUp.wav", FMOD_3D, 0, &ammoUp);
	se.system->createSound("sounds/fuelUp.wav", FMOD_3D, 0, &fuelUp);
	se.system->createSound("sounds/timeUp.wav", FMOD_3D, 0, &timeUp);
	failsound->set3DMinMaxDistance(0.5f, 300.0f);

	// Setting looping (or lack thereof) for sounds
	failsound->setMode(FMOD_LOOP_OFF);
	music->setMode(FMOD_LOOP_NORMAL);
	shoot->setMode(FMOD_LOOP_OFF);
	enemyHit->setMode(FMOD_LOOP_OFF);
	playerHit->setMode(FMOD_LOOP_OFF);
	enemyShot->setMode(FMOD_LOOP_OFF);
	shoot2->setMode(FMOD_LOOP_OFF);
	monologue->setMode(FMOD_LOOP_OFF);
	ammoUp->setMode(FMOD_LOOP_OFF);
	fuelUp->setMode(FMOD_LOOP_OFF);
	timeUp->setMode(FMOD_LOOP_OFF);

	// DSP initialization
	se.system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &pitchShift);
	pitchShift->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.5f);

}



void SoundEffect::updateSounds()
{
	// Handles updating of sounds and error checking
	result = se.system->update();
	FModErrorCheck(result);
}


void SoundEffect::playSound(FMOD::Sound *soundName, FMOD::Channel **channelName)
{
	// Plays sounds
	FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

	result = se.system->playSound(soundName, 0, true, channelName);
	result = (*channelName)->set3DAttributes(&pos, &vel);
	result = (*channelName)->setPaused(false);
}

void SoundEffect::playMusic(FMOD::Sound soundName, FMOD::Channel channelName)
{
	// Might be used in the future

}

void SoundEffect::stopSound(FMOD::Channel **channelName)
{
	(*channelName)->stop();
}

// Work in progress function
void SoundEffect::setPitchShift()
{
	//pitchShift->getParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, (rand() % 1000) / 999.0f*1.5f + 0.5f);

}

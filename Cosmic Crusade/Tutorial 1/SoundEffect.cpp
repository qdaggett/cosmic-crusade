#include "SoundEffect.h"

void SoundEffect::initializeSounds()
{
	// Initializing sound engine
	se.Init();

	// Loading in sounds from .wav files
	se.system->createSound("sounds/failsound.wav", FMOD_3D, 0, &failsound);
	se.system->createSound("sounds/meme.wav", FMOD_3D, 0, &music);
	se.system->createSound("sounds/laser.wav", FMOD_3D, 0, &shoot);
	se.system->createSound("sounds/enemyhit.wav", FMOD_3D, 0, &enemyHit);
	se.system->createSound("sounds/hit.wav", FMOD_3D, 0, &playerHit);
	se.system->createSound("sounds/enemyshot.wav", FMOD_3D, 0, &enemyShot);
	failsound->set3DMinMaxDistance(0.5f, 300.0f);

	// Setting looping (or lack thereof) for sounds
	failsound->setMode(FMOD_LOOP_OFF);
	music->setMode(FMOD_LOOP_NORMAL);
	shoot->setMode(FMOD_LOOP_OFF);
	enemyHit->setMode(FMOD_LOOP_OFF);
	playerHit->setMode(FMOD_LOOP_OFF);
	enemyShot->setMode(FMOD_LOOP_OFF);

	// DSP initialization
	se.system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &pitchShift);
	pitchShift->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.5f);

}

// TEMPORARY SOLUTION
#define min(a,b)            (((a) < (b)) ? (a) : (b));

float Random(float a, float b)
{
	return (float(rand()) / float(RAND_MAX)) * abs(b - a) + min(a, b);
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
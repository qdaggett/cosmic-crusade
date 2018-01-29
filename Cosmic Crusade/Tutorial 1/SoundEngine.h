#pragma once

// Code from Game Sound tutorials
#include "include\FMOD\fmod.hpp"
#include <math.h>
#include "common.h"
#include "include\FMOD\fmod_errors.h"
#include <iostream>

using namespace std;

void FModErrorCheck(FMOD_RESULT result);

struct Listener
{
	// Listen data members
	FMOD_VECTOR pos = { 0.0f, 0.0f, -1.0f };
	FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
};

class SoundEngine
{
private:
	bool init; // Initialization boolean
	FMOD_RESULT result; // Used for error checking with FMOD

public:
	SoundEngine();
	~SoundEngine();
	bool Init();
	void CleanUp();

	FMOD::System  *system;
	unsigned int version;
	void *extraDriverData = 0;
	Listener listener;

};
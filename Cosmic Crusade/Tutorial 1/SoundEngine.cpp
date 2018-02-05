// Code from Game Sound tutorial
#include "SoundEngine.h"

void FModErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		cout << "FMod error: " << FMOD_ErrorString(result) << endl;
		system("pause");
	}
}

SoundEngine::SoundEngine()
{
	init = false;
}

SoundEngine::~SoundEngine()
{
	CleanUp();
}

void SoundEngine::CleanUp()
{
	result = system->close();
	FModErrorCheck(result);
	result = system->release();
	FModErrorCheck(result);

	init = false;
}


bool SoundEngine::Init()
{
	if (!init)
	{
		// Creation of sound system object

		result = FMOD::System_Create(&system);
		cout << "System created" << endl;
		FModErrorCheck(result);
		if (result != FMOD_OK)
		{
			init = false;
		}

		// Checking FMOD version
		result = system->getVersion(&version);
		FModErrorCheck(result);
		if (result != FMOD_OK)
		{
			init = false;
		}

		// Initializing the sound system
		result = system->init(100, FMOD_INIT_NORMAL, extraDriverData);
		FModErrorCheck(result);
		if (result != FMOD_OK)
		{
			init = false;
		}

		// Setting the distance units
		result = system->set3DSettings(1.0, 1.0f, 1.0f);
		FModErrorCheck(result);
		if (result != FMOD_OK)
		{
			init = false;
		}

		return init;
	}
}



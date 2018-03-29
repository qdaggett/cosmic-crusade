#include "PowerUp.h"
// Quinn Daggett - 100618734

PowerUp::PowerUp()
{
	updateTimer = new Timer();
	updateTimer->tick();
}

PowerUp::~PowerUp()
{

}

float PowerUp::getLifetime()
{
	return lifetime;

}





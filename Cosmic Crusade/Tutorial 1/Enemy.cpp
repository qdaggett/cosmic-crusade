#include "Enemy.h"

Enemy::Enemy()
{
	updateTimer = new Timer();
	updateTimer->tick();
}

Enemy::~Enemy()
{

	for (int i = 0; i < projectiles.size(); i++)
	{
		delete projectiles[i];
	}
}

std::vector<Projectile*> Enemy::getProjectiles()
{
	return projectiles;
}

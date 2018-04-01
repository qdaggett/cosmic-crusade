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

void Enemy::lerp(float x, float y, float dt)
{
	glm::vec2 pos = glm::vec2(((1 - dt) * location.x) + (dt * x), ((1 - dt) * location.y) + (dt * y));

	setLocation(pos.x, pos.y);
}
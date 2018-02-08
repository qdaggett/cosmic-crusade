#pragma once
#include "GameObject.h"
#include "Projectile.h"
#include "Player.h"
#include "glm\glm.hpp"

class Enemy : public GameObject
{
public:
	Enemy();
	~Enemy();

	virtual void update(std::vector<Player*> players, std::vector<Projectile*>*) = 0;
	virtual void shoot(std::vector<Player*> players, std::vector<Projectile*>*) = 0;
	virtual void Intialize() = 0;

	std::vector<Projectile*> getProjectiles();
	std::vector<Projectile*> projectiles;
	glm::vec2 velocity;

	Projectile projectile;

	Timer* updateTimer = nullptr;
	float localTime = 0.0f;
	float delay;
	int target = 0;
private:



};